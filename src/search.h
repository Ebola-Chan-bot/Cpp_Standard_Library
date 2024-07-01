#ifndef _SEARCH_H

#ifndef _ISOMAC

/* Now define the internal interfaces.  */
extern void __hdestroy (void);
extern void *__tsearch (const void *__key, void **__rootp,
			__compar_fn_t compar);
libc_hidden_proto (__tsearch)
extern void *__tfind (const void *__key, void *const *__rootp,
		      __compar_fn_t compar);
libc_hidden_proto (__tfind)
extern void *__tdelete (const void *__key, void **__rootp,
			__compar_fn_t compar);
libc_hidden_proto (__tdelete)
#endif
#endif