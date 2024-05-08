#pragma once
#ifdef ARDUINO_ARCH_AVR
/* Copyright (C) 1995-2024 Free Software Foundation, Inc.
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

/*
 *      ISO C99 Standard: 7.24
 *	Extended multibyte and wide character utilities	<wchar.h>
 */

#ifndef _WCHAR_H
#define _WCHAR_H 1

#define __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
#include <bits/libc-header-start.h>
#include <sys/cdefs.h>

#define __need_size_t
#define __need_wchar_t
#define __need_NULL
#include <stddef.h>

#define __need___va_list
#include <stdarg.h>

#if defined __USE_XOPEN2K || defined __USE_XOPEN2K8
# ifdef __GNUC__
#  ifndef _VA_LIST_DEFINED
typedef __gnuc_va_list va_list;
#   define _VA_LIST_DEFINED
#  endif
# else
#  include <stdarg.h>
# endif
#endif

/* Tell the caller that we provide correct C++ prototypes.  */
#if defined __cplusplus && __GNUC_PREREQ (4, 4)
# define __CORRECT_ISO_CPP_WCHAR_H_PROTO
#endif

#ifndef WCHAR_MIN
/* These constants might also be defined in <inttypes.h>.  */
# define WCHAR_MIN __WCHAR_MIN
# define WCHAR_MAX __WCHAR_MAX
#endif

#ifndef WEOF
# define WEOF (0xffffffffu)
#endif

/* All versions of XPG prior to the publication of ISO C99 required
   the bulk of <wctype.h>'s declarations to appear in this header
   (because <wctype.h> did not exist prior to C99).  In POSIX.1-2001
   those declarations were marked as XSI extensions; in -2008 they
   were additionally marked as obsolescent.  _GNU_SOURCE mode
   anticipates the removal of these declarations in the next revision
   of POSIX.  */
#if (defined __USE_XOPEN && !defined __USE_GNU \
     && !(defined __USE_XOPEN2K && !defined __USE_XOPEN2KXSI))
# include <bits/wctype-wchar.h>
#endif

__BEGIN_DECLS

/* This incomplete type is defined in <time.h> but needed here because
   of `wcsftime'.  */
struct tm;


/* Copy SRC to DEST.  */
extern wchar_t *wcscpy (wchar_t *__restrict __dest,
			const wchar_t *__restrict __src)
     __THROW __nonnull ((1, 2));

/* Copy no more than N wide-characters of SRC to DEST.  */
extern wchar_t *wcsncpy (wchar_t *__restrict __dest,
			 const wchar_t *__restrict __src, size_t __n)
     __THROW __nonnull ((1, 2));

#ifdef __USE_MISC
/* Copy at most N - 1 characters from SRC to DEST.  */
extern size_t wcslcpy (wchar_t *__restrict __dest,
		       const wchar_t *__restrict __src, size_t __n)
  __THROW __nonnull ((1, 2)) __attr_access ((__write_only__, 1, 3));

/* Append SRC to DEST, possibly with truncation to keep the total size
   below N.  */
extern size_t wcslcat (wchar_t *__restrict __dest,
		       const wchar_t *__restrict __src, size_t __n)
  __THROW __nonnull ((1, 2))  __attr_access ((__read_write__, 1, 3));
#endif

/* Append SRC onto DEST.  */
extern wchar_t *wcscat (wchar_t *__restrict __dest,
			const wchar_t *__restrict __src)
     __THROW __nonnull ((1, 2));
/* Append no more than N wide-characters of SRC onto DEST.  */
extern wchar_t *wcsncat (wchar_t *__restrict __dest,
			 const wchar_t *__restrict __src, size_t __n)
     __THROW __nonnull ((1, 2));

#ifdef __USE_ISOC99
/* Convert initial portion of wide string NPTR to `long long int'
   representation.  */
__extension__
extern long long int wcstoll (const wchar_t *__restrict __nptr,
			      wchar_t **__restrict __endptr, int __base)
     __THROW;

/* Convert initial portion of wide string NPTR to `unsigned long long int'
   representation.  */
__extension__
extern unsigned long long int wcstoull (const wchar_t *__restrict __nptr,
					wchar_t **__restrict __endptr,
					int __base) __THROW;
#endif /* ISO C99.  */

#ifdef __USE_GNU
/* Convert initial portion of wide string NPTR to `long long int'
   representation.  */
__extension__
extern long long int wcstoq (const wchar_t *__restrict __nptr,
			     wchar_t **__restrict __endptr, int __base)
     __THROW;

/* Convert initial portion of wide string NPTR to `unsigned long long int'
   representation.  */
__extension__
extern unsigned long long int wcstouq (const wchar_t *__restrict __nptr,
				       wchar_t **__restrict __endptr,
				       int __base) __THROW;
#endif /* Use GNU.  */

/* Versions of the above functions that handle '0b' and '0B' prefixes
   in base 0 or 2.  */
#if __GLIBC_USE (C23_STRTOL)
# ifdef __REDIRECT
extern long int __REDIRECT_NTH (wcstol, (const wchar_t *__restrict __nptr,
					 wchar_t **__restrict __endptr,
					 int __base), __isoc23_wcstol);
extern unsigned long int __REDIRECT_NTH (wcstoul,
					 (const wchar_t *__restrict __nptr,
					  wchar_t **__restrict __endptr,
					  int __base), __isoc23_wcstoul);
__extension__
extern long long int __REDIRECT_NTH (wcstoll,
				     (const wchar_t *__restrict __nptr,
				      wchar_t **__restrict __endptr,
				      int __base), __isoc23_wcstoll);
__extension__
extern unsigned long long int __REDIRECT_NTH (wcstoull,
					      (const wchar_t *__restrict __nptr,
					       wchar_t **__restrict __endptr,
					       int __base), __isoc23_wcstoull);
#  ifdef __USE_GNU
__extension__
extern long long int __REDIRECT_NTH (wcstoq, (const wchar_t *__restrict __nptr,
					      wchar_t **__restrict __endptr,
					      int __base), __isoc23_wcstoll);
__extension__
extern unsigned long long int __REDIRECT_NTH (wcstouq,
					      (const wchar_t *__restrict __nptr,
					       wchar_t **__restrict __endptr,
					       int __base), __isoc23_wcstoull);
#  endif
# else
extern long int __isoc23_wcstol (const wchar_t *__restrict __nptr,
				 wchar_t **__restrict __endptr, int __base)
     __THROW;
extern unsigned long int __isoc23_wcstoul (const wchar_t *__restrict __nptr,
					   wchar_t **__restrict __endptr,
					   int __base)
     __THROW;
__extension__
extern long long int __isoc23_wcstoll (const wchar_t *__restrict __nptr,
				       wchar_t **__restrict __endptr,
				       int __base)
     __THROW;
__extension__
extern unsigned long long int __isoc23_wcstoull (const wchar_t *__restrict __nptr,
						 wchar_t **__restrict __endptr,
						 int __base)
     __THROW;
#  define wcstol __isoc23_wcstol
#  define wcstoul __isoc23_wcstoul
#  define wcstoll __isoc23_wcstoll
#  define wcstoull __isoc23_wcstoull
#  ifdef __USE_GNU
#   define wcstoq __isoc23_wcstoll
#   define wcstouq __isoc23_wcstoull
#  endif
# endif
#endif

#ifdef __USE_GNU
/* Parallel versions of the functions above which take the locale to
   use as an additional parameter.  These are GNU extensions inspired
   by the POSIX.1-2008 extended locale API.  */
extern long int wcstol_l (const wchar_t *__restrict __nptr,
			  wchar_t **__restrict __endptr, int __base,
			  locale_t __loc) __THROW;

extern unsigned long int wcstoul_l (const wchar_t *__restrict __nptr,
				    wchar_t **__restrict __endptr,
				    int __base, locale_t __loc) __THROW;

__extension__
extern long long int wcstoll_l (const wchar_t *__restrict __nptr,
				wchar_t **__restrict __endptr,
				int __base, locale_t __loc) __THROW;

__extension__
extern unsigned long long int wcstoull_l (const wchar_t *__restrict __nptr,
					  wchar_t **__restrict __endptr,
					  int __base, locale_t __loc)
     __THROW;

/* Versions of the above functions that handle '0b' and '0B' prefixes
   in base 0 or 2.  */
# if __GLIBC_USE (C23_STRTOL)
#  ifdef __REDIRECT
extern long int __REDIRECT_NTH (wcstol_l, (const wchar_t *__restrict __nptr,
					   wchar_t **__restrict __endptr,
					   int __base, locale_t __loc),
				__isoc23_wcstol_l);
extern unsigned long int __REDIRECT_NTH (wcstoul_l,
					 (const wchar_t *__restrict __nptr,
					  wchar_t **__restrict __endptr,
					  int __base, locale_t __loc),
					 __isoc23_wcstoul_l);
__extension__
extern long long int __REDIRECT_NTH (wcstoll_l,
				     (const wchar_t *__restrict __nptr,
				      wchar_t **__restrict __endptr,
				      int __base, locale_t __loc),
				     __isoc23_wcstoll_l);
__extension__
extern unsigned long long int __REDIRECT_NTH (wcstoull_l,
					      (const wchar_t *__restrict __nptr,
					       wchar_t **__restrict __endptr,
					       int __base, locale_t __loc),
					      __isoc23_wcstoull_l);
#  else
extern long int __isoc23_wcstol_l (const wchar_t *__restrict __nptr,
				   wchar_t **__restrict __endptr, int __base,
				   locale_t __loc) __THROW;
extern unsigned long int __isoc23_wcstoul_l (const wchar_t *__restrict __nptr,
					     wchar_t **__restrict __endptr,
					     int __base, locale_t __loc)
     __THROW;
__extension__
extern long long int __isoc23_wcstoll_l (const wchar_t *__restrict __nptr,
					 wchar_t **__restrict __endptr,
					 int __base, locale_t __loc)
     __THROW;
__extension__
extern unsigned long long int __isoc23_wcstoull_l (const wchar_t *__restrict __nptr,
						   wchar_t **__restrict __endptr,
						   int __base, locale_t __loc)
     __THROW;
#   define wcstol_l __isoc23_wcstol_l
#   define wcstoul_l __isoc23_wcstoul_l
#   define wcstoll_l __isoc23_wcstoll_l
#   define wcstoull_l __isoc23_wcstoull_l
#  endif
# endif

extern double wcstod_l (const wchar_t *__restrict __nptr,
			wchar_t **__restrict __endptr, locale_t __loc)
     __THROW;

extern float wcstof_l (const wchar_t *__restrict __nptr,
		       wchar_t **__restrict __endptr, locale_t __loc)
     __THROW;

extern long double wcstold_l (const wchar_t *__restrict __nptr,
			      wchar_t **__restrict __endptr,
			      locale_t __loc) __THROW;

# if __HAVE_FLOAT16
extern _Float16 wcstof16_l (const wchar_t *__restrict __nptr,
			    wchar_t **__restrict __endptr,
			    locale_t __loc) __THROW;
# endif

# if __HAVE_FLOAT32
extern _Float32 wcstof32_l (const wchar_t *__restrict __nptr,
			    wchar_t **__restrict __endptr,
			    locale_t __loc) __THROW;
# endif

# if __HAVE_FLOAT64
extern _Float64 wcstof64_l (const wchar_t *__restrict __nptr,
			    wchar_t **__restrict __endptr,
			    locale_t __loc) __THROW;
# endif

# if __HAVE_FLOAT128
extern _Float128 wcstof128_l (const wchar_t *__restrict __nptr,
			      wchar_t **__restrict __endptr,
			      locale_t __loc) __THROW;
# endif

# if __HAVE_FLOAT32X
extern _Float32x wcstof32x_l (const wchar_t *__restrict __nptr,
			      wchar_t **__restrict __endptr,
			      locale_t __loc) __THROW;
# endif

# if __HAVE_FLOAT64X
extern _Float64x wcstof64x_l (const wchar_t *__restrict __nptr,
			      wchar_t **__restrict __endptr,
			      locale_t __loc) __THROW;
# endif

# if __HAVE_FLOAT128X
extern _Float128x wcstof128x_l (const wchar_t *__restrict __nptr,
				wchar_t **__restrict __endptr,
				locale_t __loc) __THROW;
# endif
#endif	/* use GNU */


#ifdef __USE_XOPEN2K8
/* Copy SRC to DEST, returning the address of the terminating L'\0' in
   DEST.  */
extern wchar_t *wcpcpy (wchar_t *__restrict __dest,
			const wchar_t *__restrict __src) __THROW;

/* Copy no more than N characters of SRC to DEST, returning the address of
   the last character written into DEST.  */
extern wchar_t *wcpncpy (wchar_t *__restrict __dest,
			 const wchar_t *__restrict __src, size_t __n)
     __THROW;
#endif


/* Wide character I/O functions.  */

#if defined __USE_XOPEN2K8 || __GLIBC_USE (LIB_EXT2)
# ifndef __attr_dealloc_fclose
#   if defined __has_builtin
#     if __has_builtin (__builtin_fclose)
/* If the attribute macro hasn't been defined yet (by <stdio.h>) and
   fclose is a built-in, use it.  */
#      define __attr_dealloc_fclose __attr_dealloc (__builtin_fclose, 1)
#     endif
#   endif
# endif
# ifndef __attr_dealloc_fclose
#  define __attr_dealloc_fclose /* empty */
# endif
#endif

#if defined __USE_ISOC95 || defined __USE_UNIX98
/* Write formatted output to stdout.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int wprintf (const wchar_t *__restrict __format, ...)
     /* __attribute__ ((__format__ (__wprintf__, 1, 2))) */;
/* Write formatted output of at most N characters to S.  */
extern int swprintf (wchar_t *__restrict __s, size_t __n,
		     const wchar_t *__restrict __format, ...)
     __THROW /* __attribute__ ((__format__ (__wprintf__, 3, 4))) */;
/* Write formatted output to stdout from argument list ARG.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int vwprintf (const wchar_t *__restrict __format,
		     __gnuc_va_list __arg)
     /* __attribute__ ((__format__ (__wprintf__, 1, 0))) */;
/* Write formatted output of at most N character to S from argument
   list ARG.  */
extern int vswprintf (wchar_t *__restrict __s, size_t __n,
		      const wchar_t *__restrict __format,
		      __gnuc_va_list __arg)
     __THROW /* __attribute__ ((__format__ (__wprintf__, 3, 0))) */;
/* Read formatted input from stdin.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int wscanf (const wchar_t *__restrict __format, ...)
     /* __attribute__ ((__format__ (__wscanf__, 1, 2))) */;
/* Read formatted input from S.  */
extern int swscanf (const wchar_t *__restrict __s,
		    const wchar_t *__restrict __format, ...)
     __THROW /* __attribute__ ((__format__ (__wscanf__, 2, 3))) */;

/* For historical reasons, the C99-compliant versions of the scanf
   functions are at alternative names.  When __LDBL_COMPAT or
   __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI are in effect, this is handled in
   bits/wchar-ldbl.h.  */
# if !__GLIBC_USE (DEPRECATED_SCANF) && !defined __LDBL_COMPAT \
     && __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI == 0
#  if __GLIBC_USE (C23_STRTOL)
#   ifdef __REDIRECT
extern int __REDIRECT (fwscanf, (__FILE *__restrict __stream,
				 const wchar_t *__restrict __format, ...),
		       __isoc23_fwscanf)
     /* __attribute__ ((__format__ (__wscanf__, 2, 3))) */;
extern int __REDIRECT (wscanf, (const wchar_t *__restrict __format, ...),
		       __isoc23_wscanf)
     /* __attribute__ ((__format__ (__wscanf__, 1, 2))) */;
extern int __REDIRECT_NTH (swscanf, (const wchar_t *__restrict __s,
				     const wchar_t *__restrict __format,
				     ...), __isoc23_swscanf)
     /* __attribute__ ((__format__ (__wscanf__, 2, 3))) */;
#   else
extern int __isoc23_fwscanf (__FILE *__restrict __stream,
			     const wchar_t *__restrict __format, ...);
extern int __isoc23_wscanf (const wchar_t *__restrict __format, ...);
extern int __isoc23_swscanf (const wchar_t *__restrict __s,
			     const wchar_t *__restrict __format, ...)
     __THROW;
#    define fwscanf __isoc23_fwscanf
#    define wscanf __isoc23_wscanf
#    define swscanf __isoc23_swscanf
#   endif
#  endif
# endif

#endif /* Use ISO C95, C99 and Unix98. */

#ifdef __USE_ISOC99
/* Read formatted input from stdin into argument list ARG.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int vwscanf (const wchar_t *__restrict __format,
		    __gnuc_va_list __arg)
     /* __attribute__ ((__format__ (__wscanf__, 1, 0))) */;
/* Read formatted input from S into argument list ARG.  */
extern int vswscanf (const wchar_t *__restrict __s,
		     const wchar_t *__restrict __format,
		     __gnuc_va_list __arg)
     __THROW /* __attribute__ ((__format__ (__wscanf__, 2, 0))) */;

/* Same redirection as above for the v*wscanf family.  */
# if !__GLIBC_USE (DEPRECATED_SCANF) \
     && (!defined __LDBL_COMPAT || !defined __REDIRECT) \
     && (defined __STRICT_ANSI__ || defined __USE_XOPEN2K) \
     && __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI == 0
#  if __GLIBC_USE (C23_STRTOL)
#   ifdef __REDIRECT
extern int __REDIRECT (vfwscanf, (__FILE *__restrict __s,
				  const wchar_t *__restrict __format,
				  __gnuc_va_list __arg), __isoc23_vfwscanf)
     /* __attribute__ ((__format__ (__wscanf__, 2, 0))) */;
extern int __REDIRECT (vwscanf, (const wchar_t *__restrict __format,
				 __gnuc_va_list __arg), __isoc23_vwscanf)
     /* __attribute__ ((__format__ (__wscanf__, 1, 0))) */;
extern int __REDIRECT_NTH (vswscanf, (const wchar_t *__restrict __s,
				      const wchar_t *__restrict __format,
				      __gnuc_va_list __arg), __isoc23_vswscanf)
     /* __attribute__ ((__format__ (__wscanf__, 2, 0))) */;
#   else
extern int __isoc23_vfwscanf (__FILE *__restrict __s,
			      const wchar_t *__restrict __format,
			      __gnuc_va_list __arg);
extern int __isoc23_vwscanf (const wchar_t *__restrict __format,
			     __gnuc_va_list __arg);
extern int __isoc23_vswscanf (const wchar_t *__restrict __s,
			      const wchar_t *__restrict __format,
			      __gnuc_va_list __arg) __THROW;
#    define vfwscanf __isoc23_vfwscanf
#    define vwscanf __isoc23_vwscanf
#    define vswscanf __isoc23_vswscanf
#   endif
#  endif
# endif

#endif /* Use ISO C99. */


#ifdef __USE_GNU
/* These are defined to be equivalent to the `char' functions defined
   in POSIX.1:1996.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern wint_t getwc_unlocked (__FILE *__stream);
extern wint_t getwchar_unlocked (void);

/* This is the wide character version of a GNU extension.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern wint_t fgetwc_unlocked (__FILE *__stream);

/* Faster version when locking is not necessary.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern wint_t fputwc_unlocked (wchar_t __wc, __FILE *__stream);

/* These are defined to be equivalent to the `char' functions defined
   in POSIX.1:1996.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern wint_t putwc_unlocked (wchar_t __wc, __FILE *__stream);
extern wint_t putwchar_unlocked (wchar_t __wc);


/* This function does the same as `fgetws' but does not lock the stream.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern wchar_t *fgetws_unlocked (wchar_t *__restrict __ws, int __n,
				 __FILE *__restrict __stream);

/* This function does the same as `fputws' but does not lock the stream.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int fputws_unlocked (const wchar_t *__restrict __ws,
			    __FILE *__restrict __stream);
#endif


/* Format TP into S according to FORMAT.
   Write no more than MAXSIZE wide characters and return the number
   of wide characters written, or 0 if it would exceed MAXSIZE.  */
extern size_t wcsftime (wchar_t *__restrict __s, size_t __maxsize,
			const wchar_t *__restrict __format,
			const struct tm *__restrict __tp) __THROW;

# ifdef __USE_GNU
/* Similar to `wcsftime' but takes the information from
   the provided locale and not the global locale.  */
extern size_t wcsftime_l (wchar_t *__restrict __s, size_t __maxsize,
			  const wchar_t *__restrict __format,
			  const struct tm *__restrict __tp,
			  locale_t __loc) __THROW;
# endif

/* Define some macros helping to catch buffer overflows.  */
#if __USE_FORTIFY_LEVEL > 0 && defined __fortify_function
/* Declare all functions from bits/wchar2-decl.h first.  */
# include <bits/wchar2-decl.h>
#endif

/* The following headers provide asm redirections.  These redirections must
   appear before the first usage of these functions, e.g. in bits/wchar.h.  */
#if defined __LDBL_COMPAT || __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI == 1
# include <bits/wchar-ldbl.h>
#endif

#if __USE_FORTIFY_LEVEL > 0 && defined __fortify_function
/* Now include the function definitions and redirects too.  */
# include <bits/wchar2.h>
#endif

__END_DECLS

#endif /* wchar.h  */

#else
#include "Cpp_Standard_Library.h"
#include CSL_OfficialC(wchar.h)
#endif