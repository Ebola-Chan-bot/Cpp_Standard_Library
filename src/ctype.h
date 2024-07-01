#pragma once
#include_next <ctype.h>
#ifndef ARDUINO_ARCH_ESP32
#ifndef _CTYPE_H

#include <ctype/ctype.h>
#include <libc-symbols.h>

#ifndef _ISOMAC
/* Initialize ctype locale data.  */
extern void __ctype_init (void);
libc_hidden_proto (__ctype_init)

/* ctype/ctype.h defined this as a macro and we don't want to #undef it.
   So defeat macro expansion with parens for this declaration.  */
extern int (__isctype) (int __c, int __mask);

libc_hidden_proto (tolower)
libc_hidden_proto (toupper)

# ifndef __cplusplus

/* These accessors are used by the optimized macros to find the
   thread-local cache of ctype information from the current thread's
   locale.  For inside libc, define them as inlines using the _NL_CURRENT
   accessors.  We don't use _NL_CURRENT_LOCALE->__ctype_b here because we
   want to cause a link-time ref to _nl_current_LC_CTYPE under
   NL_CURRENT_INDIRECT.  */

#  include <locale_avr/localeinfo.h>
#  include <libc-tsd.h>

#  ifdef CTYPE_EXTERN_INLINE	/* Used by ctype/ctype-info.c, which see.  */
//原版GLIBC把CTYPE_EXTERN_INLINE定义为extern inline，实测这并不能避免头文件中的重定义问题，不清楚是怎么回事。只有ctype-info.c编译单元会定义CTYPE_EXTERN_INLINE，所以仅在此时编译这些定义。
__libc_tsd_define (extern, const uint16_t *, CTYPE_B)
__libc_tsd_define (extern, const int32_t *, CTYPE_TOUPPER)
__libc_tsd_define (extern, const int32_t *, CTYPE_TOLOWER)


CTYPE_EXTERN_INLINE const uint16_t ** __attribute__ ((const))
__ctype_b_loc (void)
{
  return __libc_tsd_address (const uint16_t *, CTYPE_B);
}

CTYPE_EXTERN_INLINE const int32_t ** __attribute__ ((const))
__ctype_toupper_loc (void)
{
  return __libc_tsd_address (const int32_t *, CTYPE_TOUPPER);
}

CTYPE_EXTERN_INLINE const int32_t ** __attribute__ ((const))
__ctype_tolower_loc (void)
{
  return __libc_tsd_address (const int32_t *, CTYPE_TOLOWER);
}

#  endif
#  ifndef __NO_CTYPE
/* The spec says that isdigit must only match the decimal digits.  We
   can check this without a memory access.  */
#   undef isdigit
#   define isdigit(c) ({ int __c = (c); __c >= '0' && __c <= '9'; })
#   undef isdigit_l
#   define isdigit_l(c, l) ({ int __c = (c); __c >= '0' && __c <= '9'; })
#   undef __isdigit_l
#   define __isdigit_l(c, l) ({ int __c = (c); __c >= '0' && __c <= '9'; })
#  endif  /* Not __NO_CTYPE.  */

# endif	/* IS_IN (libc).  */
#endif  /* Not _ISOMAC.  */

#endif /* ctype.h */
#endif