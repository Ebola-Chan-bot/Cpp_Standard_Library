#pragma once
#ifdef ARDUINO_ARCH_AVR
// Pair implementation -*- C++ -*-

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
 * Copyright (c) 1996,1997
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

/** @file bits/stl_pair.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 */

#ifndef _STL_PAIR_H
#define _STL_PAIR_H 1

#if __cplusplus >= 201103L
# include <type_traits>    // for std::__decay_and_strip
# include <bits/move.h>    // for std::move / std::forward, and std::swap
#endif
#if __cplusplus >= 202002L
# include <compare>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup utilities
   *  @{
   */

#if __cplusplus >= 201103L
  /// Tag type for piecewise construction of std::pair objects.
  struct piecewise_construct_t { explicit piecewise_construct_t() = default; };

  /// Tag for piecewise construction of std::pair objects.
  _GLIBCXX17_INLINE constexpr piecewise_construct_t piecewise_construct =
    piecewise_construct_t();

  /// @cond undocumented

  // Forward declarations.
  template<typename _T1, typename _T2>
    struct pair;

  template<typename...>
    class tuple;

  // Declarations of std::array and its std::get overloads, so that
  // std::tuple_cat can use them if <tuple> is included before <array>.
  // We also declare the other std::get overloads here so that they're
  // visible to the P2165R4 tuple-like constructors of pair and tuple.
  template<typename _Tp, size_t _Nm>
    struct array;

  template<size_t...>
    struct _Index_tuple;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(pair<_Tp1, _Tp2>& __in) noexcept;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(pair<_Tp1, _Tp2>&& __in) noexcept;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(const pair<_Tp1, _Tp2>& __in) noexcept;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(const pair<_Tp1, _Tp2>&& __in) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr __tuple_element_t<__i, tuple<_Elements...>>&
    get(tuple<_Elements...>& __t) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr const __tuple_element_t<__i, tuple<_Elements...>>&
    get(const tuple<_Elements...>& __t) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr __tuple_element_t<__i, tuple<_Elements...>>&&
    get(tuple<_Elements...>&& __t) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr const __tuple_element_t<__i, tuple<_Elements...>>&&
    get(const tuple<_Elements...>&& __t) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr _Tp&
    get(array<_Tp, _Nm>&) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr _Tp&&
    get(array<_Tp, _Nm>&&) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr const _Tp&
    get(const array<_Tp, _Nm>&) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr const _Tp&&
    get(const array<_Tp, _Nm>&&) noexcept;

#if ! __cpp_lib_concepts
  // Concept utility functions, reused in conditionally-explicit
  // constructors.
  // See PR 70437, don't look at is_constructible or
  // is_convertible if the types are the same to
  // avoid querying those properties for incomplete types.
  template <bool, typename _T1, typename _T2>
    struct _PCC
    {
      template <typename _U1, typename _U2>
      static constexpr bool _ConstructiblePair()
      {
	return __and_<is_constructible<_T1, const _U1&>,
		      is_constructible<_T2, const _U2&>>::value;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyConvertiblePair()
      {
	return __and_<is_convertible<const _U1&, _T1>,
		      is_convertible<const _U2&, _T2>>::value;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _MoveConstructiblePair()
      {
	return __and_<is_constructible<_T1, _U1&&>,
		      is_constructible<_T2, _U2&&>>::value;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyMoveConvertiblePair()
      {
	return __and_<is_convertible<_U1&&, _T1>,
		      is_convertible<_U2&&, _T2>>::value;
      }
    };

  template <typename _T1, typename _T2>
    struct _PCC<false, _T1, _T2>
    {
      template <typename _U1, typename _U2>
      static constexpr bool _ConstructiblePair()
      {
	return false;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyConvertiblePair()
      {
	return false;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _MoveConstructiblePair()
      {
	return false;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyMoveConvertiblePair()
      {
	return false;
      }
    };
#endif // lib concepts
#endif // C++11

#if __glibcxx_tuple_like // >= C++23
  template<typename _Tp>
    inline constexpr bool __is_tuple_v = false;

  template<typename... _Ts>
    inline constexpr bool __is_tuple_v<tuple<_Ts...>> = true;

  // TODO: Reuse __is_tuple_like from <type_traits>?
  template<typename _Tp>
    inline constexpr bool __is_tuple_like_v = false;

  template<typename... _Elements>
    inline constexpr bool __is_tuple_like_v<tuple<_Elements...>> = true;

  template<typename _T1, typename _T2>
    inline constexpr bool __is_tuple_like_v<pair<_T1, _T2>> = true;

  template<typename _Tp, size_t _Nm>
    inline constexpr bool __is_tuple_like_v<array<_Tp, _Nm>> = true;

  // __is_tuple_like_v<subrange> is defined in <bits/ranges_util.h>.

  template<typename _Tp>
    concept __tuple_like = __is_tuple_like_v<remove_cvref_t<_Tp>>;

  template<typename _Tp>
    concept __pair_like = __tuple_like<_Tp> && tuple_size_v<remove_cvref_t<_Tp>> == 2;

  template<typename _Tp, typename _Tuple>
    concept __eligible_tuple_like
      = __detail::__different_from<_Tp, _Tuple> && __tuple_like<_Tp>
	&& (tuple_size_v<remove_cvref_t<_Tp>> == tuple_size_v<_Tuple>)
	&& !ranges::__detail::__is_subrange<remove_cvref_t<_Tp>>;

  template<typename _Tp, typename _Pair>
    concept __eligible_pair_like
      = __detail::__different_from<_Tp, _Pair> && __pair_like<_Tp>
	&& !ranges::__detail::__is_subrange<remove_cvref_t<_Tp>>;
#endif // C++23

  template<typename _U1, typename _U2> class __pair_base
  {
#if __cplusplus >= 201103L && ! __cpp_lib_concepts
    template<typename _T1, typename _T2> friend struct pair;
    __pair_base() = default;
    ~__pair_base() = default;
    __pair_base(const __pair_base&) = default;
    __pair_base& operator=(const __pair_base&) = delete;
#endif // C++11
  };

  /// @relates pair @{

#if __cpp_deduction_guides >= 201606
  template<typename _T1, typename _T2> pair(_T1, _T2) -> pair<_T1, _T2>;
#endif

#if __cplusplus >= 201103L
  /** Swap overload for pairs. Calls std::pair::swap().
   *
   * @note This std::swap overload is not declared in C++03 mode,
   * which has performance implications, e.g. see https://gcc.gnu.org/PR38466
  */
  template<typename _T1, typename _T2>
    _GLIBCXX20_CONSTEXPR inline
#if __cplusplus > 201402L || !defined(__STRICT_ANSI__) // c++1z or gnu++11
    // Constrained free swap overload, see p0185r1
    typename enable_if<__and_<__is_swappable<_T1>,
                              __is_swappable<_T2>>::value>::type
#else
    void
#endif
    swap(pair<_T1, _T2>& __x, pair<_T1, _T2>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }

#if __glibcxx_ranges_zip // >= C++23
  template<typename _T1, typename _T2>
    requires is_swappable_v<const _T1> && is_swappable_v<const _T2>
    constexpr void
    swap(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }
#endif // C++23

#if __cplusplus > 201402L || !defined(__STRICT_ANSI__) // c++1z or gnu++11
  template<typename _T1, typename _T2>
    typename enable_if<!__and_<__is_swappable<_T1>,
			       __is_swappable<_T2>>::value>::type
    swap(pair<_T1, _T2>&, pair<_T1, _T2>&) = delete;
#endif
#endif // __cplusplus >= 201103L

  /// @} relates pair

  /**
   *  @brief A convenience wrapper for creating a pair from two objects.
   *  @param  __x  The first object.
   *  @param  __y  The second object.
   *  @return   A newly-constructed pair<> object of the appropriate type.
   *
   *  The C++98 standard says the objects are passed by reference-to-const,
   *  but C++03 says they are passed by value (this was LWG issue #181).
   *
   *  Since C++11 they have been passed by forwarding reference and then
   *  forwarded to the new members of the pair. To create a pair with a
   *  member of reference type, pass a `reference_wrapper` to this function.
   */
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 181.  make_pair() unintended behavior
#if __cplusplus >= 201103L
  // NB: DR 706.
  template<typename _T1, typename _T2>
    constexpr pair<typename __decay_and_strip<_T1>::__type,
                   typename __decay_and_strip<_T2>::__type>
    make_pair(_T1&& __x, _T2&& __y)
    {
      typedef typename __decay_and_strip<_T1>::__type __ds_type1;
      typedef typename __decay_and_strip<_T2>::__type __ds_type2;
      typedef pair<__ds_type1, __ds_type2> 	      __pair_type;
      return __pair_type(std::forward<_T1>(__x), std::forward<_T2>(__y));
    }
#else
  template<typename _T1, typename _T2>
    inline pair<_T1, _T2>
    make_pair(_T1 __x, _T2 __y)
    { return pair<_T1, _T2>(__x, __y); }
#endif

  /// @}

#if __cplusplus >= 201103L
  // Various functions which give std::pair a tuple-like interface.

  /// @cond undocumented
  template<typename _T1, typename _T2>
    struct __is_tuple_like_impl<pair<_T1, _T2>> : true_type
    { };
  /// @endcond

  /// Partial specialization for std::pair
  template<class _Tp1, class _Tp2>
    struct tuple_size<pair<_Tp1, _Tp2>>
    : public integral_constant<size_t, 2> { };

  /// Partial specialization for std::pair
  template<class _Tp1, class _Tp2>
    struct tuple_element<0, pair<_Tp1, _Tp2>>
    { typedef _Tp1 type; };

  /// Partial specialization for std::pair
  template<class _Tp1, class _Tp2>
    struct tuple_element<1, pair<_Tp1, _Tp2>>
    { typedef _Tp2 type; };

  // Forward declare the partial specialization for std::tuple
  // to work around modules bug PR c++/113814.
  template<size_t __i, typename... _Types>
    struct tuple_element<__i, tuple<_Types...>>;

#if __cplusplus >= 201703L
  template<typename _Tp1, typename _Tp2>
    inline constexpr size_t tuple_size_v<pair<_Tp1, _Tp2>> = 2;

  template<typename _Tp1, typename _Tp2>
    inline constexpr size_t tuple_size_v<const pair<_Tp1, _Tp2>> = 2;

  template<typename _Tp>
    inline constexpr bool __is_pair = false;

  template<typename _Tp, typename _Up>
    inline constexpr bool __is_pair<pair<_Tp, _Up>> = true;
#endif

  /// @cond undocumented
  template<size_t _Int>
    struct __pair_get;

  template<>
    struct __pair_get<0>
    {
      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp1&
	__get(pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.first; }

      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp1&&
	__move_get(pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<_Tp1>(__pair.first); }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp1&
	__const_get(const pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.first; }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp1&&
	__const_move_get(const pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<const _Tp1>(__pair.first); }
    };

  template<>
    struct __pair_get<1>
    {
      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp2&
	__get(pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.second; }

      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp2&&
	__move_get(pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<_Tp2>(__pair.second); }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp2&
	__const_get(const pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.second; }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp2&&
	__const_move_get(const pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<const _Tp2>(__pair.second); }
    };
  /// @endcond

  /** @{
   * std::get overloads for accessing members of std::pair
   */

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(pair<_Tp1, _Tp2>& __in) noexcept
    { return __pair_get<_Int>::__get(__in); }

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(pair<_Tp1, _Tp2>&& __in) noexcept
    { return __pair_get<_Int>::__move_get(std::move(__in)); }

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(const pair<_Tp1, _Tp2>& __in) noexcept
    { return __pair_get<_Int>::__const_get(__in); }

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(const pair<_Tp1, _Tp2>&& __in) noexcept
    { return __pair_get<_Int>::__const_move_get(std::move(__in)); }


#ifdef __glibcxx_tuples_by_type // C++ >= 14
  template <typename _Tp, typename _Up>
    constexpr _Tp&
    get(pair<_Tp, _Up>& __p) noexcept
    { return __p.first; }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&
    get(const pair<_Tp, _Up>& __p) noexcept
    { return __p.first; }

  template <typename _Tp, typename _Up>
    constexpr _Tp&&
    get(pair<_Tp, _Up>&& __p) noexcept
    { return std::move(__p.first); }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&&
    get(const pair<_Tp, _Up>&& __p) noexcept
    { return std::move(__p.first); }

  template <typename _Tp, typename _Up>
    constexpr _Tp&
    get(pair<_Up, _Tp>& __p) noexcept
    { return __p.second; }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&
    get(const pair<_Up, _Tp>& __p) noexcept
    { return __p.second; }

  template <typename _Tp, typename _Up>
    constexpr _Tp&&
    get(pair<_Up, _Tp>&& __p) noexcept
    { return std::move(__p.second); }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&&
    get(const pair<_Up, _Tp>&& __p) noexcept
    { return std::move(__p.second); }
#endif // __glibcxx_tuples_by_type


#if __glibcxx_ranges_zip // >= C++23
  template<typename _T1, typename _T2, typename _U1, typename _U2,
	   template<typename> class _TQual, template<typename> class _UQual>
    requires requires { typename pair<common_reference_t<_TQual<_T1>, _UQual<_U1>>,
				      common_reference_t<_TQual<_T2>, _UQual<_U2>>>; }
  struct basic_common_reference<pair<_T1, _T2>, pair<_U1, _U2>, _TQual, _UQual>
  {
    using type = pair<common_reference_t<_TQual<_T1>, _UQual<_U1>>,
		      common_reference_t<_TQual<_T2>, _UQual<_U2>>>;
  };

  template<typename _T1, typename _T2, typename _U1, typename _U2>
    requires requires { typename pair<common_type_t<_T1, _U1>, common_type_t<_T2, _U2>>; }
  struct common_type<pair<_T1, _T2>, pair<_U1, _U2>>
  { using type = pair<common_type_t<_T1, _U1>, common_type_t<_T2, _U2>>; };
#endif // C++23

  /// @}
#endif // C++11

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif /* _STL_PAIR_H */
#else
#include_next<bits/stl_pair.h>
#endif