// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Patterns
	{
		constexpr Patterns() noexcept(true) = default;
		Patterns(const Patterns&) = delete;
		Patterns& operator=(const Patterns&) = delete;
	public:
		enum CreateFlag : std::uint32_t
		{
			DEFAULT_MASK = 0,
			XDBG64_MASK,
		};

		static std::string CreateMask(std::uintptr_t start_address, std::size_t size, CreateFlag flag = DEFAULT_MASK) noexcept(true);
		static std::uintptr_t FindByMask(std::uintptr_t start_address, std::uintptr_t max_size, const char* mask) noexcept(true);
		static std::vector<std::uintptr_t> FindsByMask(std::uintptr_t start_address, std::uintptr_t max_size, const char* mask) noexcept(true);
		static std::uintptr_t FindByMask(std::uintptr_t start_address, std::uintptr_t max_size, const std::string& mask) noexcept(true);
		static std::vector<std::uintptr_t> FindsByMask(std::uintptr_t start_address, std::uintptr_t max_size, const std::string& mask) noexcept(true);
		static std::string ASCIIStringToMask(const std::string_view& str) noexcept(true);
	};
}