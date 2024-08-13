#pragma once
#ifdef ARDUINO_ARCH_AVR
// Deque implementation -*- C++ -*-

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
 * Copyright (c) 1997
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

/** @file bits/stl_deque.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{deque}
 */

#ifndef _STL_DEQUE_H
#define _STL_DEQUE_H 1

#include <bits/ptr_traits.h>
#include <ext/alloc_traits.h>
#include <exception_defines.h>
#if __cplusplus >= 201103L
#include <initializer_list>
#include <bits/stl_uninitialized.h> // for __is_bitwise_relocatable
#endif
#if __cplusplus > 201703L
#include <compare>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
	_GLIBCXX_BEGIN_NAMESPACE_VERSION
	_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

	/**
	 *  @brief This function controls the size of memory nodes.
	 *  @param  __size  The size of an element.
	 *  @return   The number (not byte size) of elements per node.
	 *
	 *  This function started off as a compiler kludge from SGI, but
	 *  seems to be a useful wrapper around a repeated constant
	 *  expression.  The @b 512 is tunable (and no other code needs to
	 *  change), but no investigation has been done since inheriting the
	 *  SGI code.  Touch _GLIBCXX_DEQUE_BUF_SIZE only if you know what
	 *  you are doing, however: changing it breaks the binary
	 *  compatibility!!
	 */

#ifndef _GLIBCXX_DEQUE_BUF_SIZE
#define _GLIBCXX_DEQUE_BUF_SIZE 512
#endif

	_GLIBCXX_CONSTEXPR inline size_t
	__deque_buf_size(size_t __size)
	{
		return (__size < _GLIBCXX_DEQUE_BUF_SIZE
					? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size)
					: size_t(1));
	}

	/**
	 *  @brief A deque::iterator.
	 *
	 *  Quite a bit of intelligence here.  Much of the functionality of
	 *  deque is actually passed off to this class.  A deque holds two
	 *  of these internally, marking its valid range.  Access to
	 *  elements is done as offsets of either of those two, relying on
	 *  operator overloading in this class.
	 *
	 *  All the functions are op overloads except for _M_set_node.
	 */
	template <typename _Tp, typename _Ref, typename _Ptr>
	struct _Deque_iterator
	{
#if __cplusplus < 201103L
		typedef _Deque_iterator<_Tp, _Tp &, _Tp *> iterator;
		typedef _Deque_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;
		typedef _Tp *_Elt_pointer;
		typedef _Tp **_Map_pointer;
#else
	private:
		template <typename _CvTp>
		using __iter = _Deque_iterator<_Tp, _CvTp &, __ptr_rebind<_Ptr, _CvTp>>;

	public:
		typedef __iter<_Tp> iterator;
		typedef __iter<const _Tp> const_iterator;
		typedef __ptr_rebind<_Ptr, _Tp> _Elt_pointer;
		typedef __ptr_rebind<_Ptr, _Elt_pointer> _Map_pointer;
#endif

		static size_t _S_buffer_size() _GLIBCXX_NOEXCEPT
		{
			return __deque_buf_size(sizeof(_Tp));
		}

		typedef std::random_access_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef _Ptr pointer;
		typedef _Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef _Deque_iterator _Self;

		_Elt_pointer _M_cur;
		_Elt_pointer _M_first;
		_Elt_pointer _M_last;
		_Map_pointer _M_node;

		_Deque_iterator(_Elt_pointer __x, _Map_pointer __y) _GLIBCXX_NOEXCEPT
			: _M_cur(__x),
			  _M_first(*__y),
			  _M_last(*__y + _S_buffer_size()),
			  _M_node(__y) {}

		_Deque_iterator() _GLIBCXX_NOEXCEPT
			: _M_cur(),
			  _M_first(),
			  _M_last(),
			  _M_node() {}

#if __cplusplus < 201103L
		// Conversion from iterator to const_iterator.
		_Deque_iterator(const iterator &__x) _GLIBCXX_NOEXCEPT
			: _M_cur(__x._M_cur),
			  _M_first(__x._M_first),
			  _M_last(__x._M_last),
			  _M_node(__x._M_node) {}
#else
		// Conversion from iterator to const_iterator.
		template <typename _Iter,
				  typename = _Require<is_same<_Self, const_iterator>,
									  is_same<_Iter, iterator>>>
		_Deque_iterator(const _Iter &__x) noexcept
			: _M_cur(__x._M_cur), _M_first(__x._M_first),
			  _M_last(__x._M_last), _M_node(__x._M_node) {}

		_Deque_iterator(const _Deque_iterator &__x) noexcept
			: _M_cur(__x._M_cur), _M_first(__x._M_first),
			  _M_last(__x._M_last), _M_node(__x._M_node) {}

		_Deque_iterator &operator=(const _Deque_iterator &) = default;
#endif

		iterator
		_M_const_cast() const _GLIBCXX_NOEXCEPT
		{
			return iterator(_M_cur, _M_node);
		}

		_GLIBCXX_NODISCARD
		reference
		operator*() const _GLIBCXX_NOEXCEPT
		{
			return *_M_cur;
		}

		_GLIBCXX_NODISCARD
		pointer
		operator->() const _GLIBCXX_NOEXCEPT
		{
			return _M_cur;
		}

		_Self &
		operator++() _GLIBCXX_NOEXCEPT
		{
			++_M_cur;
			if (_M_cur == _M_last)
			{
				_M_set_node(_M_node + 1);
				_M_cur = _M_first;
			}
			return *this;
		}

		_Self
		operator++(int) _GLIBCXX_NOEXCEPT
		{
			_Self __tmp = *this;
			++*this;
			return __tmp;
		}

		_Self &
		operator--() _GLIBCXX_NOEXCEPT
		{
			if (_M_cur == _M_first)
			{
				_M_set_node(_M_node - 1);
				_M_cur = _M_last;
			}
			--_M_cur;
			return *this;
		}

		_Self
		operator--(int) _GLIBCXX_NOEXCEPT
		{
			_Self __tmp = *this;
			--*this;
			return __tmp;
		}

		_Self &
		operator+=(difference_type __n) _GLIBCXX_NOEXCEPT
		{
			const difference_type __offset = __n + (_M_cur - _M_first);
			if (__offset >= 0 && __offset < difference_type(_S_buffer_size()))
				_M_cur += __n;
			else
			{
				const difference_type __node_offset =
					__offset > 0 ? __offset / difference_type(_S_buffer_size())
								 : -difference_type((-__offset - 1) / _S_buffer_size()) - 1;
				_M_set_node(_M_node + __node_offset);
				_M_cur = _M_first + (__offset - __node_offset * difference_type(_S_buffer_size()));
			}
			return *this;
		}

		_Self &
		operator-=(difference_type __n) _GLIBCXX_NOEXCEPT
		{
			return *this += -__n;
		}

		_GLIBCXX_NODISCARD
		reference
		operator[](difference_type __n) const _GLIBCXX_NOEXCEPT
		{
			return *(*this + __n);
		}

		/**
		 *  Prepares to traverse new_node.  Sets everything except
		 *  _M_cur, which should therefore be set by the caller
		 *  immediately afterwards, based on _M_first and _M_last.
		 */
		void
		_M_set_node(_Map_pointer __new_node) _GLIBCXX_NOEXCEPT
		{
			_M_node = __new_node;
			_M_first = *__new_node;
			_M_last = _M_first + difference_type(_S_buffer_size());
		}

		_GLIBCXX_NODISCARD
		friend bool
		operator==(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return __x._M_cur == __y._M_cur;
		}

		// Note: we also provide overloads whose operands are of the same type in
		// order to avoid ambiguous overload resolution when std::rel_ops
		// operators are in scope (for additional details, see libstdc++/3628)
		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend bool
		operator==(const _Self &__x,
				   const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return __x._M_cur == __y._M_cur;
		}

#if __cpp_lib_three_way_comparison
		[[nodiscard]]
		friend strong_ordering
		operator<=>(const _Self &__x, const _Self &__y) noexcept
		{
			if (const auto __cmp = __x._M_node <=> __y._M_node; __cmp != 0)
				return __cmp;
			return __x._M_cur <=> __y._M_cur;
		}
#else
		_GLIBCXX_NODISCARD
		friend bool
		operator!=(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return !(__x == __y);
		}

		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend bool
		operator!=(const _Self &__x,
				   const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return !(__x == __y);
		}

		_GLIBCXX_NODISCARD
		friend bool
		operator<(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return (__x._M_node == __y._M_node)
					   ? (__x._M_cur < __y._M_cur)
					   : (__x._M_node < __y._M_node);
		}

		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend bool
		operator<(const _Self &__x,
				  const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return (__x._M_node == __y._M_node)
					   ? (__x._M_cur < __y._M_cur)
					   : (__x._M_node < __y._M_node);
		}

		_GLIBCXX_NODISCARD
		friend bool
		operator>(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return __y < __x;
		}

		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend bool
		operator>(const _Self &__x,
				  const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return __y < __x;
		}

		_GLIBCXX_NODISCARD
		friend bool
		operator<=(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return !(__y < __x);
		}

		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend bool
		operator<=(const _Self &__x,
				   const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return !(__y < __x);
		}

		_GLIBCXX_NODISCARD
		friend bool
		operator>=(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return !(__x < __y);
		}

		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend bool
		operator>=(const _Self &__x,
				   const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return !(__x < __y);
		}
#endif // three-way comparison

		_GLIBCXX_NODISCARD
		friend difference_type
		operator-(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
		{
			return difference_type(_S_buffer_size()) * (__x._M_node - __y._M_node - bool(__x._M_node)) + (__x._M_cur - __x._M_first) + (__y._M_last - __y._M_cur);
		}

		// _GLIBCXX_RESOLVE_LIB_DEFECTS
		// According to the resolution of DR179 not only the various comparison
		// operators but also operator- must accept mixed iterator/const_iterator
		// parameters.
		template <typename _RefR, typename _PtrR>
		_GLIBCXX_NODISCARD friend difference_type
		operator-(const _Self &__x,
				  const _Deque_iterator<_Tp, _RefR, _PtrR> &__y)
			_GLIBCXX_NOEXCEPT
		{
			return difference_type(_S_buffer_size()) * (__x._M_node - __y._M_node - bool(__x._M_node)) + (__x._M_cur - __x._M_first) + (__y._M_last - __y._M_cur);
		}

		_GLIBCXX_NODISCARD
		friend _Self
		operator+(const _Self &__x, difference_type __n) _GLIBCXX_NOEXCEPT
		{
			_Self __tmp = __x;
			__tmp += __n;
			return __tmp;
		}

		_GLIBCXX_NODISCARD
		friend _Self
		operator-(const _Self &__x, difference_type __n) _GLIBCXX_NOEXCEPT
		{
			_Self __tmp = __x;
			__tmp -= __n;
			return __tmp;
		}

		_GLIBCXX_NODISCARD
		friend _Self
		operator+(difference_type __n, const _Self &__x) _GLIBCXX_NOEXCEPT
		{
			return __x + __n;
		}
	};

	/**
	 *  Deque base class.  This class provides the unified face for %deque's
	 *  allocation.  This class's constructor and destructor allocate and
	 *  deallocate (but do not initialize) storage.  This makes %exception
	 *  safety easier.
	 *
	 *  Nothing in this class ever constructs or destroys an actual Tp element.
	 *  (Deque handles that itself.)  Only/All memory management is performed
	 *  here.
	 */
	template <typename _Tp, typename _Alloc>
	class _Deque_base
	{
	protected:
		typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other _Tp_alloc_type;
		typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;

#if __cplusplus < 201103L
		typedef _Tp *_Ptr;
		typedef const _Tp *_Ptr_const;
#else
		typedef typename _Alloc_traits::pointer _Ptr;
		typedef typename _Alloc_traits::const_pointer _Ptr_const;
#endif

		typedef typename _Alloc_traits::template rebind<_Ptr>::other
			_Map_alloc_type;
		typedef __gnu_cxx::__alloc_traits<_Map_alloc_type> _Map_alloc_traits;

		typedef _Alloc allocator_type;

		allocator_type
		get_allocator() const _GLIBCXX_NOEXCEPT
		{
			return allocator_type(_M_get_Tp_allocator());
		}

		typedef _Deque_iterator<_Tp, _Tp &, _Ptr> iterator;
		typedef _Deque_iterator<_Tp, const _Tp &, _Ptr_const> const_iterator;

		_Deque_base()
			: _M_impl()
		{
			_M_initialize_map(0);
		}

		_Deque_base(size_t __num_elements)
			: _M_impl()
		{
			_M_initialize_map(__num_elements);
		}

		_Deque_base(const allocator_type &__a, size_t __num_elements)
			: _M_impl(__a)
		{
			_M_initialize_map(__num_elements);
		}

		_Deque_base(const allocator_type &__a)
			: _M_impl(__a)
		{ /* Caller must initialize map. */
		}

#if __cplusplus >= 201103L
		_Deque_base(_Deque_base &&__x)
			: _M_impl(std::move(__x._M_get_Tp_allocator()))
		{
			_M_initialize_map(0);
			if (__x._M_impl._M_map)
				this->_M_impl._M_swap_data(__x._M_impl);
		}

		_Deque_base(_Deque_base &&__x, const allocator_type &__a)
			: _M_impl(std::move(__x._M_impl), _Tp_alloc_type(__a))
		{
			__x._M_initialize_map(0);
		}

		_Deque_base(_Deque_base &&__x, const allocator_type &__a, size_t __n)
			: _M_impl(__a)
		{
			if (__x.get_allocator() == __a)
			{
				if (__x._M_impl._M_map)
				{
					_M_initialize_map(0);
					this->_M_impl._M_swap_data(__x._M_impl);
				}
			}
			else
			{
				_M_initialize_map(__n);
			}
		}
#endif

		~_Deque_base() _GLIBCXX_NOEXCEPT;

		typedef typename iterator::_Map_pointer _Map_pointer;

		struct _Deque_impl_data
		{
			_Map_pointer _M_map;
			size_t _M_map_size;
			iterator _M_start;
			iterator _M_finish;

			_Deque_impl_data() _GLIBCXX_NOEXCEPT
				: _M_map(),
				  _M_map_size(),
				  _M_start(),
				  _M_finish()
			{
			}

#if __cplusplus >= 201103L
			_Deque_impl_data(const _Deque_impl_data &) = default;
			_Deque_impl_data &
			operator=(const _Deque_impl_data &) = default;

			_Deque_impl_data(_Deque_impl_data &&__x) noexcept
				: _Deque_impl_data(__x)
			{
				__x = _Deque_impl_data();
			}
#endif

			void
			_M_swap_data(_Deque_impl_data &__x) _GLIBCXX_NOEXCEPT
			{
				// Do not use std::swap(_M_start, __x._M_start), etc as it loses
				// information used by TBAA.
				std::swap(*this, __x);
			}
		};

		// This struct encapsulates the implementation of the std::deque
		// standard container and at the same time makes use of the EBO
		// for empty allocators.
		struct _Deque_impl
			: public _Tp_alloc_type,
			  public _Deque_impl_data
		{
			_Deque_impl() _GLIBCXX_NOEXCEPT_IF(
				is_nothrow_default_constructible<_Tp_alloc_type>::value)
				: _Tp_alloc_type()
			{
			}

			_Deque_impl(const _Tp_alloc_type &__a) _GLIBCXX_NOEXCEPT
				: _Tp_alloc_type(__a)
			{
			}

#if __cplusplus >= 201103L
			_Deque_impl(_Deque_impl &&) = default;

			_Deque_impl(_Tp_alloc_type &&__a) noexcept
				: _Tp_alloc_type(std::move(__a))
			{
			}

			_Deque_impl(_Deque_impl &&__d, _Tp_alloc_type &&__a)
				: _Tp_alloc_type(std::move(__a)), _Deque_impl_data(std::move(__d))
			{
			}
#endif
		};

		_Tp_alloc_type &
		_M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
		{
			return this->_M_impl;
		}

		const _Tp_alloc_type &
		_M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT
		{
			return this->_M_impl;
		}

		_Map_alloc_type
		_M_get_map_allocator() const _GLIBCXX_NOEXCEPT
		{
			return _Map_alloc_type(_M_get_Tp_allocator());
		}

		_Ptr
		_M_allocate_node()
		{
			typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Traits;
			return _Traits::allocate(_M_impl, __deque_buf_size(sizeof(_Tp)));
		}

		void
		_M_deallocate_node(_Ptr __p) _GLIBCXX_NOEXCEPT
		{
			typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Traits;
			_Traits::deallocate(_M_impl, __p, __deque_buf_size(sizeof(_Tp)));
		}

		_Map_pointer
		_M_allocate_map(size_t __n)
		{
			_Map_alloc_type __map_alloc = _M_get_map_allocator();
			return _Map_alloc_traits::allocate(__map_alloc, __n);
		}

		void
		_M_deallocate_map(_Map_pointer __p, size_t __n) _GLIBCXX_NOEXCEPT
		{
			_Map_alloc_type __map_alloc = _M_get_map_allocator();
			_Map_alloc_traits::deallocate(__map_alloc, __p, __n);
		}

		void _M_initialize_map(size_t);
		void _M_create_nodes(_Map_pointer __nstart, _Map_pointer __nfinish);
		void _M_destroy_nodes(_Map_pointer __nstart,
							  _Map_pointer __nfinish) _GLIBCXX_NOEXCEPT;
		enum
		{
			_S_initial_map_size = 8
		};

		_Deque_impl _M_impl;
	};

	template <typename _Tp, typename _Alloc>
	_Deque_base<_Tp, _Alloc>::
		~_Deque_base() _GLIBCXX_NOEXCEPT
	{
		if (this->_M_impl._M_map)
		{
			_M_destroy_nodes(this->_M_impl._M_start._M_node,
							 this->_M_impl._M_finish._M_node + 1);
			_M_deallocate_map(this->_M_impl._M_map, this->_M_impl._M_map_size);
		}
	}

	/**
	 *  @brief Layout storage.
	 *  @param  __num_elements  The count of T's for which to allocate space
	 *                          at first.
	 *  @return   Nothing.
	 *
	 *  The initial underlying memory layout is a bit complicated...
	 */
	template <typename _Tp, typename _Alloc>
	void
	_Deque_base<_Tp, _Alloc>::
		_M_initialize_map(size_t __num_elements)
	{
		const size_t __num_nodes = (__num_elements / __deque_buf_size(sizeof(_Tp)) + 1);

		this->_M_impl._M_map_size = std::max((size_t)_S_initial_map_size,
											 size_t(__num_nodes + 2));
		this->_M_impl._M_map = _M_allocate_map(this->_M_impl._M_map_size);

		// For "small" maps (needing less than _M_map_size nodes), allocation
		// starts in the middle elements and grows outwards.  So nstart may be
		// the beginning of _M_map, but for small maps it may be as far in as
		// _M_map+3.

		_Map_pointer __nstart = (this->_M_impl._M_map + (this->_M_impl._M_map_size - __num_nodes) / 2);
		_Map_pointer __nfinish = __nstart + __num_nodes;

		__try
		{
			_M_create_nodes(__nstart, __nfinish);
		}
		__catch(...)
		{
			_M_deallocate_map(this->_M_impl._M_map, this->_M_impl._M_map_size);
			this->_M_impl._M_map = _Map_pointer();
			this->_M_impl._M_map_size = 0;
			__throw_exception_again;
		}

		this->_M_impl._M_start._M_set_node(__nstart);
		this->_M_impl._M_finish._M_set_node(__nfinish - 1);
		this->_M_impl._M_start._M_cur = _M_impl._M_start._M_first;
		this->_M_impl._M_finish._M_cur = (this->_M_impl._M_finish._M_first + __num_elements % __deque_buf_size(sizeof(_Tp)));
	}

	template <typename _Tp, typename _Alloc>
	void
	_Deque_base<_Tp, _Alloc>::
		_M_create_nodes(_Map_pointer __nstart, _Map_pointer __nfinish)
	{
		_Map_pointer __cur;
		__try
		{
			for (__cur = __nstart; __cur < __nfinish; ++__cur)
				*__cur = this->_M_allocate_node();
		}
		__catch(...)
		{
			_M_destroy_nodes(__nstart, __cur);
			__throw_exception_again;
		}
	}

	template <typename _Tp, typename _Alloc>
	void
	_Deque_base<_Tp, _Alloc>::
		_M_destroy_nodes(_Map_pointer __nstart,
						 _Map_pointer __nfinish) _GLIBCXX_NOEXCEPT
	{
		for (_Map_pointer __n = __nstart; __n < __nfinish; ++__n)
			_M_deallocate_node(*__n);
	}

	_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif /* _STL_DEQUE_H */
#else
#include_next <bits/stl_deque.h>
#endif