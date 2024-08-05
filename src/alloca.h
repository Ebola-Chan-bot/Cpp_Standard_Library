#pragma once
#include_next <alloca.h>
#ifdef ARDUINO_ARCH_AVR
#include <libc-symbols.h>
#ifndef _ISOMAC

#undef __alloca

#ifdef __GNUC__
#define __alloca(size) __builtin_alloca(size)
#endif /* GCC.  */
extern int __libc_use_alloca(size_t size) __attribute__((const));
extern int __libc_alloca_cutoff(size_t size) __attribute__((const));
libc_hidden_proto(__libc_alloca_cutoff)

#define __MAX_ALLOCA_CUTOFF 65536

#if defined stackinfo_get_sp && defined stackinfo_sub_sp
#define alloca_account(size, avar) \
	({ void *old__ = stackinfo_get_sp ();					      \
     void *m__ = __alloca (size);					      \
     avar += stackinfo_sub_sp (old__);					      \
     m__; })
#else
#define alloca_account(size, avar) \
	({ size_t s__ = (size);						      \
     avar += s__;							      \
     __alloca (s__); })
#endif

#endif /* !_ISOMAC */
#endif