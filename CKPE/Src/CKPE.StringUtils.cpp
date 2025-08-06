// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <limits.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <CKPE.StringUtils.h>
#include "Impl/utfcpp/utf8.h"
#include "Impl/utf8.h"

//#define UNICODE_USES_WINDOWS

namespace CKPE
{
	bool StringUtils::IsASCII(const std::string& src) noexcept(true)
	{
		for (std::size_t i = 0; i < src.length(); i++)
			if (src[i] & 0x80) return false;
		return true;
	}

	bool StringUtils::IsUtf8(const std::string& src) noexcept(true)
	{
#ifdef UNICODE_USES_WINDOWS
		auto src_1 = (const uint8_t*)(src.c_str());
		std::int32_t charlen = 0;
		auto length = src.length();
		std::size_t result = 0;

		while (result < length)
		{
			uint8_t c = *src_1;
			if (c < 0x80)
				// regular single byte ASCII character
				charlen = 1;
			else if (c <= 0xC1)
			{
				// single byte character, between valid UTF-8 encodings
				// 11000000 and 11000001 map 2 byte to 0..128, which is invalid and used for XSS attacks
				if (c >= 192) return false;
				charlen = 1;
			}
			else if (c <= 0xDF)
			{
				// could be 2 byte character (%110xxxxx %10xxxxxx)
				if ((result < (length - 1)) && ((src_1[1] & 0xC0) == 0x80))
					charlen = 2;
				else
					// missing following bytes
					return false;
			}
			else if (c <= 0xEF)
			{
				// could be 3 byte character (%1110xxxx %10xxxxxx %10xxxxxx)
				if ((result < (length - 2)) && ((src_1[1] & 0xC0) == 0x80) && ((src_1[2] & 0xC0) == 0x80))
				{
					if ((c == 0xE0) && (src_1[1] <= 0x9F))
						// XSS attack: 3 bytes are mapped to the 1 or 2 byte codes
						return false;
					charlen = 3;
				}
				else
					// missing following bytes
					return false;
			}
			else if (c <= 0xF7)
			{
				// could be 4 byte character (%11110xxx %10xxxxxx %10xxxxxx %10xxxxxx)
				if ((result < (length - 3)) && ((src_1[1] & 0xC0) == 0x80) && ((src_1[2] & 0xC0) == 0x80) && ((src_1[3] & 0xC0) == 0x80))
				{
					if ((c == 0xF0) && (src_1[1] <= 0x8F))
						// XSS attack: 4 bytes are mapped to the 1-3 byte codes
						return false;

					if ((c > 0xF4) || ((c == 0xF4) && (src_1[1] > 0x8F)))
						// out of range U+10FFFF
						return false;

					charlen = 4;
				}
				else
					// missing following bytes
					return false;
			}
			else
				return false;

			result += charlen;
			src_1 += charlen;

			if (result > length)
				// missing following bytes
				return false;
		}

		return true;
#else
		return utf8::is_valid(src);
#endif // DEBUG
	}

	bool StringUtils::IsASCII(const char* src) noexcept(true)
	{
		if (!src) return false;
		for (std::size_t i = 0; src[i]; i++)
			if (src[i] & 0x80) return false;
		return true;
	}

	bool StringUtils::IsUtf8(const char* src) noexcept(true)
	{
		if (!src) return false;

#ifdef UNICODE_USES_WINDOWS
		auto src_1 = (const uint8_t*)(src);
		std::int32_t charlen = 0;
		auto length = strlen(src);
		std::size_t result = 0;

		while (result < length)
		{
			uint8_t c = *src_1;
			if (c < 0x80)
				// regular single byte ASCII character
				charlen = 1;
			else if (c <= 0xC1)
			{
				// single byte character, between valid UTF-8 encodings
				// 11000000 and 11000001 map 2 byte to 0..128, which is invalid and used for XSS attacks
				if (c >= 192) return false;
				charlen = 1;
			}
			else if (c <= 0xDF)
			{
				// could be 2 byte character (%110xxxxx %10xxxxxx)
				if ((result < (length - 1)) && ((src_1[1] & 0xC0) == 0x80))
					charlen = 2;
				else
					// missing following bytes
					return false;
			}
			else if (c <= 0xEF)
			{
				// could be 3 byte character (%1110xxxx %10xxxxxx %10xxxxxx)
				if ((result < (length - 2)) && ((src_1[1] & 0xC0) == 0x80) && ((src_1[2] & 0xC0) == 0x80))
				{
					if ((c == 0xE0) && (src_1[1] <= 0x9F))
						// XSS attack: 3 bytes are mapped to the 1 or 2 byte codes
						return false;
					charlen = 3;
				}
				else
					// missing following bytes
					return false;
			}
			else if (c <= 0xF7)
			{
				// could be 4 byte character (%11110xxx %10xxxxxx %10xxxxxx %10xxxxxx)
				if ((result < (length - 3)) && ((src_1[1] & 0xC0) == 0x80) && ((src_1[2] & 0xC0) == 0x80) && ((src_1[3] & 0xC0) == 0x80))
				{
					if ((c == 0xF0) && (src_1[1] <= 0x8F))
						// XSS attack: 4 bytes are mapped to the 1-3 byte codes
						return false;

					if ((c > 0xF4) || ((c == 0xF4) && (src_1[1] > 0x8F)))
						// out of range U+10FFFF
						return false;

					charlen = 4;
				}
				else
					// missing following bytes
					return false;
			}
			else
				return false;

			result += charlen;
			src_1 += charlen;

			if (result > length)
				// missing following bytes
				return false;
		}

		return true;
#else
		return utf8::is_valid(src);
#endif // DEBUG
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
		if (!src.empty() && (src.length() < (std::size_t)std::numeric_limits<int>::max()))
		{
			auto len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src.c_str(), (int)src.length(), 0, 0);
			if (len > 0)
			{
				auto mem = std::make_unique<wchar_t[]>((std::size_t)len + 1);
				if (mem)
				{
					if (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src.c_str(), (int)src.length(),
						mem.get(), len) > 0)
						return mem.get();
				}
			}
		}
		return L"";
	}

	std::string StringUtils::Utf8ToWinCP(const std::string& src) noexcept(true)
	{
		if (IsASCII(src) || !src.length()) return src;

#ifdef UNICODE_USES_WINDOWS
		auto wlen = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), (int)src.length(), 0, 0);
		if ((wlen > 0) && (wlen < (std::size_t)std::numeric_limits<int>::max()))
		{
			auto wmem = std::make_unique<wchar_t[]>((std::size_t)wlen + 1);
			if (wmem)
			{
				if (MultiByteToWideChar(CP_UTF8, 0, src.c_str(), (int)src.length(),
					wmem.get(), wlen) <= 0)
					return "";

				wmem[wlen] = '\0';
			}

			auto len = WideCharToMultiByte(CP_ACP, 0, wmem.get(), (int)wlen, 0, 0, nullptr, nullptr);
			if (len > 0)
			{
				auto mem = std::make_unique<char[]>((std::size_t)len + 1);
				if (mem && (WideCharToMultiByte(CP_ACP, 0, wmem.get(), (int)wlen, mem.get(), (int)len,
					nullptr, nullptr) > 0))
				{
					mem[len] = '\0';
					return mem.get();
				}
			}
		}
#else
		std::wstring u16s;
		utf8::utf8to16(src.begin(), src.end(), std::back_inserter(u16s));

		if ((u16s.length() > 0) && (u16s.length() < (std::size_t)std::numeric_limits<int>::max()))
		{
			auto len = WideCharToMultiByte(CP_ACP, 0, u16s.c_str(), (int)u16s.length(), 0, 0, nullptr, nullptr);
			if (len > 0)
			{
				auto mem = std::make_unique<char[]>((std::size_t)len + 1);
				if (mem && (WideCharToMultiByte(CP_ACP, 0, u16s.c_str(), (int)u16s.length(), mem.get(), (int)len,
					nullptr, nullptr) > 0))
				{
					mem[len] = '\0';
					return mem.get();
				}
			}
		}
#endif // UNICODE_USES_WINDOWS

		return "";
	}

	std::string StringUtils::WinCPToUtf8(const std::string& src) noexcept(true)
	{
		if (IsASCII(src) || !src.length()) return src;

		auto len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src.c_str(), (int)src.length(), 0, 0);
		if (len > 0)
		{
			auto mem = std::make_unique<wchar_t[]>((std::size_t)len + 1);
			if (mem && (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src.c_str(), (int)src.length(),
				mem.get(), len) > 0))
				return Utf16ToUtf8(mem.get());
		}
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

	static const char* whitespaceDelimitersA = " \t\n\r\f\v";

	std::string StringUtils::Trim(const std::string& str) noexcept(true)
	{
		std::string s = str;

		s.erase(s.find_last_not_of(whitespaceDelimitersA) + 1);
		s.erase(0, s.find_first_not_of(whitespaceDelimitersA));

		return s;
	}

	std::string StringUtils::Trim(const char* s) noexcept(true)
	{
		std::string str(s);
		return Trim(str);
	}

	static const wchar_t* whitespaceDelimitersW = L" \t\n\r\f\v";

	std::wstring StringUtils::Trim(const std::wstring& str) noexcept(true)
	{
		std::wstring s = str;

		s.erase(s.find_last_not_of(whitespaceDelimitersW) + 1);
		s.erase(0, s.find_first_not_of(whitespaceDelimitersW));

		return s;
	}

	std::wstring StringUtils::Trim(const wchar_t* s) noexcept(true)
	{
		std::wstring str(s);
		return Trim(str);
	}

	std::string StringUtils::QuoteRemove(const std::string& str) noexcept(true)
	{
		std::string s = str;

		if (s.size() > 1)
		{
			if ((s.front() == '"') && (s.back() == '"'))
			{
				if (s.size() == 2)
					s.erase();
				else
				{
					s.erase(s.begin());
					s.erase(s.end() - 1);
				}
			}
		}
		return s;
	}

	std::string StringUtils::QuoteRemove(const char* s) noexcept(true)
	{
		std::string r(s);
		return QuoteRemove(r);
	}

	std::wstring StringUtils::QuoteRemove(const std::wstring& str) noexcept(true)
	{
		std::wstring s = str;

		if (s.size() > 1)
		{
			if ((s.front() == L'"') && (s.back() == L'"'))
			{
				if (s.size() == 2)
					s.erase();
				else
				{
					s.erase(s.begin());
					s.erase(s.end() - 1);
				}
			}
		}
		return s;
	}

	std::wstring StringUtils::QuoteRemove(const wchar_t* s) noexcept(true)
	{
		std::wstring r(s);
		return QuoteRemove(r);
	}

	std::string StringUtils::FormatString(const std::string& format_string, ...) noexcept(true)
	{
		va_list ap;
		va_start(ap, &format_string);
		auto r = FormatStringVa(format_string, ap);
		va_end(ap);
		return r;
	}

	std::string StringUtils::FormatStringVa(const std::string& format_string, va_list ap) noexcept(true)
	{
		std::string r;
		auto l = _vscprintf(format_string.c_str(), ap);
		if (l < 1) return r;
		r.resize((std::size_t)l);
		if (!r.empty())
			vsprintf(r.data(), format_string.c_str(), ap);
		return r;
	}

	std::wstring StringUtils::FormatString(const std::wstring& format_string, ...) noexcept(true)
	{
		va_list ap;
		va_start(ap, &format_string);
		auto r = FormatStringVa(format_string, ap);
		va_end(ap);
		return r;
	}

	std::wstring StringUtils::FormatStringVa(const std::wstring& format_string, va_list ap) noexcept(true)
	{
		std::wstring r;
		auto l = _vscwprintf(format_string.c_str(), ap);
		if (l < 1) return r;
		r.resize((std::size_t)l);
		if (!r.empty())
			_vswprintf(r.data(), format_string.c_str(), ap);
		return r;
	}
}