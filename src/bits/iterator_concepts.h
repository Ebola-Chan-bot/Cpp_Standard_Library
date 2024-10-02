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
	_GLIBCXX17_INLINE constexpr default_sentinel_t default_sentinel{};

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
	struct _CSL__iterator_traits; // GCC原文中没有CSL前缀，但是会跟SAM标准库冲突

	namespace __detail
	{
		template <typename _Tp>
		using __with_ref = _Tp &;

		template <typename _Tp>
		_CSL_Concept(__can_reference, is_same_v<void_t<__with_ref<_Tp>>, void>);

		template <typename _Tp>
		_CSL_Concept(__dereferenceable, is_same_v<decltype(*declval<_Tp &>()), __can_reference>);
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

#define _CSL_TemplateConcept(Concept, Type) typename Type, typename = enable_if_t<Concept<Type>>

				// The result type of iter_move(std::declval<_Tp>())
				template <_CSL_TemplateConcept(std::__detail::__dereferenceable, _Tp)>
				using __type = typename __result<_Tp>::type;
			};
		} // namespace __imove
		/// @endcond

		inline namespace _Cpo
		{
			_GLIBCXX17_INLINE constexpr __imove::_IterMove iter_move{};
		}
	} // namespace ranges

	template <typename _Tp, typename = enable_if_t<__detail::__dereferenceable<_Tp> && __detail::__can_reference<ranges::__imove::_IterMove::__type<_Tp &>>>>
	using iter_rvalue_reference_t = ranges::__imove::_IterMove::__type<_Tp &>;

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
		_CSL_Concept(__primary_traits_iter, __is_base_of(_CSL__iterator_traits<_Iter, void>, iterator_traits<_Iter>));

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

	template <typename T>
	_CSL_Concept(_CSL_RequiresTypename, same_as<void, std::void_t<T>>);

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
		// 535
		template <typename _In>
		_CSL_Concept(__indirectly_readable_impl, _CSL_RequiresTypename<iter_value_t<_In>> &&_CSL_RequiresTypename<iter_reference_t<_In>> &&_CSL_RequiresTypename<iter_rvalue_reference_t<_In>> &&same_as<iter_reference_t<const _In>, iter_reference_t<_In>> _CSL_Parentheses11 &&same_as<iter_rvalue_reference_t<const _In>, iter_rvalue_reference_t<_In>> _CSL_Parentheses11 &&common_reference_with<iter_reference_t<_In> &&, iter_value_t<_In> &> _CSL_Parentheses11 &&common_reference_with<iter_reference_t<_In> &&, iter_rvalue_reference_t<_In> &&> _CSL_Parentheses11 &&common_reference_with<iter_rvalue_reference_t<_In> &&, const iter_value_t<_In> &> _CSL_Parentheses11);

	} // namespace __detail

	/// Requirements for types that are readable by applying operator*.
	template <typename _In>
	_CSL_Concept(indirectly_readable, __detail::__indirectly_readable_impl<remove_cvref_t<_In>> _CSL_Parentheses11);
	// 559
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
	// 575
	/// Requirements for writing a value into an iterator's referenced object.
#define __o std::declval<_Out &&>()
#define __t std::declval<_Tp &&>()
	template <typename _Out, typename _Tp>
	_CSL_Concept(indirectly_writable, _CSL_RequiresExpression(*__o = std::forward<_Tp>(__t)) && _CSL_RequiresExpression(*std::forward<_Out>(__o) = std::forward<_Tp>(__t)) && _CSL_RequiresExpression(const_cast<const iter_reference_t<_Out> &&>(*__o) = std::forward<_Tp>(__t)) && _CSL_RequiresExpression(const_cast<const iter_reference_t<_Out> &&>(*std::forward<_Out>(__o)) = std::forward<_Tp>(__t)));
	// 587
	namespace ranges
	{
		namespace __detail
		{
			class __max_diff_type;
			class __max_size_type;

			__extension__ template <typename _Tp>
			constexpr bool __is_signed_int128
#if __SIZEOF_INT128__
				_CSL_Function17Variable(same_as<_Tp, __int128>);
#else
				_CSL_Function17Variable(false);
#endif

			__extension__ template <typename _Tp>
			constexpr bool __is_unsigned_int128
#if __SIZEOF_INT128__
				_CSL_Function17Variable(same_as<_Tp, unsigned __int128>);
#else
				_CSL_Function17Variable(false);
#endif

			template <typename _Tp>
			_CSL_Concept(__cv_bool, same_as<const volatile _Tp, const volatile bool> _CSL_Parentheses11);

			template <typename _Tp>
			_CSL_Concept(__integral_nonbool, integral<_Tp> _CSL_Parentheses11 && !__cv_bool<_Tp> _CSL_Parentheses11);

			template <typename _Tp>
			_CSL_Concept(__is_int128, __is_signed_int128<_Tp> _CSL_Parentheses11 || __is_unsigned_int128<_Tp> _CSL_Parentheses11);

			template <typename _Tp>
			_CSL_Concept(__is_integer_like, __integral_nonbool<_Tp> _CSL_Parentheses11 || __is_int128<_Tp> _CSL_Parentheses11 || same_as<_Tp, __max_diff_type> _CSL_Parentheses11 || same_as<_Tp, __max_size_type> _CSL_Parentheses11);

			template <typename _Tp>
			_CSL_Concept(__is_signed_integer_like, signed_integral<_Tp> _CSL_Parentheses11 || __is_signed_int128<_Tp> _CSL_Parentheses11 || same_as<_Tp, __max_diff_type> _CSL_Parentheses11);

		}
	} // namespace ranges::__detail

	namespace __detail
	{
		using ranges::__detail::__is_signed_integer_like;
	}
	// 633
	/// Requirements on types that can be incremented with ++.
#define __i std::declval<_Iter>()
	template <typename _Iter>
	_CSL_Concept(weakly_incrementable, movable<_Iter> _CSL_Parentheses11 &&_CSL_RequiresTypename<iter_difference_t<_Iter>> &&_CSL_RequiresTypename<__detail::__is_signed_integer_like<iter_difference_t<_Iter>>> &&_CSL_RequiresExpressionType(++__i, _Iter &) && _CSL_RequiresExpression(__i++));

	template <typename _Iter>
	_CSL_Concept(incrementable, regular<_Iter> _CSL_Parentheses11 &&weakly_incrementable<_Iter> _CSL_Parentheses11 &&_CSL_RequiresExpressionType(__i++, _Iter));

	template <typename _Iter>
	_CSL_Concept(input_or_output_iterator, same_as<decltype(*std::declval<_Iter>()), __detail::__can_reference> _CSL_Parentheses11);

	template <typename _Sent, typename _Iter>
	_CSL_Concept(sentinel_for, semiregular<_Sent> _CSL_Parentheses11 &&input_or_output_iterator<_Iter> _CSL_Parentheses11 &&__detail::__weakly_eq_cmp_with<_Sent, _Iter> _CSL_Parentheses11);

	template <typename _Sent, typename _Iter>
	_GLIBCXX17_INLINE _CSL_Concept(disable_sized_sentinel_for, false);

#define __i std::declval<const _Iter &>()
#define __s std::declval<const _Sent &>()
	template <typename _Sent, typename _Iter>
	_CSL_Concept(sized_sentinel_for, sentinel_for<_Sent, _Iter> _CSL_Parentheses11 && !disable_sized_sentinel_for<remove_cv_t<_Sent>, remove_cv_t<_Iter>> _CSL_Parentheses11 && _CSL_RequiresExpressionType(__i - __s, iter_difference_t<_Iter>) && _CSL_RequiresExpressionType(__s - __i, iter_difference_t<_Iter>));

	template <typename _Iter>
	_CSL_Concept(input_iterator, input_or_output_iterator<_Iter> _CSL_Parentheses11 &&indirectly_readable<_Iter> _CSL_Parentheses11 &&same_as<void, std::void_t<__detail::__iter_concept<_Iter>>> _CSL_Parentheses11 &&derived_from<__detail::__iter_concept<_Iter>, input_iterator_tag> _CSL_Parentheses11);

#define __t std::declval<_Tp>()
	template <typename _Iter, typename _Tp>
	_CSL_Concept(output_iterator, input_or_output_iterator<_Iter> _CSL_Parentheses11 &&indirectly_writable<_Iter, _Tp> _CSL_Parentheses11 &&same_as<void, std::void_t<decltype(*__i++ = std::forward<_Tp>(__t))>> _CSL_Parentheses11);

	template <typename _Iter>
	_CSL_Concept(forward_iterator, input_iterator<_Iter> _CSL_Parentheses11 &&derived_from<__detail::__iter_concept<_Iter>, forward_iterator_tag> _CSL_Parentheses11 &&incrementable<_Iter> _CSL_Parentheses11 &&sentinel_for<_Iter, _Iter> _CSL_Parentheses11);

	template <typename _Iter>
	_CSL_Concept(bidirectional_iterator, forward_iterator<_Iter> _CSL_Parentheses11 &&derived_from<__detail::__iter_concept<_Iter>, bidirectional_iterator_tag> _CSL_Parentheses11 &&same_as<_Iter &, decltype(--__i)> _CSL_Parentheses11 &&same_as<_Iter, decltype(__i--)> _CSL_Parentheses11);

#define __j std::declval<const _Iter>()
#define __n std::declval<iter_difference_t<_Iter>>()
	template <typename _Iter>
	_CSL_Concept(random_access_iterator, bidirectional_iterator<_Iter> _CSL_Parentheses11 &&derived_from<__detail::__iter_concept<_Iter>, random_access_iterator_tag> _CSL_Parentheses11 &&totally_ordered<_Iter> _CSL_Parentheses11 &&sized_sentinel_for<_Iter, _Iter> _CSL_Parentheses11 &&same_as<_Iter &, decltype(__i += __n)> _CSL_Parentheses11 &&same_as<_Iter, decltype(__j + __n)> _CSL_Parentheses11 &&same_as<_Iter, decltype(__n + __j)> _CSL_Parentheses11 &&same_as<_Iter &, decltype(__i -= __n)> _CSL_Parentheses11 &&same_as<_Iter, decltype(__j - __n)> _CSL_Parentheses11 &&same_as<iter_reference_t<_Iter>, decltype(__j[__n])> _CSL_Parentheses11);

	template <typename _Iter>
	_CSL_Concept(contiguous_iterator, random_access_iterator<_Iter> _CSL_Parentheses11 &&derived_from<__detail::__iter_concept<_Iter>, contiguous_iterator_tag> _CSL_Parentheses11 &&is_lvalue_reference_v<iter_reference_t<_Iter>> _CSL_Parentheses11 &&same_as<iter_value_t<_Iter>, remove_cvref_t<iter_reference_t<_Iter>>> _CSL_Parentheses11 &&same_as<decltype(std::to_address(std::declval<const _Iter &>())), add_pointer_t<iter_reference_t<_Iter>>> _CSL_Parentheses11);
	// 723
	// 1000
	// This is the namespace for [range.access] CPOs.
	namespace ranges
	{
		namespace __access
		{
			using std::__detail::__class_or_enum;

#undef __t
			struct _Decay_copy final
			{
				template <typename _Tp>
				constexpr decay_t<_Tp>
				operator()(_Tp &&__t) const
					noexcept(is_nothrow_convertible_v<_Tp, decay_t<_Tp>> _CSL_Parentheses11)
				{
					return std::forward<_Tp>(__t);
				}
			} _GLIBCXX17_INLINE constexpr __decay_copy{};

#define __t std::declval<_Tp &>()
			template <typename _Tp>
			_CSL_Concept(__member_begin, input_or_output_iterator<decltype(__decay_copy(__t.begin()))> _CSL_Parentheses11);
#if __cplusplus > 201103L
			// Poison pill so that unqualified lookup doesn't find std::begin.
			void begin() = delete;
#endif
			template <typename _Tp>
			_CSL_Concept(__adl_begin, __class_or_enum<remove_reference_t<_Tp>> _CSL_Parentheses11 &&input_or_output_iterator<decltype(__decay_copy(begin(__t)))> _CSL_Parentheses11);

#undef __t
			// Simplified version of std::ranges::begin that only supports lvalues,
			// for use by __range_iter_t below.
			template <typename _Tp, typename = enable_if_t<is_array_v<_Tp> _CSL_Parentheses11 || __member_begin<_Tp &> _CSL_Parentheses11 || __adl_begin<_Tp &> _CSL_Parentheses11>>
			auto
			__begin(_Tp &__t)
			{
				if _GLIBCXX17_CONSTEXPR (is_array_v<_Tp> _CSL_Parentheses11)
					return __t + 0;
				else if _GLIBCXX17_CONSTEXPR (__member_begin<_Tp &> _CSL_Parentheses11)
					return __t.begin();
				else
					return begin(__t);
			}
		}
	} // namespace ranges::__access

	namespace __detail
	{
		// Implementation of std::ranges::iterator_t, without using ranges::begin.
		template <typename _Tp>
		using __range_iter_t = decltype(ranges::__access::__begin(std::declval<_Tp &>()));

	} // namespace __detail
#undef __i
#undef __j
#undef __n
#undef __o
#undef __s
#endif // C++20 library concepts
	_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std
#pragma GCC diagnostic pop
#endif // _ITERATOR_CONCEPTS_H
#endif