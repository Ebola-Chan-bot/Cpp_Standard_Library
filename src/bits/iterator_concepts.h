#pragma once
#ifdef ARDUINO_ARCH_ESP32
#include_next <bits/iterator_concepts.h>
#else
// Concepts and traits for use with iterators -*- C++ -*-

// Copyright (C) 2019-2024 Free Software Foundation, Inc.
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

/** @file bits/iterator_concepts.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 */

#ifndef _ITERATOR_CONCEPTS_H
#define _ITERATOR_CONCEPTS_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <concepts>
#include <bits/ptr_traits.h> // to_address

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic" // __int128

namespace std _GLIBCXX_VISIBILITY(default)
{
	_GLIBCXX_BEGIN_NAMESPACE_VERSION

	/** A sentinel type that can be used to check for the end of a range.
	 *
	 * For some iterator types the past-the-end sentinel value is independent
	 * of the underlying sequence, and a default sentinel can be used with them.
	 * For example, a `std::counted_iterator` keeps a count of how many elements
	 * remain, and so checking for the past-the-end value only requires checking
	 * if that count has reached zero. A past-the-end `std::istream_iterator` is
	 * equal to the default-constructed value, which can be easily checked.
	 *
	 * Comparing iterators of these types to `std::default_sentinel` is a
	 * convenient way to check if the end has been reached.
	 *
	 * @since C++20
	 */
	struct default_sentinel_t
	{
	};

	/// A default sentinel value.
	inline constexpr default_sentinel_t default_sentinel{};

#if __cpp_lib_concepts
	struct input_iterator_tag;
	struct output_iterator_tag;
	struct forward_iterator_tag;
	struct bidirectional_iterator_tag;
	struct random_access_iterator_tag;
	struct contiguous_iterator_tag;

	template <typename _Iterator>
	struct iterator_traits;

	template <typename _Iterator, typename>
	struct __iterator_traits;

	namespace __detail
	{
		template <typename _Tp>
		using __with_ref = _Tp &;

		template <typename _Tp>
		concept __can_reference = is_same_v<void_t<__with_ref<_Tp>>, void>;

		template <typename _Tp>
		concept __dereferenceable = is_same_v<decltype(*declval<_Tp &>()), __can_reference>;
	} // namespace __detail

	template <typename _Tp, typename = enable_if_t<__detail::__dereferenceable<_Tp>>>
	using iter_reference_t = decltype(*std::declval<_Tp &>());

	namespace ranges
	{
		/// @cond undocumented
		namespace __imove
		{
			void iter_move() = delete;

			struct _IterMove
			{
			private:
				template <typename _Tp>
				struct __result
				{
					using type = iter_reference_t<_Tp>;
				};

			public:
			};
		} // namespace __imove
		/// @endcond

		inline namespace _Cpo
		{
			inline constexpr __imove::_IterMove iter_move{};
		}
	} // namespace ranges

	template <typename>
	struct incrementable_traits
	{
	};

	template <typename _Iter>
	struct incrementable_traits<const _Iter>
		: incrementable_traits<_Iter>
	{
	};

#if defined __STRICT_ANSI__ && defined __SIZEOF_INT128__
	// __int128 is incrementable even if !integral<__int128>
	template <>
	struct incrementable_traits<__int128>
	{
		using difference_type = __int128;
	};

	template <>
	struct incrementable_traits<unsigned __int128>
	{
		using difference_type = __int128;
	};
#endif

	namespace __detail
	{
		// An iterator such that iterator_traits<_Iter> names a specialization
		// generated from the primary template.
		template <typename _Iter>
		concept __primary_traits_iter = __is_base_of(__iterator_traits<_Iter, void>, iterator_traits<_Iter>);

		template <typename _Iter, typename _Tp>
		struct __iter_traits_impl
		{
			using type = iterator_traits<_Iter>;
		};

		// ITER_TRAITS
		template <typename _Iter, typename _Tp = _Iter>
		using __iter_traits = typename __iter_traits_impl<_Iter, _Tp>::type;

		template <typename _Tp>
		using __iter_diff_t = typename __iter_traits<_Tp, incrementable_traits<_Tp>>::difference_type;
	} // namespace __detail

	template <typename _Tp>
	using iter_difference_t = __detail::__iter_diff_t<remove_cvref_t<_Tp>>;

	namespace __detail
	{
		template <typename>
		struct __cond_value_type
		{
		};

	} // namespace __detail

	template <typename>
	struct indirectly_readable_traits
	{
	};

	template <typename _Tp>
	struct indirectly_readable_traits<_Tp *>
		: __detail::__cond_value_type<_Tp>
	{
	};

	template <typename _Iter>
	struct indirectly_readable_traits<const _Iter>
		: indirectly_readable_traits<_Iter>
	{
	};

	namespace __detail
	{
		template <typename _Tp>
		using __iter_value_t = typename __iter_traits<_Tp, indirectly_readable_traits<_Tp>>::value_type;
	} // namespace __detail

	template <typename _Tp>
	using iter_value_t = __detail::__iter_value_t<remove_cvref_t<_Tp>>;

	namespace __detail
	{
		template <typename _Iter>
		struct __iter_concept_impl;

		// Otherwise, there is no ITER_CONCEPT(I) type.
		template <typename _Iter>
		struct __iter_concept_impl
		{
		};

		// ITER_CONCEPT
		template <typename _Iter>
		using __iter_concept = typename __iter_concept_impl<_Iter>::type;

	} // namespace __detail

	namespace __detail
	{
		template <typename _Tp>
		struct __indirect_value
		{
			using type = iter_value_t<_Tp> &;
		};

		// __indirect_value<projected<_Iter, _Proj>> is defined later.
	} // namespace __detail

	template <typename _Tp>
	using __indirect_value_t = typename __detail::__indirect_value<_Tp>::type;

	namespace ranges::__detail
	{
		class __max_diff_type;
		class __max_size_type;

		__extension__ template <typename _Tp>
		concept __is_signed_int128
#if __SIZEOF_INT128__
			= same_as<_Tp, __int128>;
#else
			= false;
#endif

		__extension__ template <typename _Tp>
		concept __is_unsigned_int128
#if __SIZEOF_INT128__
			= same_as<_Tp, unsigned __int128>;
#else
			= false;
#endif

		template <typename _Tp>
		concept __cv_bool = same_as<const volatile _Tp, const volatile bool>;

		template <typename _Tp>
		concept __integral_nonbool = integral<_Tp> && !__cv_bool<_Tp>;

		template <typename _Tp>
		concept __is_int128 = __is_signed_int128<_Tp> || __is_unsigned_int128<_Tp>;

		template <typename _Tp>
		concept __is_integer_like = __integral_nonbool<_Tp> || __is_int128<_Tp> || same_as<_Tp, __max_diff_type> || same_as<_Tp, __max_size_type>;

		template <typename _Tp>
		concept __is_signed_integer_like = signed_integral<_Tp> || __is_signed_int128<_Tp> || same_as<_Tp, __max_diff_type>;

	} // namespace ranges::__detail

	namespace __detail
	{
		using ranges::__detail::__is_signed_integer_like;
	}
	// 633
	/// Requirements on types that can be incremented with ++.
	template <typename _Iter>
	concept weakly_incrementable = movable<_Iter> && requires(_Iter __i) {
		typename iter_difference_t<_Iter>;
		requires __detail::__is_signed_integer_like<iter_difference_t<_Iter>>;
		{ ++__i } -> same_as<_Iter &>;
		__i++;
	};

	template <typename _Iter>
	concept incrementable = regular<_Iter> && weakly_incrementable<_Iter> && requires(_Iter __i) { { __i++ } -> same_as<_Iter>; };

	template <typename _Iter>
	concept input_or_output_iterator = requires(_Iter __i) { { *__i } -> __detail::__can_reference; } && weakly_incrementable<_Iter>;
	// 653
	template <typename _Sent, typename _Iter>
	inline constexpr bool disable_sized_sentinel_for = false;
	// 670
	template <typename _Iter>
	concept input_iterator = input_or_output_iterator<_Iter> && indirectly_readable<_Iter> && requires { typename __detail::__iter_concept<_Iter>; } && derived_from<__detail::__iter_concept<_Iter>, input_iterator_tag>;

	template <typename _Iter, typename _Tp>
	concept output_iterator = input_or_output_iterator<_Iter> && indirectly_writable<_Iter, _Tp> && requires(_Iter __i, _Tp &&__t) { *__i++ = std::forward<_Tp>(__t); };

	template <typename _Iter>
	concept forward_iterator = input_iterator<_Iter> && derived_from<__detail::__iter_concept<_Iter>, forward_iterator_tag> && incrementable<_Iter> && sentinel_for<_Iter, _Iter>;

	template <typename _Iter>
	concept bidirectional_iterator = forward_iterator<_Iter> && derived_from<__detail::__iter_concept<_Iter>, bidirectional_iterator_tag>
#define __i std::declval<_Iter>()
									 && same_as<_Iter &, decltype(--__i)> && same_as<_Iter, decltype(__i--)>;

	template <typename _Iter>
	concept random_access_iterator = bidirectional_iterator<_Iter> && derived_from<__detail::__iter_concept<_Iter>, random_access_iterator_tag> && totally_ordered<_Iter> && sized_sentinel_for<_Iter, _Iter>
#define __i std::declval<_Iter>()
#define __j std::declval<const _Iter>()
#define __n std::declval<iter_difference_t<_Iter>>()
									 && same_as<_Iter &, decltype(__i += __n)> && same_as<_Iter, decltype(__j + __n)> && same_as<_Iter, decltype(__n + __j)> && same_as<_Iter &, decltype(__i -= __n)> && same_as<_Iter, decltype(__j - __n)> && same_as<iter_reference_t<_Iter>, decltype(__j[__n])>;
#undef __i
#undef __j
#undef __n

	template <typename _Iter>
	concept contiguous_iterator = random_access_iterator<_Iter> && derived_from<__detail::__iter_concept<_Iter>, contiguous_iterator_tag> && is_lvalue_reference_v<iter_reference_t<_Iter>> && same_as<iter_value_t<_Iter>, remove_cvref_t<iter_reference_t<_Iter>>> && same_as<decltype(std::to_address(std::declval<const _Iter &>())), add_pointer_t<iter_reference_t<_Iter>>>;
	// 723

	namespace __detail
	{
		template <typename _Iter, typename _Proj>
		struct __projected
		{
			struct __type
			{

				// These are used to identify and obtain the template arguments of a
				// specialization of the 'projected' alias template below.
				using __projected_Iter = _Iter;
				using __projected_Proj = _Proj;
			};
		};

		// Optimize the common case of the projection being std::identity.
		template <typename _Iter>
		struct __projected<_Iter, identity>
		{
			using __type = _Iter;
		};
	} // namespace __detail

	namespace ranges
	{
		/// @cond undocumented
		namespace __iswap
		{
			template <typename _It1, typename _It2>
			void iter_swap(_It1, _It2) = delete;

			template <typename _Xp, typename _Yp>
			constexpr iter_value_t<_Xp>
			__iter_exchange_move(_Xp &&__x, _Yp &&__y) noexcept(noexcept(iter_value_t<_Xp>(iter_move(__x))) && noexcept(*__x = iter_move(__y)))
			{
				iter_value_t<_Xp> __old_value(iter_move(__x));
				*__x = iter_move(__y);
				return __old_value;
			}

			struct _IterSwap
			{
			private:
			public:
			};
		} // namespace __iswap
		/// @endcond

		inline namespace _Cpo
		{
			inline constexpr __iswap::_IterSwap iter_swap{};
		}

	} // namespace ranges

	/// [alg.req.ind.cmp], concept `indirectly_comparable`
	template <typename _I1, typename _I2, typename _Rel, typename _P1 = identity,
			  typename _P2 = identity>
	concept indirectly_comparable = indirect_binary_predicate<_Rel, projected<_I1, _P1>,
															  projected<_I2, _P2>>;

	/// [alg.req.permutable], concept `permutable`
	template <typename _Iter>
	concept permutable = forward_iterator<_Iter> && indirectly_movable_storable<_Iter, _Iter> && indirectly_swappable<_Iter, _Iter>;

	/// [alg.req.sortable], concept `sortable`
	template <typename _Iter, typename _Rel = ranges::less,
			  typename _Proj = identity>
	concept sortable = permutable<_Iter> && indirect_strict_weak_order<_Rel, projected<_Iter, _Proj>>;

	struct unreachable_sentinel_t
	{
	};

	inline constexpr unreachable_sentinel_t unreachable_sentinel{};

	// This is the namespace for [range.access] CPOs.
	namespace ranges::__access
	{
		using std::__detail::__class_or_enum;

		struct _Decay_copy final
		{
			template <typename _Tp>
			constexpr decay_t<_Tp>
			operator()(_Tp &&__t) const
				noexcept(is_nothrow_convertible_v<_Tp, decay_t<_Tp>>)
			{
				return std::forward<_Tp>(__t);
			}
		} inline constexpr __decay_copy{};

		// Poison pill so that unqualified lookup doesn't find std::begin.
		void begin() = delete;
	} // namespace ranges::__access

	namespace __detail
	{
		// Implementation of std::ranges::iterator_t, without using ranges::begin.
		template <typename _Tp>
		using __range_iter_t = decltype(ranges::__access::__begin(std::declval<_Tp &>()));

	} // namespace __detail
#endif // C++20 library concepts
	_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std
#pragma GCC diagnostic pop
#endif // _ITERATOR_CONCEPTS_H
#endif