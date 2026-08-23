#pragma once

#include <algorithm>

namespace CKPE
{
	template <class E, class U = std::underlying_type_t<E>>
	class TEnumSet
	{
		U _impl{ 0 };
	public:
		using enum_type = E;
		using underlying_type = U;

		static_assert(std::is_enum_v<E>, "TEnumSet<E, ...> must be an enum");
		static_assert(std::is_integral_v<U>, "TEnumSet<..., U> must be an integral");

		constexpr TEnumSet() noexcept(true) = default;
		constexpr TEnumSet(const TEnumSet&) noexcept(true) = default;
		constexpr TEnumSet(TEnumSet&&) noexcept(true) = default;

		template <class U2>
		constexpr TEnumSet(TEnumSet<E, U2> a_rhs) noexcept(true) :
			_impl(static_cast<U>(a_rhs.get()))
		{}

		template <class... Args>
		constexpr TEnumSet(Args... a_values) noexcept(true)
			requires(std::same_as<Args, E> && ...)
			:
			_impl((static_cast<U>(a_values) | ...))
		{}

		~TEnumSet() noexcept(true) = default;

		constexpr TEnumSet& operator=(const TEnumSet&) noexcept(true) = default;
		constexpr TEnumSet& operator=(TEnumSet&&) noexcept(true) = default;

		template <class U2>
		constexpr TEnumSet& operator=(TEnumSet<E, U2> a_rhs) noexcept(true)
		{
			_impl = static_cast<U>(a_rhs.get());
			return *this;
		}

		constexpr TEnumSet& operator=(E a_value) noexcept(true)
		{
			_impl = static_cast<U>(a_value);
			return *this;
		}

		[[nodiscard]] explicit constexpr operator bool() const noexcept(true) { return _impl != static_cast<U>(0); }

		[[nodiscard]] constexpr E operator*() const noexcept(true) { return get(); }
		[[nodiscard]] constexpr E get() const noexcept(true) { return static_cast<E>(_impl); }
		[[nodiscard]] constexpr U underlying() const noexcept(true) { return _impl; }

		template <class... Args>
		constexpr TEnumSet& set(Args... a_args) noexcept(true)
			requires(std::same_as<Args, E> && ...)
		{
			_impl |= (static_cast<U>(a_args) | ...);
			return *this;
		}

		template <class... Args>
		constexpr TEnumSet& set(bool a_set, Args... a_args) noexcept(true)
			requires(std::same_as<Args, E> && ...)
		{
			if (a_set)
				_impl |= (static_cast<U>(a_args) | ...);
			else
				_impl &= ~(static_cast<U>(a_args) | ...);

			return *this;
		}

		template <class... Args>
		constexpr TEnumSet& reset(Args... a_args) noexcept(true)
			requires(std::same_as<Args, E> && ...)
		{
			_impl &= ~(static_cast<U>(a_args) | ...);
			return *this;
		}

		constexpr TEnumSet& reset() noexcept(true)
		{
			_impl = 0;
			return *this;
		}

		template <class... Args>
		[[nodiscard]] constexpr bool any(Args... a_args) const noexcept(true)
			requires(std::same_as<Args, E> && ...)
		{
			return (_impl & (static_cast<U>(a_args) | ...)) != static_cast<U>(0);
		}

		template <class... Args>
		[[nodiscard]] constexpr bool all(Args... a_args) const noexcept(true)
			requires(std::same_as<Args, E> && ...)
		{
			return (_impl & (static_cast<U>(a_args) | ...)) == (static_cast<U>(a_args) | ...);
		}

		template <class... Args>
		[[nodiscard]] constexpr bool none(Args... a_args) const noexcept(true)
			requires(std::same_as<Args, E> && ...)
		{
			return (_impl & (static_cast<U>(a_args) | ...)) == static_cast<U>(0);
		}

		friend constexpr bool operator==(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs.underlying() == a_rhs.underlying(); }
		friend constexpr bool operator==(TEnumSet a_lhs, E a_rhs) noexcept(true) { return a_lhs.underlying() == static_cast<U>(a_rhs); }
		friend constexpr bool operator==(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<U>(a_lhs) == a_rhs.underlying(); }

		friend constexpr std::strong_ordering operator<=>(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs.underlying() <=> a_rhs.underlying(); }
		friend constexpr std::strong_ordering operator<=>(TEnumSet a_lhs, E a_rhs) noexcept(true) { return a_lhs.underlying() <=> static_cast<U>(a_rhs); }
		friend constexpr std::strong_ordering operator<=>(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<U>(a_lhs) <=> a_rhs.underlying(); }

		friend constexpr TEnumSet operator&(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() & a_rhs.underlying()); }
		friend constexpr TEnumSet operator&(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() & static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator&(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) & a_rhs.underlying()); }

		friend constexpr TEnumSet& operator&=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs & a_rhs; }
		friend constexpr TEnumSet& operator&=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs & a_rhs; }

		friend constexpr TEnumSet operator|(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() | a_rhs.underlying()); }
		friend constexpr TEnumSet operator|(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() | static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator|(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) | a_rhs.underlying()); }

		friend constexpr TEnumSet& operator|=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs | a_rhs; }
		friend constexpr TEnumSet& operator|=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs | a_rhs; }

		friend constexpr TEnumSet operator^(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() ^ a_rhs.underlying()); }
		friend constexpr TEnumSet operator^(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() ^ static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator^(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) ^ a_rhs.underlying()); }

		friend constexpr TEnumSet& operator^=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs ^ a_rhs; }
		friend constexpr TEnumSet& operator^=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs ^ a_rhs; }

		friend constexpr TEnumSet operator+(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() + a_rhs.underlying()); }
		friend constexpr TEnumSet operator+(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() + static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator+(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) + a_rhs.underlying()); }

		friend constexpr TEnumSet& operator+=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs + a_rhs; }
		friend constexpr TEnumSet& operator+=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs + a_rhs; }

		friend constexpr TEnumSet operator-(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() - a_rhs.underlying()); }
		friend constexpr TEnumSet operator-(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() - static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator-(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) - a_rhs.underlying()); }

		friend constexpr TEnumSet& operator-=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs - a_rhs; }
		friend constexpr TEnumSet& operator-=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs - a_rhs; }

		friend constexpr TEnumSet operator<<(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() << a_rhs.underlying()); }
		friend constexpr TEnumSet operator<<(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() << static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator<<(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) << a_rhs.underlying()); }

		friend constexpr TEnumSet& operator<<=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs << a_rhs; }
		friend constexpr TEnumSet& operator<<=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs << a_rhs; }

		friend constexpr TEnumSet operator>>(TEnumSet a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() >> a_rhs.underlying()); }
		friend constexpr TEnumSet operator>>(TEnumSet a_lhs, E a_rhs) noexcept(true) { return static_cast<E>(a_lhs.underlying() >> static_cast<U>(a_rhs)); }
		friend constexpr TEnumSet operator>>(E a_lhs, TEnumSet a_rhs) noexcept(true) { return static_cast<E>(static_cast<U>(a_lhs) >> a_rhs.underlying()); }

		friend constexpr TEnumSet& operator>>=(TEnumSet& a_lhs, TEnumSet a_rhs) noexcept(true) { return a_lhs = a_lhs >> a_rhs; }
		friend constexpr TEnumSet& operator>>=(TEnumSet& a_lhs, E a_rhs) noexcept(true) { return a_lhs = a_lhs >> a_rhs; }

		friend constexpr TEnumSet& operator~(TEnumSet& a_lhs) noexcept(true) { return a_lhs = ~a_lhs.underlying(); }
	};

	template <class... Args>
	TEnumSet(Args...) -> TEnumSet<std::common_type_t<Args...>, std::underlying_type_t<std::common_type_t<Args...>>>;
}

#define CKPE_DEFINE_ENUM_CLASS_FLAGS(E)\
	constexpr E&	operator|=(E& a_lhs, E a_rhs)	noexcept(true) { return a_lhs = (E)((__underlying_type(E))a_lhs | (__underlying_type(E))a_rhs); }\
	constexpr E&	operator&=(E& a_lhs, E a_rhs)	noexcept(true) { return a_lhs = (E)((__underlying_type(E))a_lhs & (__underlying_type(E))a_rhs); }\
	constexpr E&	operator^=(E& a_lhs, E a_rhs)	noexcept(true) { return a_lhs = (E)((__underlying_type(E))a_lhs ^ (__underlying_type(E))a_rhs); }\
	constexpr E		operator|(E a_lhs, E a_rhs)		noexcept(true) { return (E)((__underlying_type(E))a_lhs | (__underlying_type(E))a_rhs); }\
	constexpr E		operator&(E a_lhs, E a_rhs)		noexcept(true) { return (E)((__underlying_type(E))a_lhs & (__underlying_type(E))a_rhs); }\
	constexpr E		operator^(E a_lhs, E a_rhs)		noexcept(true) { return (E)((__underlying_type(E))a_lhs ^ (__underlying_type(E))a_rhs); }\
	constexpr bool	operator!(E a_lhs)				noexcept(true) { return !(__underlying_type(E))a_lhs; }\
	constexpr E		operator~(E a_lhs)				noexcept(true) { return (E) ~(__underlying_type(E))a_lhs; }
