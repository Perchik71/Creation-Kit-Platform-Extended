// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Utils.h>
#include <CKPE.Common.ID.h>
#include <CKPE.Common.Offset.h>
#include <algorithm>
#include <span>
#include <functional>
#include <cassert>

namespace CKPE
{
	namespace Common
	{
		inline constexpr std::uint8_t NOP = 0x90;
		inline constexpr std::uint8_t RET = 0xC3;
		inline constexpr std::uint8_t INT3 = 0xCC;

		template <class T = std::uintptr_t>
		class Relocation
		{
		public:
			using value_type =
				std::conditional_t<
				std::is_member_pointer_v<T> || std::is_function_v<std::remove_pointer_t<T>>,
				std::decay_t<T>,
				T>;

			constexpr Relocation() noexcept = default;

			explicit constexpr Relocation(std::uintptr_t a_address) noexcept :
				_impl{ a_address }
			{}

			explicit Relocation(const Offset& a_offset) :
				_impl{ a_offset.Address() }
			{}

			explicit Relocation(const ID& a_id) :
				_impl{ a_id.Address() }
			{}

			explicit Relocation(const ID& a_id, std::ptrdiff_t a_offset) :
				_impl{ a_id.Address() + a_offset }
			{}

			explicit Relocation(const ID& a_id, const Offset& a_offset) :
				_impl{ a_id.Address() + a_offset.Value() }
			{}

			constexpr Relocation& operator=(std::uintptr_t a_address) noexcept
			{
				_impl = a_address;
				return *this;
			}

			Relocation& operator=(const Offset& a_offset)
			{
				_impl = a_offset.Address();
				return *this;
			}

			Relocation& operator=(const ID& a_id)
			{
				_impl = a_id.Address();
				return *this;
			}

			template <class U = value_type>
			[[nodiscard]] decltype(auto) operator*() const noexcept
				requires(std::is_pointer_v<U>)
			{
				return *Get();
			}

			template <class U = value_type>
			[[nodiscard]] auto operator->() const noexcept
				requires(std::is_pointer_v<U>)
			{
				return Get();
			}

			[[nodiscard]] operator bool() const noexcept
			{
				return _impl != 0;
			}

			template <class... Args>
			std::invoke_result_t<const value_type&, Args...> operator()(Args&&... a_args) const
				noexcept(std::is_nothrow_invocable_v<const value_type&, Args...>)
				requires(std::invocable<const value_type&, Args...>)
			{
				return std::function<T>((T*)Address())(std::forward<Args>(a_args)...);
			}

			[[nodiscard]] constexpr std::uintptr_t Address() const noexcept(true) { return _impl; }
			[[nodiscard]] std::size_t              Offset() const { return _impl - Base(); }

			[[nodiscard]] value_type Get() const
				noexcept(std::is_nothrow_copy_constructible_v<value_type>)
			{
				assert(_impl != 0);
				return *(value_type*)&_impl;
			}

			template <std::ptrdiff_t O = 0>
			void Write(const void* a_src, std::size_t a_count)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				SafeWrite::Write(Address() + O, 
					reinterpret_cast<const std::uint8_t*>(a_src), a_count);
			}

			template <std::ptrdiff_t O = 0, std::integral U>
			void Write(const U& a_data)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				SafeWrite::Write(Address() + O, 
					reinterpret_cast<const std::uint8_t*>(std::addressof(a_data)), sizeof(U));
			}

			template <std::ptrdiff_t O = 0>
			void Write(const std::initializer_list<std::uint8_t> a_data)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				SafeWrite::Write(Address() + O, a_data.begin(), a_data.size());
			}

			template <std::ptrdiff_t O = 0, class U>
			void Write(const std::span<U> a_data)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				SafeWrite::Write(Address() + O,
					reinterpret_cast<const std::uint8_t*>(a_data.data()), a_data.size_bytes());
			}

			template <std::ptrdiff_t O = 0>
			std::uintptr_t WriteJump(const std::uintptr_t a_dst)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				return Detours::DetourJump(Address() + O, a_dst);
			}

			template <std::ptrdiff_t O = 0, class F>
			std::uintptr_t WriteJump(const F a_dst)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				return Detours::DetourJump(Address() + O, *(uintptr_t*)&a_dst);
			}

			template <std::ptrdiff_t O = 0>
			std::uintptr_t WriteCall(const std::uintptr_t a_dst)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				return Detours::DetourCall(Address() + O, a_dst);
			}

			template <std::ptrdiff_t O = 0, class F>
			std::uintptr_t WriteCall(const F a_dst)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				return Detours::DetourCall(Address() + O, *(uintptr_t*)&a_dst);
			}

			template <std::ptrdiff_t O = 0>
			void WriteFill(const std::uint8_t a_value, const std::size_t a_count)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				SafeWrite::WriteSet(Address() + O, a_value, a_count);
			}

			template <class U = value_type>
			std::uintptr_t WriteVFunc(std::size_t a_idx, std::uintptr_t a_newFunc)
				requires(std::same_as<U, std::uintptr_t>)
			{
				return Detours::DetourVTable(Address(), a_newFunc, a_idx * sizeof(std::uintptr_t));
			}

			template <class F>
			std::uintptr_t WriteVFunc(std::size_t a_idx, F a_newFunc)
				requires(std::same_as<value_type, std::uintptr_t>)
			{
				return WriteVFunc(a_idx, *(uintptr_t*)&a_newFunc);
			}

		private:
			[[nodiscard]] static std::uintptr_t Base() noexcept(true)
			{
				return Application::GetSingleton()->GetBase();
			}

			std::uintptr_t _impl{ 0 };
		};
	}
}