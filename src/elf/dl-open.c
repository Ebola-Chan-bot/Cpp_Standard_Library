#ifdef ARDUINO_ARCH_AVR
/* Load a shared object at runtime, relocate it, and run its initializer.
   Copyright (C) 1996-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>		/* Check whether MAP_COPY is defined.  */
#include <sys/param.h>
#include <libc-lock.h>
#include <ldsodefs.h>
#include <sysdep-cancel.h>
#include <tls.h>
#include <stap-probe.h>
#include <atomic.h>
#include <array_length.h>
#include <confdefs.h>

/* We must be careful not to leave us in an inconsistent state.  Thus we
   catch any error and re-raise it after cleaning up.  */

struct dl_open_args
{
  const char *file;
  int mode;
  /* This is the caller of the dlopen() function.  */
  const void *caller_dlopen;
  struct link_map *map;
  /* Namespace ID.  */
  Lmid_t nsid;

  /* Original value of _ns_global_scope_pending_adds.  Set by
     dl_open_worker.  Only valid if nsid is a real namespace
     (non-negative).  */
  unsigned int original_global_scope_pending_adds;

  /* Set to true by dl_open_worker if libc.so was already loaded into
     the namespace at the time dl_open_worker was called.  This is
     used to determine whether libc.so early initialization has
     already been done before, and whether to roll back the cached
     libc_map value in the namespace in case of a dlopen failure.  */
  bool libc_already_loaded;

  /* Set to true if the end of dl_open_worker_begin was reached.  */
  bool worker_continue;

  /* Original parameters to the program and the current environment.  */
  int argc;
  char **argv;
  char **env;
};

/* Called in case the global scope cannot be extended.  */
static void __attribute__ ((noreturn))
add_to_global_resize_failure (struct link_map *new)
{
  _dl_signal_error (ENOMEM, new->l_libname->name, NULL,
		    N_ ("cannot extend global scope"));
}

/* Grow the global scope array for the namespace, so that all the new
   global objects can be added later in add_to_global_update, without
   risk of memory allocation failure.  add_to_global_resize raises
   exceptions for memory allocation errors.  */
static void
add_to_global_resize (struct link_map *new)
{
  struct link_namespaces *ns = &GL (dl_ns)[new->l_ns];

  /* Count the objects we have to put in the global scope.  */
  unsigned int to_add = 0;
  for (unsigned int cnt = 0; cnt < new->l_searchlist.r_nlist; ++cnt)
    if (new->l_searchlist.r_list[cnt]->l_global == 0)
      ++to_add;

  /* The symbols of the new objects and its dependencies are to be
     introduced into the global scope that will be used to resolve
     references from other dynamically-loaded objects.

     The global scope is the searchlist in the main link map.  We
     extend this list if necessary.  There is one problem though:
     since this structure was allocated very early (before the libc
     is loaded) the memory it uses is allocated by the malloc()-stub
     in the ld.so.  When we come here these functions are not used
     anymore.  Instead the malloc() implementation of the libc is
     used.  But this means the block from the main map cannot be used
     in an realloc() call.  Therefore we allocate a completely new
     array the first time we have to add something to the locale scope.  */

  if (__builtin_add_overflow (ns->_ns_global_scope_pending_adds, to_add,
			      &ns->_ns_global_scope_pending_adds))
    add_to_global_resize_failure (new);

  unsigned int new_size = 0; /* 0 means no new allocation.  */
  void *old_global = NULL; /* Old allocation if free-able.  */

  /* Minimum required element count for resizing.  Adjusted below for
     an exponential resizing policy.  */
  size_t required_new_size;
  if (__builtin_add_overflow (ns->_ns_main_searchlist->r_nlist,
			      ns->_ns_global_scope_pending_adds,
			      &required_new_size))
    add_to_global_resize_failure (new);

  if (ns->_ns_global_scope_alloc == 0)
    {
      if (__builtin_add_overflow (required_new_size, 8, &new_size))
	add_to_global_resize_failure (new);
    }
  else if (required_new_size > ns->_ns_global_scope_alloc)
    {
      if (__builtin_mul_overflow (required_new_size, 2, &new_size))
	add_to_global_resize_failure (new);

      /* The old array was allocated with our malloc, not the minimal
	 malloc.  */
      old_global = ns->_ns_main_searchlist->r_list;
    }

  if (new_size > 0)
    {
      size_t allocation_size;
      if (__builtin_mul_overflow (new_size, sizeof (struct link_map *),
				  &allocation_size))
	add_to_global_resize_failure (new);
      struct link_map **new_global = malloc (allocation_size);
      if (new_global == NULL)
	add_to_global_resize_failure (new);

      /* Copy over the old entries.  */
      memcpy (new_global, ns->_ns_main_searchlist->r_list,
	      ns->_ns_main_searchlist->r_nlist * sizeof (struct link_map *));

      ns->_ns_global_scope_alloc = new_size;
      ns->_ns_main_searchlist->r_list = new_global;

      if (!RTLD_SINGLE_THREAD_P)
	THREAD_GSCOPE_WAIT ();

      free (old_global);
    }
}

/* Actually add the new global objects to the global scope.  Must be
   called after add_to_global_resize.  This function cannot fail.  */
static void
add_to_global_update (struct link_map *new)
{
  struct link_namespaces *ns = &GL (dl_ns)[new->l_ns];

  /* Now add the new entries.  */
  unsigned int new_nlist = ns->_ns_main_searchlist->r_nlist;
  for (unsigned int cnt = 0; cnt < new->l_searchlist.r_nlist; ++cnt)
    {
      struct link_map *map = new->l_searchlist.r_list[cnt];

      if (map->l_global == 0)
	{
	  map->l_global = 1;

	  /* The array has been resized by add_to_global_resize.  */
	  assert (new_nlist < ns->_ns_global_scope_alloc);

	  ns->_ns_main_searchlist->r_list[new_nlist++] = map;

	  /* We modify the global scope.  Report this.  */
	  if (__glibc_unlikely (GLRO(dl_debug_mask) & DL_DEBUG_SCOPES))
	    _dl_debug_printf ("\nadd %s [%lu] to global scope\n",
			      map->l_name, map->l_ns);
	}
    }

  /* Some of the pending adds have been performed by the loop above.
     Adjust the counter accordingly.  */
  unsigned int added = new_nlist - ns->_ns_main_searchlist->r_nlist;
  assert (added <= ns->_ns_global_scope_pending_adds);
  ns->_ns_global_scope_pending_adds -= added;

  atomic_write_barrier ();
  ns->_ns_main_searchlist->r_nlist = new_nlist;
}

/* Search link maps in all namespaces for the DSO that contains the object at
   address ADDR.  Returns the pointer to the link map of the matching DSO, or
   NULL if a match is not found.  */
struct link_map *
_dl_find_dso_for_object (const ElfW(Addr) addr)
{
  struct link_map *l;

  /* Find the highest-addressed object that ADDR is not below.  */
  for (Lmid_t ns = 0; ns < GL(dl_nns); ++ns)
    for (l = GL(dl_ns)[ns]._ns_loaded; l != NULL; l = l->l_next)
      if (addr >= l->l_map_start && addr < l->l_map_end
	  && (l->l_contiguous
	      || _dl_addr_inside_object (l, (ElfW(Addr)) addr)))
	{
	  assert (ns == l->l_ns);
	  return l;
	}
  return NULL;
}
rtld_hidden_def (_dl_find_dso_for_object);

/* Return true if NEW is found in the scope for MAP.  */
static size_t
scope_has_map (struct link_map *map, struct link_map *new)
{
  size_t cnt;
  for (cnt = 0; map->l_scope[cnt] != NULL; ++cnt)
    if (map->l_scope[cnt] == &new->l_searchlist)
      return true;
  return false;
}

/* Return the length of the scope for MAP.  */
static size_t
scope_size (struct link_map *map)
{
  size_t cnt;
  for (cnt = 0; map->l_scope[cnt] != NULL; )
    ++cnt;
  return cnt;
}

/* Resize the scopes of depended-upon objects, so that the new object
   can be added later without further allocation of memory.  This
   function can raise an exceptions due to malloc failure.  */
static void
resize_scopes (struct link_map *new)
{
  /* If the file is not loaded now as a dependency, add the search
     list of the newly loaded object to the scope.  */
  for (unsigned int i = 0; i < new->l_searchlist.r_nlist; ++i)
    {
      struct link_map *imap = new->l_searchlist.r_list[i];

      /* If the initializer has been called already, the object has
	 not been loaded here and now.  */
      if (imap->l_init_called && imap->l_type == lt_loaded)
	{
	  if (scope_has_map (imap, new))
	    /* Avoid duplicates.  */
	    continue;

	  size_t cnt = scope_size (imap);
	  if (__glibc_unlikely (cnt + 1 >= imap->l_scope_max))
	    {
	      /* The l_scope array is too small.  Allocate a new one
		 dynamically.  */
	      size_t new_size;
	      struct r_scope_elem **newp;

	      if (imap->l_scope != imap->l_scope_mem
		  && imap->l_scope_max < array_length (imap->l_scope_mem))
		{
		  /* If the current l_scope memory is not pointing to
		     the static memory in the structure, but the
		     static memory in the structure is large enough to
		     use for cnt + 1 scope entries, then switch to
		     using the static memory.  */
		  new_size = array_length (imap->l_scope_mem);
		  newp = imap->l_scope_mem;
		}
	      else
		{
		  new_size = imap->l_scope_max * 2;
		  newp = (struct r_scope_elem **)
		    malloc (new_size * sizeof (struct r_scope_elem *));
		  if (newp == NULL)
		    _dl_signal_error (ENOMEM, "dlopen", NULL,
				      N_("cannot create scope list"));
		}

	      /* Copy the array and the terminating NULL.  */
	      memcpy (newp, imap->l_scope,
		      (cnt + 1) * sizeof (imap->l_scope[0]));
	      struct r_scope_elem **old = imap->l_scope;

	      imap->l_scope = newp;

	      if (old != imap->l_scope_mem)
		_dl_scope_free (old);

	      imap->l_scope_max = new_size;
	    }
	}
    }
}

/* Call _dl_add_to_slotinfo with DO_ADD set to false, to allocate
   space in GL (dl_tls_dtv_slotinfo_list).  This can raise an
   exception.  The return value is true if any of the new objects use
   TLS.  */
static bool
resize_tls_slotinfo (struct link_map *new)
{
  bool any_tls = false;
  for (unsigned int i = 0; i < new->l_searchlist.r_nlist; ++i)
    {
      struct link_map *imap = new->l_searchlist.r_list[i];

      /* Only add TLS memory if this object is loaded now and
	 therefore is not yet initialized.  */
      if (! imap->l_init_called && imap->l_tls_blocksize > 0)
	{
	  _dl_add_to_slotinfo (imap, false);
	  any_tls = true;
	}
    }
  return any_tls;
}

/* Mark the objects as NODELETE if required.  This is delayed until
   after dlopen failure is not possible, so that _dl_close can clean
   up objects if necessary.  */
static void
activate_nodelete (struct link_map *new)
{
  /* It is necessary to traverse the entire namespace.  References to
     objects in the global scope and unique symbol bindings can force
     NODELETE status for objects outside the local scope.  */
  for (struct link_map *l = GL (dl_ns)[new->l_ns]._ns_loaded; l != NULL;
       l = l->l_next)
    if (l->l_nodelete_pending)
      {
	if (__glibc_unlikely (GLRO (dl_debug_mask) & DL_DEBUG_FILES))
	  _dl_debug_printf ("activating NODELETE for %s [%lu]\n",
			    l->l_name, l->l_ns);

	/* The flag can already be true at this point, e.g. a signal
	   handler may have triggered lazy binding and set NODELETE
	   status immediately.  */
	l->l_nodelete_active = true;

	/* This is just a debugging aid, to indicate that
	   activate_nodelete has run for this map.  */
	l->l_nodelete_pending = false;
      }
}

/* Relocate the object L.  *RELOCATION_IN_PROGRESS controls whether
   the debugger is notified of the start of relocation processing.  */
static void
_dl_open_relocate_one_object (struct dl_open_args *args, struct r_debug *r,
			      struct link_map *l, int reloc_mode,
			      bool *relocation_in_progress)
{
  if (l->l_real->l_relocated)
    return;

  if (!*relocation_in_progress)
    {
      /* Notify the debugger that relocations are about to happen.  */
      LIBC_PROBE (reloc_start, 2, args->nsid, r);
      *relocation_in_progress = true;
    }

#ifdef SHARED
  if (__glibc_unlikely (GLRO(dl_profile) != NULL))
    {
      /* If this here is the shared object which we want to profile
	 make sure the profile is started.  We can find out whether
	 this is necessary or not by observing the `_dl_profile_map'
	 variable.  If it was NULL but is not NULL afterwards we must
	 start the profiling.  */
      struct link_map *old_profile_map = GL(dl_profile_map);

      _dl_relocate_object (l, l->l_scope, reloc_mode | RTLD_LAZY, 1);

      if (old_profile_map == NULL && GL(dl_profile_map) != NULL)
	{
	  /* We must prepare the profiling.  */
	  _dl_start_profile ();

	  /* Prevent unloading the object.  */
	  GL(dl_profile_map)->l_nodelete_active = true;
	}
    }
  else
#endif
    _dl_relocate_object (l, l->l_scope, reloc_mode, 0);
}


/* struct dl_init_args and call_dl_init are used to call _dl_init with
   exception handling disabled.  */
struct dl_init_args
{
  struct link_map *new;
  int argc;
  char **argv;
  char **env;
};

static void
call_dl_init (void *closure)
{
  struct dl_init_args *args = closure;
  _dl_init (args->new, args->argc, args->argv, args->env);
}
#endif