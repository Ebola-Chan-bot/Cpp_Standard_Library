#pragma once
#ifdef ARDUINO_ARCH_AVR
// The  -*- C++ -*- type traits classes for internal use in libstdc++

// Copyright (C) 2000-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/cpp_type_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/type_traits}
 */

// Written by Gabriel Dos Reis <dosreis@cmla.ens-cachan.fr>

#ifndef _CPP_TYPE_TRAITS_H
#define _CPP_TYPE_TRAITS_H 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <type_traits>

//
// This file provides some compile-time information about various types.
// These representations were designed, on purpose, to be constant-expressions
// and not types as found in <bits/type_traits.h>.  In particular, they
// can be used in control structures and the optimizer hopefully will do
// the obvious thing.
//
// Why integral expressions, and not functions nor types?
// Firstly, these compile-time entities are used as template-arguments
// so function return values won't work:  We need compile-time entities.
// We're left with types and constant  integral expressions.
// Secondly, from the point of view of ease of use, type-based compile-time
// information is -not- *that* convenient.  One has to write lots of
// overloaded functions and to hope that the compiler will select the right
// one. As a net effect, the overall structure isn't very clear at first
// glance.
// Thirdly, partial ordering and overload resolution (of function templates)
// is highly costly in terms of compiler-resource.  It is a Good Thing to
// keep these resource consumption as least as possible.
//
// See valarray_array.h for a case use.
//
// -- Gaby (dosreis@cmla.ens-cachan.fr) 2000-03-06.
//
// Update 2005: types are also provided and <bits/type_traits.h> has been
// removed.
//

extern "C++"
{

	namespace std _GLIBCXX_VISIBILITY(default)
	{
		_GLIBCXX_BEGIN_NAMESPACE_VERSION
		// 75
		template <bool>
		struct __truth_type
		{
			typedef __false_type __type;
		};

		template <>
		struct __truth_type<true>
		{
			typedef __true_type __type;
		};

		// N.B. The conversions to bool are needed due to the issue
		// explained in c++/19404.
		template <class _Sp, class _Tp>
		struct __traitor
		{
			enum
			{
				__value = bool(_Sp::__value) || bool(_Tp::__value)
			};
			typedef typename __truth_type<__value>::__type __type;
		};
		// 92
		//  285
		//
		//  Floating point types
		//
		template <typename _Tp>
		struct __is_floating
		{
			enum
			{
				__value = 0
			};
			typedef __false_type __type;
		};

		// three specializations (float, double and 'long double')
		template <>
		struct __is_floating<float>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};

		template <>
		struct __is_floating<double>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};

		template <>
		struct __is_floating<long double>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};

#ifdef __STDCPP_FLOAT16_T__
		template <>
		struct __is_floating<_Float16>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif

#ifdef __STDCPP_FLOAT32_T__
		template <>
		struct __is_floating<_Float32>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif

#ifdef __STDCPP_FLOAT64_T__
		template <>
		struct __is_floating<_Float64>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif

#ifdef __STDCPP_FLOAT128_T__
		template <>
		struct __is_floating<_Float128>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif

#ifdef __STDCPP_BFLOAT16_T__
		template <>
		struct __is_floating<__gnu_cxx::__bfloat16_t>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif

		//
		// Pointer types
		//
#if _GLIBCXX_USE_BUILTIN_TRAIT(__is_pointer)
		template <typename _Tp, bool _IsPtr = __is_pointer(_Tp)>
		struct __is_pointer : __truth_type<_IsPtr>
		{
			enum
			{
				__value = _IsPtr
			};
		};
#else
		template <typename _Tp>
		struct __is_pointer
		{
			enum
			{
				__value = 0
			};
			typedef __false_type __type;
		};

		template <typename _Tp>
		struct __is_pointer<_Tp *>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif

		//
		// An arithmetic type is an integer type or a floating point type
		//
		template <typename _Tp>
		struct __is_arithmetic
			: public __traitor<__is_integer<_Tp>, __is_floating<_Tp>>{};

		//
		// A scalar type is an arithmetic type or a pointer type
		//
		template <typename _Tp>
		struct __is_scalar
			: public __traitor<__is_arithmetic<_Tp>, __is_pointer<_Tp>>{};
		// 403
		// 429
		template <typename _Tp>
		struct __is_byte
		{
			enum
			{
				__value = 0
			};
			typedef __false_type __type;
		};

		template <>
		struct __is_byte<char>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};

		template <>
		struct __is_byte<signed char>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};

		template <>
		struct __is_byte<unsigned char>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};

#if __cplusplus >= 201703L
		enum class byte : unsigned char;

		template <>
		struct __is_byte<byte>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif // C++17

#ifdef _GLIBCXX_USE_CHAR8_T
		template <>
		struct __is_byte<char8_t>
		{
			enum
			{
				__value = 1
			};
			typedef __true_type __type;
		};
#endif
		// 477
		_GLIBCXX_END_NAMESPACE_VERSION
	} // namespace
} // extern "C++"

#endif //_CPP_TYPE_TRAITS_H
#else
#include_next <bits/cpp_type_traits.h>
#endif