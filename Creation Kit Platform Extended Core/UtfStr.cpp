// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Conversion
	{
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
	}
}