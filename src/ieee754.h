/* Copyright (C) 1992-2024 Free Software Foundation, Inc.
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

#ifndef _IEEE754_H
#define _IEEE754_H 1

#include <features.h>
#include <stdint.h>
#include <bits/endian.h>

__BEGIN_DECLS

union ieee754_float
  {
    float f;

    /* This is the IEEE 754 single-precision format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	uint32_t negative:1;
	uint32_t exponent:8;
	uint32_t mantissa:23;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	uint32_t mantissa:23;
	uint32_t exponent:8;
	uint32_t negative:1;
#endif				/* Little endian.  */
      } ieee;

    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	uint32_t negative:1;
	uint32_t exponent:8;
	uint32_t quiet_nan:1;
	uint32_t mantissa:22;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	uint32_t mantissa:22;
	uint32_t quiet_nan:1;
	uint32_t exponent:8;
	uint32_t negative:1;
#endif				/* Little endian.  */
      } ieee_nan;
  };

#define IEEE754_FLOAT_BIAS	0x7f /* Added to exponent.  */


union ieee754_double
  {
    double d;

    /* This is the IEEE 754 double-precision format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	uint32_t negative:1;
	uint32_t exponent:11;
	/* Together these comprise the mantissa.  */
	uint32_t mantissa0:20;
	uint32_t mantissa1:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
# if	__FLOAT_WORD_ORDER == __BIG_ENDIAN
	uint32_t mantissa0:20;
	uint32_t exponent:11;
	uint32_t negative:1;
	uint32_t mantissa1:32;
# else
	/* Together these comprise the mantissa.  */
	uint32_t mantissa1:32;
	uint32_t mantissa0:20;
	uint32_t exponent:11;
	uint32_t negative:1;
# endif
#endif				/* Little endian.  */
      } ieee;

    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	uint32_t negative:1;
	uint32_t exponent:11;
	uint32_t quiet_nan:1;
	/* Together these comprise the mantissa.  */
	uint32_t mantissa0:19;
	uint32_t mantissa1:32;
#else
# if	__FLOAT_WORD_ORDER == __BIG_ENDIAN
	uint32_t mantissa0:19;
	uint32_t quiet_nan:1;
	uint32_t exponent:11;
	uint32_t negative:1;
	uint32_t mantissa1:32;
# else
	/* Together these comprise the mantissa.  */
	uint32_t mantissa1:32;
	uint32_t mantissa0:19;
	uint32_t quiet_nan:1;
	uint32_t exponent:11;
	uint32_t negative:1;
# endif
#endif
      } ieee_nan;
  };

#define IEEE754_DOUBLE_BIAS	0x3ff /* Added to exponent.  */


union ieee854_long_double
  {
    long double d;

    /* This is the IEEE 854 double-extended-precision format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	uint32_t negative:1;
	uint32_t exponent:15;
	uint32_t empty:16;
	uint32_t mantissa0:32;
	uint32_t mantissa1:32;
#endif
#if	__BYTE_ORDER == __LITTLE_ENDIAN
# if	__FLOAT_WORD_ORDER == __BIG_ENDIAN
	uint32_t exponent:15;
	uint32_t negative:1;
	uint32_t empty:16;
	uint32_t mantissa0:32;
	uint32_t mantissa1:32;
# else
	uint32_t mantissa1:32;
	uint32_t mantissa0:32;
	uint32_t exponent:15;
	uint32_t negative:1;
	uint32_t empty:16;
# endif
#endif
      } ieee;

    /* This is for NaNs in the IEEE 854 double-extended-precision format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	uint32_t negative:1;
	uint32_t exponent:15;
	uint32_t empty:16;
	uint32_t one:1;
	uint32_t quiet_nan:1;
	uint32_t mantissa0:30;
	uint32_t mantissa1:32;
#endif
#if	__BYTE_ORDER == __LITTLE_ENDIAN
# if	__FLOAT_WORD_ORDER == __BIG_ENDIAN
	uint32_t exponent:15;
	uint32_t negative:1;
	uint32_t empty:16;
	uint32_t mantissa0:30;
	uint32_t quiet_nan:1;
	uint32_t one:1;
	uint32_t mantissa1:32;
# else
	uint32_t mantissa1:32;
	uint32_t mantissa0:30;
	uint32_t quiet_nan:1;
	uint32_t one:1;
	uint32_t exponent:15;
	uint32_t negative:1;
	uint32_t empty:16;
# endif
#endif
      } ieee_nan;
  };

#define IEEE854_LONG_DOUBLE_BIAS 0x3fff

__END_DECLS

#endif /* ieee754.h */