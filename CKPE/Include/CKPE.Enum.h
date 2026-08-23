#pragma once

#include <algorithm>

namespace CKPE
{
	template <class E, class U = std::underlying_type_t<E>>
	class TEnum
	{
		U _impl{ 0 };
	public:
		using enum_type = E;
		using underlying_type = U;

		static_assert(std::is_enum_v<E>, "TEnum<E, ...> must be an enum");
		static_assert(std::is_integral_v<U>, "TEnum<..., U> must be an integral");

		constexpr TEnum() noexcept(true) = default;
		constexpr TEnum(const TEnum&) noexcept(true) = default;
		constexpr TEnum(TEnum&&) noexcept(true) = default;

		template <class U2>
		constexpr TEnum(TEnum<E, U2> a_rhs) noexcept(true) :
			_impl(static_cast<U>(a_rhs.get()))
		{}

		constexpr TEnum(E a_value) noexcept(true) :
			_impl(static_cast<U>(a_value))
		{}

		~TEnum() noexcept(true) = default;

		constexpr TEnum& operator=(const TEnum&) noexcept(true) = default;
		constexpr TEnum& operator=(TEnum&&) noexcept(true) = default;

		template <class U2>
		constexpr TEnum& operator=(TEnum<E, U2> a_rhs) noexcept(true)
		{
			_impl = static_cast<U>(a_rhs.get());
		}

		constexpr TEnum& operator=(E a_value) noexcept(true)
		{
			_impl = static_cast<U>(a_value);
			return *this;
		}

		[[nodiscard]] explicit constexpr operator bool() const noexcept(true) { return _impl != static_cast<U>(0); }

		[[nodiscard]] constexpr E operator*() const noexcept(true) { return get(); }
		[[nodiscard]] constexpr E get() const noexcept(true) { return static_cast<E>(_impl); }
		[[nodiscard]] constexpr U underlying() const noexcept(true) { return _impl; }

		friend constexpr bool operator==(TEnum a_lhs, TEnum a_rhs) noexcept(true) { return a_lhs.underlying() == a_rhs.underlying(); }
		friend constexpr bool operator==(TEnum a_lhs, E a_rhs) noexcept(true) { return a_lhs.underlying() == static_cast<U>(a_rhs); }
		friend constexpr bool operator==(E a_lhs, TEnum a_rhs) noexcept(true) { return static_cast<U>(a_lhs) == a_rhs.underlying(); }
	};

	template <class... Args>
	TEnum(Args...) -> TEnum<std::common_type_t<Args...>, std::underlying_type_t<std::common_type_t<Args...>>>;
}