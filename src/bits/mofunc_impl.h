// Implementation of std::move_only_function -*- C++ -*-

// Copyright The GNU Toolchain Authors.
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

/** @file include/bits/mofunc_impl.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */

#ifndef _GLIBCXX_MOF_CV
#define _GLIBCXX_MOF_CV
#endif

#ifdef _GLIBCXX_MOF_REF
#define _GLIBCXX_MOF_INV_QUALS _GLIBCXX_MOF_CV _GLIBCXX_MOF_REF
#else
#define _GLIBCXX_MOF_REF
#define _GLIBCXX_MOF_INV_QUALS _GLIBCXX_MOF_CV &
#endif

#define _GLIBCXX_MOF_CV_REF _GLIBCXX_MOF_CV _GLIBCXX_MOF_REF

namespace std _GLIBCXX_VISIBILITY(default)
{
	_GLIBCXX_BEGIN_NAMESPACE_VERSION
	/**
	 *  @brief Polymorphic function wrapper.
	 *  @ingroup functors
	 *  @since C++23
	 *  @headerfile functional
	 *
	 *  The `std::move_only_function` class template is a call wrapper similar
	 *  to `std::function`, but does not require the stored target function
	 *  to be copyable.
	 *
	 *  It also supports const-qualification, ref-qualification, and
	 *  no-throw guarantees. The qualifications and exception-specification
	 *  of the `move_only_function::operator()` member function are respected
	 *  when invoking the target function.
	 */
	template <typename _Res, typename... _ArgTypes
#if __cplusplus >= 201703L
			  ,
			  bool _Noex
#endif
			  >
	class move_only_function<_Res(_ArgTypes...) _GLIBCXX_MOF_CV
								 _GLIBCXX_MOF_REF
#if __cplusplus >= 201703L
							 noexcept(_Noex)
#endif
							 >
		: _Mofunc_base
	{
#if __cplusplus < 201703L
		static constexpr bool _Noex = false;
#endif
		template <typename _Tp>
		using __callable = __conditional_t<_Noex,
										   is_nothrow_invocable_r<_Res, _Tp, _ArgTypes...>,
										   is_invocable_r<_Res, _Tp, _ArgTypes...>>;

		// [func.wrap.mov.con]/1 is-callable-from<VT>
		template <typename _Vt>
#if __cplusplus < 201402L
		struct __is_callable_from : bool_constant<__and_<__callable<_Vt _GLIBCXX_MOF_CV_REF>,
														 __callable<_Vt _GLIBCXX_MOF_INV_QUALS>>::value>
		{
		};
#else
		static constexpr bool __is_callable_from = __and_v<__callable<_Vt _GLIBCXX_MOF_CV_REF>,
														   __callable<_Vt _GLIBCXX_MOF_INV_QUALS>>;
#endif
		template <typename... T>
		static bool _ValidAndNull(const T &...)
		{
			return false;
		}
		template <typename T>
		static auto _ValidAndNull(const T &__f) -> decltype(__f == nullptr, bool())
		{
			return __f == nullptr;
		}

	public:
		using result_type = _Res;

		/// Creates an empty object.
		move_only_function() noexcept {}

		/// Creates an empty object.
		move_only_function(nullptr_t) noexcept {}

		/// Moves the target object, leaving the source empty.
		move_only_function(move_only_function &&__x) noexcept
			: _Mofunc_base(static_cast<_Mofunc_base &&>(__x)),
			  _M_invoke(std::__exchange(__x._M_invoke, nullptr))
		{
		}

		/// Stores a target object initialized from the argument.
		template <typename _Fn, typename _Vt = decay_t<_Fn>, typename = enable_if_t<(!_CSL_Struct14Value_V(is_same, _Vt, move_only_function)) && (!_CSL_Struct14Value_V(__is_in_place_type, _Vt)) && _CSL_Struct14Value(__is_callable_from, _Vt)>>
		move_only_function(_Fn &&__f) noexcept(_S_nothrow_init<_Vt, _Fn>())
		{
			if _GLIBCXX14_CONSTEXPR (_CSL_Struct14Value_V(is_function, remove_pointer_t<_Vt>) || _CSL_Struct14Value_V(is_member_pointer, _Vt) || _CSL_Struct14Value_V(__is_move_only_function, _Vt))
			{
				if (_ValidAndNull(__f))
					return;
			}
			_M_init<_Vt>(std::forward<_Fn>(__f));
			_M_invoke = &_S_invoke<_Vt>;
		}

		/// Stores a target object initialized from the arguments.
		template <typename _Tp, typename... _Args>
		explicit move_only_function(in_place_type_t<_Tp>, _Args &&...__args, enable_if_t<_CSL_Struct14Value_V(is_constructible, _Tp, _Args...) && _CSL_Struct14Value(__is_callable_from, _Tp), int> = 0) noexcept(_S_nothrow_init<_Tp, _Args...>())
			: _M_invoke(&_S_invoke<_Tp>)
		{
			_GLIBCXX17_STATIC_ASSERT(_CSL_Struct14Value_V(is_same, decay_t<_Tp>, _Tp));
			_M_init<_Tp>(std::forward<_Args>(__args)...);
		}

		/// Stores a target object initialized from the arguments.
		template <typename _Tp, typename _Up, typename... _Args>
		explicit move_only_function(in_place_type_t<_Tp>, initializer_list<_Up> __il,
									_Args &&...__args, enable_if_t<_CSL_Struct14Value_V(is_constructible, _Tp, initializer_list<_Up> &, _Args...) && _CSL_Struct14Value(__is_callable_from, _Tp), int> = 0) noexcept(_S_nothrow_init<_Tp, initializer_list<_Up> &, _Args...>())
			: _M_invoke(&_S_invoke<_Tp>)
		{
			_GLIBCXX17_STATIC_ASSERT(_CSL_Struct14Value_V(is_same, decay_t<_Tp>, _Tp));
			_M_init<_Tp>(__il, std::forward<_Args>(__args)...);
		}

		/// Stores a new target object, leaving `x` empty.
		move_only_function &
		operator=(move_only_function &&__x) noexcept
		{
			_Mofunc_base::operator=(static_cast<_Mofunc_base &&>(__x));
			_M_invoke = std::__exchange(__x._M_invoke, nullptr);
			return *this;
		}

		/// Destroys the target object (if any).
		move_only_function &
		operator=(nullptr_t) noexcept
		{
			_Mofunc_base::operator=(nullptr);
			_M_invoke = nullptr;
			return *this;
		}

		/// Stores a new target object, initialized from the argument.
		template <typename _Fn>
		enable_if_t<_CSL_Struct14Value_V(is_constructible, move_only_function, _Fn), move_only_function &>
		operator=(_Fn &&__f) noexcept(_CSL_Struct14Value_V(is_nothrow_constructible, move_only_function, _Fn))
		{
			move_only_function(std::forward<_Fn>(__f)).swap(*this);
			return *this;
		}

		~move_only_function() = default;

		/// True if a target object is present, false otherwise.
		explicit operator bool() const noexcept { return _M_invoke != nullptr; }

		/** Invoke the target object.
		 *
		 * The target object will be invoked using the supplied arguments,
		 * and as an lvalue or rvalue, and as const or non-const, as dictated
		 * by the template arguments of the `move_only_function` specialization.
		 *
		 * @pre Must not be empty.
		 */
		_Res
		operator()(_ArgTypes... __args) _GLIBCXX_MOF_CV_REF noexcept(_Noex)
		{
			__glibcxx_assert(*this != nullptr);
			return _M_invoke(this, std::forward<_ArgTypes>(__args)...);
		}

		/// Exchange the target objects (if any).
		void
		swap(move_only_function &__x) noexcept
		{
			_Mofunc_base::swap(__x);
			std::swap(_M_invoke, __x._M_invoke);
		}

		/// Exchange the target objects (if any).
		friend void
		swap(move_only_function &__x, move_only_function &__y) noexcept
		{
			__x.swap(__y);
		}

		/// Check for emptiness by comparing with `nullptr`.
		friend bool
		operator==(const move_only_function &__x, nullptr_t) noexcept
		{
			return __x._M_invoke == nullptr;
		}
#ifndef __cpp_impl_three_way_comparison
		// C++20之前，只定义operator==不会自动生成operator!=，需要手动定义
		friend bool
		operator!=(const move_only_function &__x, nullptr_t) noexcept
		{
			return __x._M_invoke != nullptr;
		}
#endif

	private:
		template <typename _Tp>
		using __param_t = __conditional_t<_CSL_Struct14Value_V(is_scalar, _Tp), _Tp, _Tp &&>;

		using _Invoker = _Res (*)(_Mofunc_base _GLIBCXX_MOF_CV *,
								  __param_t<_ArgTypes>...) noexcept(_Noex);

		template <typename _Tp>
		static _Res
		_S_invoke(_Mofunc_base _GLIBCXX_MOF_CV *__self,
				  __param_t<_ArgTypes>... __args) noexcept(_Noex)
		{
			using _TpCv = _Tp _GLIBCXX_MOF_CV;
			using _TpInv = _Tp _GLIBCXX_MOF_INV_QUALS;
			return std::__invoke_r<_Res>(
				std::forward<_TpInv>(*_S_access<_TpCv>(__self)),
				std::forward<__param_t<_ArgTypes>>(__args)...);
		}

		_Invoker _M_invoke = nullptr;
	};

#undef _GLIBCXX_MOF_CV_REF
#undef _GLIBCXX_MOF_CV
#undef _GLIBCXX_MOF_REF
#undef _GLIBCXX_MOF_INV_QUALS

	_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std