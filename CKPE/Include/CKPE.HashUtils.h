// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdio>
#include <cstdint>
#include <string>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API HashUtils
	{
		constexpr HashUtils() noexcept(true) = default;
		HashUtils(const HashUtils&) = delete;
		HashUtils& operator=(const HashUtils&) = delete;
	public:
		static std::uint32_t CRC32(const char* in) noexcept(true);
		static std::uint32_t CRC32Buffer(const void* in, const std::uint32_t size) noexcept(true);
		static std::uint32_t CRC32Update(const void* in, const std::uint32_t size, std::uint32_t prev_crc) noexcept(true);
		static std::uint32_t CRC32Final(std::uint32_t crc) noexcept(true);
		static std::uint32_t CRC32File(const std::string& filename) noexcept(true);
		static std::uint32_t CRC32File(const std::wstring& filename) noexcept(true);
		static std::uint32_t MurmurHash32(const void* key, std::size_t len, std::uint32_t seed = 0) noexcept(true);
		static std::uint64_t MurmurHash64A(const void* key, std::size_t len, std::uint64_t seed = 0) noexcept(true);
		static std::uint32_t MurmurHash32(const std::string& key, std::uint32_t seed = 0) noexcept(true);
		static std::uint64_t MurmurHash64A(const std::string& key, std::uint64_t seed = 0) noexcept(true);
	};
}