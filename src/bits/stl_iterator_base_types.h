#pragma once
#if __cplusplus < 202002L
// SAM和ESP32也有此文件，但定义的iterator_traits是C++17旧版，无法重定义，这里只能替换
//  Types used in iterator implementation -*- C++ -*-

// Copyright (C) 2001-2024 Free Software Foundation, Inc.
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file bits/stl_iterator_base_types.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 *
 *  This file contains all of the general iterator-related utility types,
 *  such as iterator_traits and struct iterator.
 */

#ifndef _STL_ITERATOR_BASE_TYPES_H
#define _STL_ITERATOR_BASE_TYPES_H 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <iterator_base>
#include <bits/iterator_concepts.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION
  // 110
  /// Contiguous iterators point to objects stored contiguously in memory.
  struct contiguous_iterator_tag : public random_access_iterator_tag
  {
  };
  // 116
  // 142
  /**
   *  @brief  Traits class for iterators.
   *
   *  This class does nothing but define nested typedefs.  The general
   *  version simply @a forwards the nested typedefs from the Iterator
   *  argument.  Specialized versions for pointers and pointers-to-const
   *  provide tighter, more correct semantics.
   */
  template <typename _Iterator>
  struct iterator_traits;

#if __cplusplus >= 201103L
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2408. SFINAE-friendly common_type/iterator_traits is missing in C++14
  template <typename _Iterator, typename = __void_t<>>
  struct __iterator_traits
  {
  };

/*本库有类似concepts的定义但没有真正的concept关键词，所以不定义__cpp_lib_concepts，但不需要这一段，会导致不明确
#if !__cpp_lib_concepts

  template <typename _Iterator>
  struct __iterator_traits<_Iterator,
                           __void_t<typename _Iterator::iterator_category,
                                    typename _Iterator::value_type,
                                    typename _Iterator::difference_type,
                                    typename _Iterator::pointer,
                                    typename _Iterator::reference>>
  {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type value_type;
    typedef typename _Iterator::difference_type difference_type;
    typedef typename _Iterator::pointer pointer;
    typedef typename _Iterator::reference reference;
  };
#endif // ! concepts
*/

  template <typename _Iterator>
  struct iterator_traits
      : public __iterator_traits<_Iterator>
  {
  };

#else  // ! C++11
  template <typename _Iterator>
  struct iterator_traits
  {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type value_type;
    typedef typename _Iterator::difference_type difference_type;
    typedef typename _Iterator::pointer pointer;
    typedef typename _Iterator::reference reference;
  };
#endif // C++11

  // #if __cplusplus > 201703L
  /// Partial specialization for object pointer types.
  template <typename _Tp>
#if __cpp_concepts >= 201907L
    requires is_object_v<_Tp>
#endif
  struct iterator_traits<_Tp *>
  {
    using iterator_concept = contiguous_iterator_tag;
    using iterator_category = random_access_iterator_tag;
    using value_type = remove_cv_t<_Tp>;
    using difference_type = ptrdiff_t;
    using pointer = _Tp *;
    using reference = _Tp &;
  };
  /*
  #else
    /// Partial specialization for pointer types.
    template <typename _Tp>
    struct iterator_traits<_Tp *>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp value_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp *pointer;
      typedef _Tp &reference;
    };

    /// Partial specialization for const pointer types.
    template <typename _Tp>
    struct iterator_traits<const _Tp *>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp value_type;
      typedef ptrdiff_t difference_type;
      typedef const _Tp *pointer;
      typedef const _Tp &reference;
    };
  #endif*/

  /**
   *  This function is not a part of the C++ standard but is syntactic
   *  sugar for internal library use only.
   */
  template <typename _Iter>
  __attribute__((__always_inline__)) inline _GLIBCXX_CONSTEXPR
      typename iterator_traits<_Iter>::iterator_category
      __iterator_category(const _Iter &)
  {
    return typename iterator_traits<_Iter>::iterator_category();
  }

  ///@}

#if __cplusplus >= 201103L
  template <typename _Iter>
  using __iter_category_t = typename iterator_traits<_Iter>::iterator_category;

  template <typename _InIter>
  using _RequireInputIter =
      __enable_if_t<is_convertible<__iter_category_t<_InIter>,
                                   input_iterator_tag>::value>;

  template <typename _It,
            typename _Cat = __iter_category_t<_It>>
  struct __is_random_access_iter
      : is_base_of<random_access_iterator_tag, _Cat>
  {
    typedef is_base_of<random_access_iterator_tag, _Cat> _Base;
    enum
    {
      __value = _Base::value
    };
  };
#else
  template <typename _It, typename _Traits = iterator_traits<_It>,
            typename _Cat = typename _Traits::iterator_category>
  struct __is_random_access_iter
  {
    enum
    {
      __value = __is_base_of(random_access_iterator_tag, _Cat)
    };
  };
#endif

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif /* _STL_ITERATOR_BASE_TYPES_H */
#else
#include_next <bits/stl_iterator_base_types.h>
#endif