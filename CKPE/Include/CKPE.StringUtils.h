// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <string>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API StringUtils
	{
		constexpr StringUtils() noexcept(true) = default;
		StringUtils(const StringUtils&) = delete;
		StringUtils& operator=(const StringUtils&) = delete;
	public:
		static bool IsASCII(const std::string& src) noexcept(true);
		static std::wstring Utf8ToUtf16(const std::string& src) noexcept(true);
		static std::string Utf16ToUtf8(const std::wstring& src) noexcept(true);
		static std::string Utf16ToWinCP(const std::wstring& src) noexcept(true);
		static std::wstring WinCPToUtf16(const std::string& src) noexcept(true);
		static std::string Utf8ToWinCP(const std::string& src) noexcept(true);
		static std::string WinCPToUtf8(const std::string& src) noexcept(true);

		static std::string ToUpperUTF8(const std::string& src) noexcept(true);
		static std::string ToLowerUTF8(const std::string& src) noexcept(true);
		static std::wstring ToUpper(const std::wstring& src) noexcept(true);
		static std::wstring ToLower(const std::wstring& src) noexcept(true);
	};
}