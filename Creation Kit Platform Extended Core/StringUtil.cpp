// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <Voltek.UnicodeConverter.h>

namespace CreationKitPlatformExtended
{
	namespace Conversion
	{
#if 0
		typedef bool(*laz_unicode_plugin_is_utf8_t)(const char* str);
		typedef int(*laz_unicode_plugin_wincp_to_utf8_t)(const char* src, char* dst);
		typedef int(*laz_unicode_plugin_utf8_to_wincp_t)(const char* src, char* dst, const bool test_invalid);
		typedef int(*laz_unicode_plugin_utf16_to_utf8_t)(const wchar_t* src, char* dst);
		typedef int(*laz_unicode_plugin_utf8_to_utf16_t)(const char* src, wchar_t* dst, const bool test_invalid);

		laz_unicode_plugin_is_utf8_t laz_unicode_plugin_is_utf8 = nullptr;
		laz_unicode_plugin_wincp_to_utf8_t laz_unicode_plugin_wincp_to_utf8 = nullptr;
		laz_unicode_plugin_utf8_to_wincp_t laz_unicode_plugin_utf8_to_wincp = nullptr;
		laz_unicode_plugin_utf16_to_utf8_t laz_unicode_plugin_utf16_to_utf8 = nullptr;
		laz_unicode_plugin_utf8_to_utf16_t laz_unicode_plugin_utf8_to_utf16 = nullptr;

		bool LazUnicodePluginInit()
		{
			HMODULE hModule = LoadLibraryW(L"vup-x86_64.dll");
			if (hModule)
			{
				laz_unicode_plugin_is_utf8 = (laz_unicode_plugin_is_utf8_t)GetProcAddress(hModule, (LPCSTR)5);
				laz_unicode_plugin_wincp_to_utf8 = (laz_unicode_plugin_wincp_to_utf8_t)GetProcAddress(hModule, (LPCSTR)2);
				laz_unicode_plugin_utf8_to_wincp = (laz_unicode_plugin_utf8_to_wincp_t)GetProcAddress(hModule, (LPCSTR)1);
				laz_unicode_plugin_utf16_to_utf8 = (laz_unicode_plugin_utf16_to_utf8_t)GetProcAddress(hModule, (LPCSTR)4);
				laz_unicode_plugin_utf8_to_utf16 = (laz_unicode_plugin_utf8_to_utf16_t)GetProcAddress(hModule, (LPCSTR)3);

				return laz_unicode_plugin_is_utf8 && laz_unicode_plugin_wincp_to_utf8 && laz_unicode_plugin_utf8_to_wincp &&
					laz_unicode_plugin_utf16_to_utf8 && laz_unicode_plugin_utf8_to_utf16;
			}

			return false;
		}

		bool IsUtf8Valid(const String& str)
		{
			if (laz_unicode_plugin_is_utf8)
				return laz_unicode_plugin_is_utf8(str.c_str());
			else
			{
				if (!MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0))
					return GetLastError() == ERROR_NO_UNICODE_TRANSLATION;
				return true;
			}
		}

		String Utf16ToUtf8(const WideString& str)
		{
			String utf8str;

			if (str.empty())
				return utf8str;

			if (laz_unicode_plugin_utf16_to_utf8)
			{
				int l = laz_unicode_plugin_utf16_to_utf8(str.c_str(), NULL);
				AssertMsgVa(l > 0, "laz_unicode_plugin_utf16_to_utf8()");
				// this will null-terminate
				utf8str.resize(l);
				laz_unicode_plugin_utf16_to_utf8(str.c_str(), &utf8str[0]);
			}
			else
			{
				int utf8_cnt = WideCharToMultiByte(CP_UTF8, MB_PRECOMPOSED, str.c_str(), -1, NULL, 0, NULL, NULL);
				//AssertMsgVa(utf8_cnt > 0, "%s", XUtil::Str::GetLastErrorToStr("WideCharToMultiByte()").c_str());
				// this will null-terminate
				utf8str.resize(utf8_cnt);
				WideCharToMultiByte(CP_UTF8, MB_PRECOMPOSED, str.c_str(), -1, &utf8str[0], utf8_cnt, NULL, NULL);
			}

			return utf8str;
		}

		WideString Utf8ToUtf16(const String& str)
		{
			WideString utf16str;

			if (str.empty())
				return utf16str;

			if (laz_unicode_plugin_utf8_to_utf16)
			{
				utf16str.resize(str.length());
				int l = laz_unicode_plugin_utf8_to_utf16(str.c_str(), &utf16str[0], false);
				AssertMsgVa(l > 0, "laz_unicode_plugin_utf8_to_utf16()");
				// this will null-terminate
				utf16str.resize(l);
			}
			else
			{
				int utf16_cnt = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str.c_str(), -1, NULL, 0);
				//AssertMsgVa(utf16_cnt > 0, "%s", XUtil::Str::GetLastErrorToStr("MultiByteToWideChar()").c_str());
				// this will null-terminate
				utf16str.resize(utf16_cnt);
				MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str.c_str(), -1, &utf16str[0], utf16_cnt);
			}

			return utf16str;
		}

		String Utf8ToAnsi(const String& str)
		{
			String ansistr;

			if (str.empty())
				return ansistr;

			if (laz_unicode_plugin_utf8_to_wincp)
			{
				ansistr.resize(str.length());
				int l = laz_unicode_plugin_utf8_to_wincp(str.c_str(), &ansistr[0], false);
				AssertMsgVa(l > 0, "laz_unicode_plugin_utf8_to_wincp()");
				// this will null-terminate
				ansistr.resize(l);
			}
			else
			{
				WideString utf16str = Utf8ToUtf16(str);
				int ansi_cnt = WideCharToMultiByte(CP_ACP, 0, utf16str.c_str(), -1, NULL, 0, NULL, NULL);
				//AssertMsgVa(ansi_cnt > 0, "%s", XUtil::Str::GetLastErrorToStr("WideCharToMultiByte()").c_str());
				// this will null-terminate
				ansistr.resize(ansi_cnt);
				WideCharToMultiByte(CP_ACP, 0, utf16str.c_str(), -1, &ansistr[0], ansi_cnt, NULL, NULL);
			}

			return ansistr;
		}

		String AnsiToUtf8(const String& str)
		{
			if (str.empty())
				return String();

			if (laz_unicode_plugin_wincp_to_utf8)
			{
				String utf8str;
				int l = laz_unicode_plugin_wincp_to_utf8(str.c_str(), NULL);
				AssertMsgVa(l > 0, "laz_unicode_plugin_utf16_to_utf8()");
				// this will null-terminate
				utf8str.resize(l);
				laz_unicode_plugin_wincp_to_utf8(str.c_str(), &utf8str[0]);
				return utf8str;
			}
			else
			{
				WideString utf16str;
				int utf16_cnt = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
				//AssertMsgVa(utf16_cnt > 0, "%s", XUtil::Str::GetLastErrorToStr("MultiByteToWideChar()").c_str());
				// this will null-terminate
				utf16str.resize(utf16_cnt);
				MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &utf16str[0], utf16_cnt);

				return Utf16ToUtf8(utf16str);
			}

		}

		String Utf16ToAnsi(const WideString& str)
		{
			if (str.empty())
				return String();

			return Utf8ToAnsi(Utf16ToUtf8(str));
		}

		WideString AnsiToUtf16(const String& str)
		{
			if (str.empty())
				return WideString();

			return Utf8ToUtf16(AnsiToUtf8(str));
		}

		String WideToAnsi(const WideString& str)
		{
			String ansistr;

			if (str.empty())
				return ansistr;

			int ansi_cnt = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
			//AssertMsgVa(ansi_cnt > 0, "%s", XUtil::Str::GetLastErrorToStr("WideCharToMultiByte()").c_str());
			// this will null-terminate
			ansistr.resize(ansi_cnt);
			WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &ansistr[0], ansi_cnt, NULL, NULL);

			return ansistr;
		}

		WideString AnsiToWide(const String& str)
		{
			WideString widestr;

			if (str.empty())
				return widestr;

			int wide_cnt = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), (int)str.length(), NULL, 0);
			//AssertMsgVa(wide_cnt > 0, "%s", XUtil::Str::GetLastErrorToStr("MultiByteToWideChar()").c_str());
			// this will null-terminate
			widestr.resize(wide_cnt);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), (int)str.length(), &widestr[0], wide_cnt);

			return widestr;
		}
#else
		bool LazUnicodePluginInit()
		{
			return true;
		}

		bool IsUtf8Valid(const String& str)
		{
			return voltek::find_invalid_utf8_codepoint(str.c_str(), str.length()) == -1;
		}

		String Utf16ToUtf8(const WideString& str)
		{
			int l = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0, nullptr, nullptr);
			if (l > 0)
			{
				auto Str = std::make_unique<char[]>((size_t)l + 1);
				WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), Str.get(), l, nullptr, nullptr);
				return Str.get();
			}
			return "";
		}

		WideString Utf8ToUtf16(const String& str)
		{
			int l = MultiByteToWideChar(CP_UTF8, 0, (char*)str.c_str(), (int)str.length(), nullptr, 0);
			if (l > 0)
			{
				auto Str = std::make_unique<wchar_t[]>((size_t)l + 1);
				MultiByteToWideChar(CP_UTF8, 0, (char*)str.c_str(), (int)str.length(), Str.get(), l);
				return Str.get();
			}
			return L"";
		}

		String Utf8ToAnsi(const String& str)
		{
			String ansistr;

			if (str.empty())
				return ansistr;

			if (voltek::is_ascii(str.c_str()))
				return str;

			ansistr.resize(str.length());
			int l = voltek::utf8_to_wincp(str.c_str(), &ansistr[0]);
			AssertMsgVa(l > 0, "voltek::utf8_to_wincp()");
			// this will null-terminate
			ansistr.resize(l);
			return ansistr;
		}

		String AnsiToUtf8(const String& str)
		{
			String utf8str;

			if (str.empty())
				return String();

			int l = voltek::wincp_to_utf8(str.c_str(), NULL);
			AssertMsgVa(l > 0, "voltek::wincp_to_utf8()");
			// this will null-terminate
			utf8str.resize(l);
			voltek::wincp_to_utf8(str.c_str(), &utf8str[0]);
			return utf8str;
		}

		String Utf16ToAnsi(const WideString& str)
		{
			if (str.empty())
				return String();

			return Utf8ToAnsi(Utf16ToUtf8(str));
		}

		WideString AnsiToUtf16(const String& str)
		{
			if (str.empty())
				return WideString();

			return Utf8ToUtf16(AnsiToUtf8(str));
		}

		String WideToAnsi(const WideString& str)
		{
			int l = WideCharToMultiByte(CP_ACP, 0, str.c_str(), (int)str.length(), nullptr, 0, nullptr, nullptr);
			if (l > 0)
			{
				auto Str = std::make_unique<char[]>((size_t)l + 1);
				WideCharToMultiByte(CP_ACP, 0, str.c_str(), (int)str.length(), Str.get(), l, nullptr, nullptr);
				return Str.get();
			}
			return "";
		}

		WideString AnsiToWide(const String& str)
		{
			int l = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), nullptr, 0);
			if (l > 0)
			{
				auto Str = std::make_unique<wchar_t[]>((size_t)l + 1);
				MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), Str.get(), l);
				return Str.get();
			}
			return L"";
		}
#endif
	}

	namespace Utils
	{
		String QuoteRemove(const char* s)
		{
			String Ret(s);
			if (Ret.size() > 1) {
				if ((Ret.front() == '"') && (Ret.back() == '"')) {
					if (Ret.size() == 2)
						Ret.erase();
					else 
					{
						Ret.erase(Ret.begin());
						Ret.erase(Ret.end() - 1);
					}
				}
			}
			return Ret;
		}

		WideString QuoteRemove(const wchar_t* s)
		{
			WideString Ret(s);
			if (Ret.size() > 1) {
				if ((Ret.front() == '"') && (Ret.back() == '"')) {
					if (Ret.size() == 2)
						Ret.erase();
					else
					{
						Ret.erase(Ret.begin());
						Ret.erase(Ret.end() - 1);
					}
				}
			}
			return Ret;
		}

		String GetApplicationPath()
		{
			String Path;
			Path.resize(MAX_PATH);
			Path.resize(GetModuleFileName(GetModuleHandle(NULL), Path.data(), MAX_PATH));
			return DirNameOf(Path);
		}

		// Author: Nukem9
		uint64_t MurmurHash64A(const void* Key, size_t Len, uint64_t Seed)
		{
			/*-----------------------------------------------------------------------------
			// https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c#L65
			// MurmurHash2, 64-bit versions, by Austin Appleby
			//
			// The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
			// and endian-ness issues if used across multiple platforms.
			//
			// 64-bit hash for 64-bit platforms
			*/
			const uint64_t m = 0xc6a4a7935bd1e995ull;
			const int r = 47;

			uint64_t h = Seed ^ (Len * m);

			const uint64_t* data = (const uint64_t*)Key;
			const uint64_t* end = data + (Len / 8);

			while (data != end)
			{
				uint64_t k = *data++;

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;
			}

			const unsigned char* data2 = (const unsigned char*)data;

			switch (Len & 7)
			{
			case 7: h ^= ((uint64_t)data2[6]) << 48;
			case 6: h ^= ((uint64_t)data2[5]) << 40;
			case 5: h ^= ((uint64_t)data2[4]) << 32;
			case 4: h ^= ((uint64_t)data2[3]) << 24;
			case 3: h ^= ((uint64_t)data2[2]) << 16;
			case 2: h ^= ((uint64_t)data2[1]) << 8;
			case 1: h ^= ((uint64_t)data2[0]);
				h *= m;
			}

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}

		String GetLastErrorToStr(DWORD err, const String& namefunc)
		{
			// Retrieve the system error message for the last-error code

			String str;
			LPVOID lpMsgBuf = nullptr;
			LPVOID lpDisplayBuf = nullptr;

			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)&lpMsgBuf,
				0, NULL);

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
				((size_t)strlen((LPCSTR)lpMsgBuf) + strlen(namefunc.c_str()) + 40) * sizeof(CHAR));
			StringCchPrintfA((LPSTR)lpDisplayBuf,
				LocalSize(lpDisplayBuf) / sizeof(CHAR),
				"%s failed with error %d: %s",
				namefunc.c_str(), err, lpMsgBuf);
			str = (LPSTR)lpDisplayBuf;

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);

			return str;
		}

		String GetLastErrorToStr(const String& namefunc)
		{
			return GetLastErrorToStr(GetLastError(), namefunc);
		}
	}
}