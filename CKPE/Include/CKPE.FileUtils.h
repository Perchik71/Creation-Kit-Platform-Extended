// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API FileUtils
	{
		constexpr FileUtils() noexcept(true) = default;
		FileUtils(const FileUtils&) = delete;
		FileUtils& operator=(const FileUtils&) = delete;
	public:
		[[nodiscard]] static std::uint64_t GetFileVersion(const std::string& fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileVersion(const std::wstring& fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileVersion(const char* fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileVersion(const wchar_t* fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileSize(const std::string& fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileSize(const std::wstring& fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileSize(const char* fname) noexcept(true);
		[[nodiscard]] static std::uint64_t GetFileSize(const wchar_t* fname) noexcept(true);
	};
}