#ifdef ARDUINO_ARCH_AVR
/* Miscellaneous support functions for dynamic linker
   Copyright (C) 1997-2024 Free Software Foundation, Inc.
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

#include <_itoa.h>
#include <fcntl.h>
#include <ldsodefs.h>
#include <link.h>
#include <not-cancel.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/* Test whether given NAME matches any of the names of the given object.  */
int
_dl_name_match_p (const char *name, const struct link_map *map)
{
  if (strcmp (name, map->l_name) == 0)
    return 1;

  struct libname_list *runp = map->l_libname;

  while (runp != NULL)
    if (strcmp (name, runp->name) == 0)
      return 1;
    else
      /* Synchronize with the release MO store in add_name_to_object.
	 See CONCURRENCY NOTES in add_name_to_object in dl-load.c.  */
      runp = atomic_load_acquire (&runp->next);

  return 0;
}

unsigned long int
_dl_higher_prime_number (unsigned long int n)
{
  /* These are primes that are near, but slightly smaller than, a
     power of two.  */
  static const uint32_t primes[] = {
    UINT32_C (7),
    UINT32_C (13),
    UINT32_C (31),
    UINT32_C (61),
    UINT32_C (127),
    UINT32_C (251),
    UINT32_C (509),
    UINT32_C (1021),
    UINT32_C (2039),
    UINT32_C (4093),
    UINT32_C (8191),
    UINT32_C (16381),
    UINT32_C (32749),
    UINT32_C (65521),
    UINT32_C (131071),
    UINT32_C (262139),
    UINT32_C (524287),
    UINT32_C (1048573),
    UINT32_C (2097143),
    UINT32_C (4194301),
    UINT32_C (8388593),
    UINT32_C (16777213),
    UINT32_C (33554393),
    UINT32_C (67108859),
    UINT32_C (134217689),
    UINT32_C (268435399),
    UINT32_C (536870909),
    UINT32_C (1073741789),
    UINT32_C (2147483647),
				       /* 4294967291L */
    UINT32_C (2147483647) + UINT32_C (2147483644)
  };

  const uint32_t *low = &primes[0];
  const uint32_t *high = &primes[sizeof (primes) / sizeof (primes[0])];

  while (low != high)
    {
      const uint32_t *mid = low + (high - low) / 2;
      if (n > *mid)
       low = mid + 1;
      else
       high = mid;
    }

#if 0
  /* If we've run out of primes, abort.  */
  if (n > *low)
    {
      fprintf (stderr, "Cannot find prime bigger than %lu\n", n);
      abort ();
    }
#endif

  return *low;
}
#endif