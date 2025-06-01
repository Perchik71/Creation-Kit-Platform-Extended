// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <limits.h>
#include <CKPE.StringUtils.h>
#include "Impl/utfcpp/utf8.h"
#include "Impl/utf8.h"

namespace CKPE
{
	bool StringUtils::IsASCII(const std::string& src) noexcept(true)
	{
		for (size_t i = 0; i < src.length(); i++)
			if (src[i] & 0x80) return false;
		return true;
	}

	std::wstring StringUtils::Utf8ToUtf16(const std::string& src) noexcept(true)
	{
		std::wstring r{ L"" };
		utf8::utf8to16(src.begin(), src.end(), back_inserter(r));
		return r;
	}

	std::string StringUtils::Utf16ToUtf8(const std::wstring& src) noexcept(true)
	{
		std::string r{ "" };
		utf8::utf16to8(src.begin(), src.end(), back_inserter(r));
		return r;
	}

	std::string StringUtils::Utf16ToWinCP(const std::wstring& src) noexcept(true)
	{
		std::string r{ "" };
		if (!src.empty() && (src.size() < (std::size_t)std::numeric_limits<int>::max()))
		{
			auto len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), (int)src.length(),
				0, 0, nullptr, nullptr);
			if (len > 0)
			{
				r.resize((std::size_t)len);
				if (!r.empty())
					WideCharToMultiByte(CP_ACP, 0, src.c_str(), (int)src.length(), r.data(),
						(int)r.length(), nullptr, nullptr);
			}
		}
		return r;
	}

	std::wstring StringUtils::WinCPToUtf16(const std::string& src) noexcept(true)
	{
		std::wstring r{ L"" };
		if (!src.empty() && (src.size() < (std::size_t)std::numeric_limits<int>::max()))
		{
			auto len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src.c_str(), (int)src.length(), 0, 0);
			if (len > 0)
			{
				r.resize((std::size_t)len);
				if (!r.empty())
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src.c_str(), (int)src.length(), r.data(), (int)r.length());
			}
		}
		return r;
	}

	std::string StringUtils::Utf8ToWinCP(const std::string& src) noexcept(true)
	{
		if (IsASCII(src)) return src;
		return Utf16ToWinCP(Utf8ToUtf16(src));
	}

	std::string StringUtils::WinCPToUtf8(const std::string& src) noexcept(true)
	{
		if (IsASCII(src)) return src;
		return Utf16ToUtf8(WinCPToUtf16(src));
	}

	std::string StringUtils::ToUpperUTF8(const std::string& src) noexcept(true)
	{
		std::string s = src;
		utf8upr((char8_t*)s.data());
		return s;
	}

	std::string StringUtils::ToLowerUTF8(const std::string& src) noexcept(true)
	{
		std::string s = src;
		utf8lwr((char8_t*)s.data());
		return s;
	}

	std::wstring StringUtils::ToUpper(const std::wstring& src) noexcept(true)
	{
		return Utf8ToUtf16(ToUpperUTF8(Utf16ToUtf8(src)));
	}

	std::wstring StringUtils::ToLower(const std::wstring& src) noexcept(true)
	{
		return Utf8ToUtf16(ToLowerUTF8(Utf16ToUtf8(src)));
	}
}