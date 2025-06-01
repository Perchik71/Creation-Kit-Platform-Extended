// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <initializer_list>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API SafeWrite
	{
		constexpr SafeWrite() noexcept(true) = default;
		SafeWrite(const SafeWrite&) = delete;
		SafeWrite& operator=(const SafeWrite&) = delete;
	public:
		static void Write(std::uintptr_t address, const uint8_t* data, std::size_t size) noexcept(true);
		static void Write(std::uintptr_t address, std::initializer_list<std::uint8_t> data) noexcept(true);
		static void WriteSet(std::uintptr_t address, std::uint8_t value, std::size_t size) noexcept(true);
		static void WriteNop(std::uintptr_t address, std::size_t size) noexcept(true);
	};

	class CKPE_API ScopeSafeWrite
	{
		bool _init{ false };
		std::uint32_t _of{ 0 };
		std::uintptr_t _target{ 0 };
		std::uintptr_t _size{ 0 };

		constexpr ScopeSafeWrite() noexcept(true) = default;
		ScopeSafeWrite(const ScopeSafeWrite&) = delete;
		ScopeSafeWrite& operator=(const ScopeSafeWrite&) = delete;
	public:
		ScopeSafeWrite(std::uintptr_t target, std::uintptr_t size) noexcept(true);
		~ScopeSafeWrite() noexcept(true);

		[[nodiscard]] bool Contain(std::uintptr_t address, std::size_t size) const noexcept(true);
		void Write(std::uintptr_t address, const uint8_t* data, std::size_t size) const noexcept(true);
		void Write(std::uintptr_t address, std::initializer_list<std::uint8_t> data) const noexcept(true);
		void WriteSet(std::uintptr_t address, std::uint8_t value, std::size_t size) const noexcept(true);
		void WriteNop(std::uintptr_t address, std::size_t size) const noexcept(true);
	};
}