#pragma once
// Functor implementations -*- C++ -*-

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

/** @file bits/stl_function.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */
#ifdef ARDUINO_ARCH_AVR
#if __cplusplus > 201103L
#include <bits/move.h>
#endif
#include <functional>
#else
#include_next <bits/stl_function.h>
#endif
#ifdef ARDUINO_ARCH_SAM
#include <type_traits>
#endif
#ifndef ARDUINO_ARCH_ESP32
namespace std _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION
  /** @}  */
#endif
#ifdef ARDUINO_ARCH_AVR
  // 20.3.2 arithmetic

  /** @defgroup arithmetic_functors Arithmetic Function Object Classes
   *  @ingroup functors
   *
   *  The library provides function objects for basic arithmetic operations.
   *  See the documentation for @link functors function objects @endlink
   *  for examples of their use.
   *
   *  @{
   */

#if __glibcxx_transparent_operators // C++ >= 14
  struct __is_transparent;          // undefined

  template <typename _Tp = void>
  struct modulus;

  template <typename _Tp = void>
  struct negate;
#endif

#ifdef __glibcxx_transparent_operators // C++ >= 14

  /// One of the @link arithmetic_functors math functors@endlink.
  template <>
  struct modulus<void>
  {
    template <typename _Tp, typename _Up>
    _GLIBCXX14_CONSTEXPR auto
    operator()(_Tp &&__t, _Up &&__u) const
        noexcept(noexcept(std::forward<_Tp>(__t) % std::forward<_Up>(__u)))
            -> decltype(std::forward<_Tp>(__t) % std::forward<_Up>(__u))
    {
      return std::forward<_Tp>(__t) % std::forward<_Up>(__u);
    }

    typedef __is_transparent is_transparent;
  };

  /// One of the @link arithmetic_functors math functors@endlink.
  template <>
  struct negate<void>
  {
    template <typename _Tp>
    _GLIBCXX14_CONSTEXPR auto
    operator()(_Tp &&__t) const
        noexcept(noexcept(-std::forward<_Tp>(__t)))
            -> decltype(-std::forward<_Tp>(__t))
    {
      return -std::forward<_Tp>(__t);
    }

    typedef __is_transparent is_transparent;
  };
#endif
  /** @}  */

  // 20.3.3 comparisons
  /** @defgroup comparison_functors Comparison Classes
   *  @ingroup functors
   *
   *  The library provides six wrapper functors for all the basic comparisons
   *  in C++, like @c <.
   *
   *  @{
   */
#if __glibcxx_transparent_operators // C++ >= 14

  template <typename _Tp = void>
  struct greater_equal;

  template <typename _Tp = void>
  struct less_equal;
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  // Partial specialization of std::greater_equal for pointers.
  template <typename _Tp>
  struct greater_equal<_Tp *> : public binary_function<_Tp *, _Tp *, bool>
  {
    _GLIBCXX14_CONSTEXPR
    bool
    operator()(_Tp *__x, _Tp *__y) const _GLIBCXX_NOTHROW
    {
#if __cplusplus >= 201402L
      if (std::__is_constant_evaluated())
        return __x >= __y;
#endif
      return (__UINTPTR_TYPE__)__x >= (__UINTPTR_TYPE__)__y;
    }
  };

  // Partial specialization of std::less_equal for pointers.
  template <typename _Tp>
  struct less_equal<_Tp *> : public binary_function<_Tp *, _Tp *, bool>
  {
    _GLIBCXX14_CONSTEXPR
    bool
    operator()(_Tp *__x, _Tp *__y) const _GLIBCXX_NOTHROW
    {
#if __cplusplus >= 201402L
      if (std::__is_constant_evaluated())
        return __x <= __y;
#endif
      return (__UINTPTR_TYPE__)__x <= (__UINTPTR_TYPE__)__y;
    }
  };
#pragma GCC diagnostic pop

#ifdef __glibcxx_transparent_operators // C++ >= 14

  /// One of the @link comparison_functors comparison functors@endlink.
  template <>
  struct greater_equal<void>
  {
    template <typename _Tp, typename _Up>
    constexpr auto
    operator()(_Tp &&__t, _Up &&__u) const
        noexcept(noexcept(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)))
            -> decltype(std::forward<_Tp>(__t) >= std::forward<_Up>(__u))
    {
      return _S_cmp(std::forward<_Tp>(__t), std::forward<_Up>(__u),
                    __ptr_cmp<_Tp, _Up>{});
    }

    template <typename _Tp, typename _Up>
    constexpr bool
    operator()(_Tp *__t, _Up *__u) const noexcept
    {
      return greater_equal<common_type_t<_Tp *, _Up *>>{}(__t, __u);
    }

    typedef __is_transparent is_transparent;

  private:
    template <typename _Tp, typename _Up>
    static constexpr auto
    _S_cmp(_Tp &&__t, _Up &&__u, false_type) -> decltype(std::forward<_Tp>(__t) >= std::forward<_Up>(__u))
    {
      return std::forward<_Tp>(__t) >= std::forward<_Up>(__u);
    }

    template <typename _Tp, typename _Up>
    static constexpr bool
    _S_cmp(_Tp &&__t, _Up &&__u, true_type) noexcept
    {
      return greater_equal<const volatile void *>{}(
          static_cast<const volatile void *>(std::forward<_Tp>(__t)),
          static_cast<const volatile void *>(std::forward<_Up>(__u)));
    }

    // True if there is no viable operator>= member function.
    template <typename _Tp, typename _Up, typename = void>
    struct __not_overloaded2 : true_type
    {
    };

    // False if we can call T.operator>=(U)
    template <typename _Tp, typename _Up>
    struct __not_overloaded2<_Tp, _Up, __void_t<decltype(std::declval<_Tp>().operator>=(std::declval<_Up>()))>>
        : false_type
    {
    };

    // True if there is no overloaded operator>= for these operands.
    template <typename _Tp, typename _Up, typename = void>
    struct __not_overloaded : __not_overloaded2<_Tp, _Up>
    {
    };

    // False if we can call operator>=(T,U)
    template <typename _Tp, typename _Up>
    struct __not_overloaded<_Tp, _Up, __void_t<decltype(operator>=(std::declval<_Tp>(), std::declval<_Up>()))>>
        : false_type
    {
    };

    template <typename _Tp, typename _Up>
    using __ptr_cmp = __and_<__not_overloaded<_Tp, _Up>,
                             is_convertible<_Tp, const volatile void *>,
                             is_convertible<_Up, const volatile void *>>;
  };

  /// One of the @link comparison_functors comparison functors@endlink.
  template <>
  struct less_equal<void>
  {
    template <typename _Tp, typename _Up>
    constexpr auto
    operator()(_Tp &&__t, _Up &&__u) const
        noexcept(noexcept(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)))
            -> decltype(std::forward<_Tp>(__t) <= std::forward<_Up>(__u))
    {
      return _S_cmp(std::forward<_Tp>(__t), std::forward<_Up>(__u),
                    __ptr_cmp<_Tp, _Up>{});
    }

    template <typename _Tp, typename _Up>
    constexpr bool
    operator()(_Tp *__t, _Up *__u) const noexcept
    {
      return less_equal<common_type_t<_Tp *, _Up *>>{}(__t, __u);
    }

    typedef __is_transparent is_transparent;

  private:
    template <typename _Tp, typename _Up>
    static constexpr auto
    _S_cmp(_Tp &&__t, _Up &&__u, false_type) -> decltype(std::forward<_Tp>(__t) <= std::forward<_Up>(__u))
    {
      return std::forward<_Tp>(__t) <= std::forward<_Up>(__u);
    }

    template <typename _Tp, typename _Up>
    static constexpr bool
    _S_cmp(_Tp &&__t, _Up &&__u, true_type) noexcept
    {
      return less_equal<const volatile void *>{}(
          static_cast<const volatile void *>(std::forward<_Tp>(__t)),
          static_cast<const volatile void *>(std::forward<_Up>(__u)));
    }

    // True if there is no viable operator<= member function.
    template <typename _Tp, typename _Up, typename = void>
    struct __not_overloaded2 : true_type
    {
    };

    // False if we can call T.operator<=(U)
    template <typename _Tp, typename _Up>
    struct __not_overloaded2<_Tp, _Up, __void_t<decltype(std::declval<_Tp>().operator<=(std::declval<_Up>()))>>
        : false_type
    {
    };

    // True if there is no overloaded operator<= for these operands.
    template <typename _Tp, typename _Up, typename = void>
    struct __not_overloaded : __not_overloaded2<_Tp, _Up>
    {
    };

    // False if we can call operator<=(T,U)
    template <typename _Tp, typename _Up>
    struct __not_overloaded<_Tp, _Up, __void_t<decltype(operator<=(std::declval<_Tp>(), std::declval<_Up>()))>>
        : false_type
    {
    };

    template <typename _Tp, typename _Up>
    using __ptr_cmp = __and_<__not_overloaded<_Tp, _Up>,
                             is_convertible<_Tp, const volatile void *>,
                             is_convertible<_Up, const volatile void *>>;
  };
#endif // __glibcxx_transparent_operators

#ifdef __glibcxx_transparent_operators // C++ >= 14
  template <typename _Tp = void>
  struct bit_and;

  template <typename _Tp = void>
  struct bit_or;

  template <typename _Tp = void>
  struct bit_xor;

  template <typename _Tp = void>
  struct bit_not;
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 660. Missing Bitwise Operations.
  template <typename _Tp>
  struct bit_and : public binary_function<_Tp, _Tp, _Tp>
  {
    _GLIBCXX14_CONSTEXPR
    _Tp
    operator()(const _Tp &__x, const _Tp &__y) const
    {
      return __x & __y;
    }
  };

  template <typename _Tp>
  struct bit_or : public binary_function<_Tp, _Tp, _Tp>
  {
    _GLIBCXX14_CONSTEXPR
    _Tp
    operator()(const _Tp &__x, const _Tp &__y) const
    {
      return __x | __y;
    }
  };

  template <typename _Tp>
  struct bit_xor : public binary_function<_Tp, _Tp, _Tp>
  {
    _GLIBCXX14_CONSTEXPR
    _Tp
    operator()(const _Tp &__x, const _Tp &__y) const
    {
      return __x ^ __y;
    }
  };

  template <typename _Tp>
  struct bit_not : public unary_function<_Tp, _Tp>
  {
    _GLIBCXX14_CONSTEXPR
    _Tp
    operator()(const _Tp &__x) const
    {
      return ~__x;
    }
  };
#pragma GCC diagnostic pop

#ifdef __glibcxx_transparent_operators // C++ >= 14
  template <>
  struct bit_and<void>
  {
    template <typename _Tp, typename _Up>
    _GLIBCXX14_CONSTEXPR auto
    operator()(_Tp &&__t, _Up &&__u) const
        noexcept(noexcept(std::forward<_Tp>(__t) & std::forward<_Up>(__u)))
            -> decltype(std::forward<_Tp>(__t) & std::forward<_Up>(__u))
    {
      return std::forward<_Tp>(__t) & std::forward<_Up>(__u);
    }

    typedef __is_transparent is_transparent;
  };

  template <>
  struct bit_or<void>
  {
    template <typename _Tp, typename _Up>
    _GLIBCXX14_CONSTEXPR auto
    operator()(_Tp &&__t, _Up &&__u) const
        noexcept(noexcept(std::forward<_Tp>(__t) | std::forward<_Up>(__u)))
            -> decltype(std::forward<_Tp>(__t) | std::forward<_Up>(__u))
    {
      return std::forward<_Tp>(__t) | std::forward<_Up>(__u);
    }

    typedef __is_transparent is_transparent;
  };

  template <>
  struct bit_xor<void>
  {
    template <typename _Tp, typename _Up>
    _GLIBCXX14_CONSTEXPR auto
    operator()(_Tp &&__t, _Up &&__u) const
        noexcept(noexcept(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u)))
            -> decltype(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u))
    {
      return std::forward<_Tp>(__t) ^ std::forward<_Up>(__u);
    }

    typedef __is_transparent is_transparent;
  };

  template <>
  struct bit_not<void>
  {
    template <typename _Tp>
    _GLIBCXX14_CONSTEXPR auto
    operator()(_Tp &&__t) const
        noexcept(noexcept(~std::forward<_Tp>(__t)))
            -> decltype(~std::forward<_Tp>(__t))
    {
      return ~std::forward<_Tp>(__t);
    }

    typedef __is_transparent is_transparent;
  };
#endif // C++14

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  template <typename _Tp>
  struct _Identity
      : public unary_function<_Tp, _Tp>
  {
    _Tp &
    operator()(_Tp &__x) const
    {
      return __x;
    }

    const _Tp &
    operator()(const _Tp &__x) const
    {
      return __x;
    }
  };

  // Partial specialization, avoids confusing errors in e.g. std::set<const T>.
  template <typename _Tp>
  struct _Identity<const _Tp> : _Identity<_Tp>
  {
  };

  template <typename _Pair>
  struct _Select1st
      : public unary_function<_Pair, typename _Pair::first_type>
  {
    typename _Pair::first_type &
    operator()(_Pair &__x) const
    {
      return __x.first;
    }

    const typename _Pair::first_type &
    operator()(const _Pair &__x) const
    {
      return __x.first;
    }

#if __cplusplus >= 201103L
    template <typename _Pair2>
    typename _Pair2::first_type &
    operator()(_Pair2 &__x) const
    {
      return __x.first;
    }

    template <typename _Pair2>
    const typename _Pair2::first_type &
    operator()(const _Pair2 &__x) const
    {
      return __x.first;
    }
#endif
  };

  template <typename _Pair>
  struct _Select2nd
      : public unary_function<_Pair, typename _Pair::second_type>
  {
    typename _Pair::second_type &
    operator()(_Pair &__x) const
    {
      return __x.second;
    }

    const typename _Pair::second_type &
    operator()(const _Pair &__x) const
    {
      return __x.second;
    }
  };
#pragma GCC diagnostic pop
#endif // ARDUINO_ARCH_AVR
#ifndef ARDUINO_ARCH_ESP32
  /** @}  */

#ifdef __glibcxx_transparent_operators // C++ >= 14
  template <typename _Func, typename _SfinaeType, typename = __void_t<>>
  struct __has_is_transparent
  {
  };

  template <typename _Func, typename _SfinaeType>
  struct __has_is_transparent<_Func, _SfinaeType,
                              __void_t<typename _Func::is_transparent>>
  {
    typedef void type;
  };

  template <typename _Func, typename _SfinaeType>
  using __has_is_transparent_t = typename __has_is_transparent<_Func, _SfinaeType>::type;
#endif

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#if (__cplusplus < 201103L) || _GLIBCXX_USE_DEPRECATED
// # include <backward/binders.h>
#endif
#endif