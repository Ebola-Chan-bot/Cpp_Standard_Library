// -*- C++ -*-

// Copyright (C) 2005-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file ext/type_traits.h
 *  This file is a GNU extension to the Standard C++ Library.
 */
#pragma once
#ifdef ARDUINO_ARCH_AVR
#include "../bits/c++config.h"
extern "C++"
{
	namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
	{
		_GLIBCXX_BEGIN_NAMESPACE_VERSION
		// 42
		//  Define a nested type if some predicate holds.
		template <bool, typename>
		struct __enable_if
		{
		};

		template <typename _Tp>
		struct __enable_if<true, _Tp>
		{
			typedef _Tp __type;
		};
		// 51
		_GLIBCXX_END_NAMESPACE_VERSION
	} // namespace
} // extern "C++"
#else
#include "../Cpp_Standard_Library.h"
#include CSL_Official(ext/type_traits.h)
#endif