// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Patterns.h>
#include <algorithm>

namespace CKPE
{
	std::uintptr_t Patterns::FindByMask(std::uintptr_t start_address, std::uintptr_t max_size, 
		const char* mask) noexcept(true)
	{
		std::vector<std::pair<std::uint8_t, bool>> pattern;

		for (std::size_t i = 0; i < strlen(mask);)
		{
			if (mask[i] != '?')
			{
				pattern.emplace_back((std::uint8_t)strtoul(&mask[i], nullptr, 16), false);
				i += 3;
			}
			else
			{
				pattern.emplace_back(0x00, true);
				i += 2;
			}
		}

		const std::uint8_t* dataStart = (std::uint8_t*)start_address;
		const std::uint8_t* dataEnd = (std::uint8_t*)start_address + max_size + 1;

		auto ret = std::search(dataStart, dataEnd, pattern.begin(), pattern.end(),
			[](std::uint8_t CurrentByte, std::pair<std::uint8_t, bool>& Pattern) {
				return Pattern.second || (CurrentByte == Pattern.first);
			});

		if (ret == dataEnd)
			return 0;

		return std::distance(dataStart, ret) + start_address;
	}

	std::vector<std::uintptr_t> Patterns::FindsByMask(std::uintptr_t start_address, std::uintptr_t max_size,
		const char* mask) noexcept(true)
	{
		std::vector<std::uintptr_t> results;
		std::vector<std::pair<std::uint8_t, bool>> pattern;

		for (std::size_t i = 0; i < strlen(mask);)
		{
			if (mask[i] != '?')
			{
				pattern.emplace_back((std::uint8_t)strtoul(&mask[i], nullptr, 16), false);
				i += 3;
			}
			else
			{
				pattern.emplace_back(0x00, true);
				i += 2;
			}
		}

		const std::uint8_t* dataStart = (uint8_t*)start_address;
		const std::uint8_t* dataEnd = (uint8_t*)start_address + max_size + 1;

		for (const std::uint8_t* i = dataStart;;)
		{
			auto ret = std::search(i, dataEnd, pattern.begin(), pattern.end(),
				[](std::uint8_t CurrentByte, std::pair<uint8_t, bool>& Pattern) {
					return Pattern.second || (CurrentByte == Pattern.first);
				});

			if (ret == dataEnd)
				break;

			std::uintptr_t addr = std::distance(dataStart, ret) + start_address;
			results.push_back(addr);

			i = (std::uint8_t*)(addr + 1);
		}

		return results;
	}

	std::uintptr_t Patterns::FindByMask(std::uintptr_t start_address, std::uintptr_t max_size, 
		const std::string& mask) noexcept(true)
	{
		std::vector<std::pair<std::uint8_t, bool>> pattern;

		for (std::size_t i = 0; i < mask.length();)
		{
			if (mask[i] != '?')
			{
				pattern.emplace_back((std::uint8_t)strtoul(&mask[i], nullptr, 16), false);
				i += 3;
			}
			else
			{
				pattern.emplace_back(0x00, true);
				i += 2;
			}
		}

		const std::uint8_t* dataStart = (std::uint8_t*)start_address;
		const std::uint8_t* dataEnd = (std::uint8_t*)start_address + max_size + 1;

		auto ret = std::search(dataStart, dataEnd, pattern.begin(), pattern.end(),
			[](std::uint8_t CurrentByte, std::pair<std::uint8_t, bool>& Pattern) {
				return Pattern.second || (CurrentByte == Pattern.first);
			});

		if (ret == dataEnd)
			return 0;

		return std::distance(dataStart, ret) + start_address;
	}

	std::vector<std::uintptr_t> Patterns::FindsByMask(std::uintptr_t start_address, std::uintptr_t max_size, 
		const std::string& mask) noexcept(true)
	{
		std::vector<std::uintptr_t> results;
		std::vector<std::pair<std::uint8_t, bool>> pattern;

		for (std::size_t i = 0; i < mask.length();)
		{
			if (mask[i] != '?')
			{
				pattern.emplace_back((std::uint8_t)strtoul(&mask[i], nullptr, 16), false);
				i += 3;
			}
			else
			{
				pattern.emplace_back(0x00, true);
				i += 2;
			}
		}

		const std::uint8_t* dataStart = (uint8_t*)start_address;
		const std::uint8_t* dataEnd = (uint8_t*)start_address + max_size + 1;

		for (const std::uint8_t* i = dataStart;;)
		{
			auto ret = std::search(i, dataEnd, pattern.begin(), pattern.end(),
				[](std::uint8_t CurrentByte, std::pair<uint8_t, bool>& Pattern) {
					return Pattern.second || (CurrentByte == Pattern.first);
				});

			if (ret == dataEnd)
				break;

			std::uintptr_t addr = std::distance(dataStart, ret) + start_address;
			results.push_back(addr);

			i = (std::uint8_t*)(addr + 1);
		}

		return results;
	}

	std::string Patterns::ASCIIStringToMask(const std::string_view& mask) noexcept(true)
	{
		std::string r;
		char buf[4]{0};

		for (auto ch : mask)
		{
			if (ch >= 0x80)
				return "";

			_itoa_s(ch, buf, 16);
			r += buf;
			r += " ";
		}

		return r;
	}
}