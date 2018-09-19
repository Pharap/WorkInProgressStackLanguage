#pragma once

//
//   Copyright (C) 2018 Pharap (@Pharap)
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

#if defined(ARDUINO)
namespace std
{
	template< typename T, T v > struct integral_constant
	{
		using value_type = T;
		using type = integral_constant;

		static constexpr T value = v;

		constexpr operator value_type(void) const noexcept { return value; }

		// Since C++14
		constexpr T operator ()(void) const noexcept { return value; }

	};

	template< bool value > using bool_constant = integral_constant<bool, value>;

	using true_type = bool_constant<true>;

	using false_type = bool_constant<false>;

	template< typename T > struct remove_reference { using type = T; };
	template< typename T > struct remove_reference<T&> { using type = T; };
	template< typename T > struct remove_reference<T&&> { using type = T; };

	template< typename T > struct is_lvalue_reference : false_type {};
	template< typename T > struct is_lvalue_reference<T&> : true_type {};

	template< typename T > /*constexpr*/ remove_reference_t<T> && move(T && t) noexcept
	{
		return static_cast<remove_reference_t<T> &&>(t);
	}
	
	template< typename T > /*constexpr*/ T&& forward(remove_reference_t<T> & t) noexcept
	{
		return static_cast<T &&>(t);
	}
	
	template< typename T > /*constexpr*/ T&& forward(remove_reference_t<T> && t) noexcept
	{
		static_assert(!is_lvalue_reference<T>::value, "template argument substituting T is an lvalue reference type");
		return static_cast<T &&>(t);
	}
}
#else
#include <utility>
#endif