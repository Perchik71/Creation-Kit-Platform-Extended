// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "ProfileUtil.h"

namespace CreationKitPlatformExtended
{
	namespace Conversion
	{
		bool LazUnicodePluginInit();
		bool IsUtf8Valid(const String& str);
		String Utf16ToUtf8(const WideString& str);
		WideString Utf8ToUtf16(const String& str);
		String Utf8ToAnsi(const String& str);
		String AnsiToUtf8(const String& str);
		String Utf16ToAnsi(const WideString& str);
		WideString AnsiToUtf16(const String& str);
		String WideToAnsi(const WideString& str);
		WideString AnsiToWide(const String& str);
	}

	namespace Utils
	{
		String QuoteRemove(const char* s);
		WideString QuoteRemove(const wchar_t* s);

		String GetApplicationPath();

		static const char* whitespaceDelimiters = " \t\n\r\f\v";

		inline String& Trim(String& str) 
		{
			str.erase(str.find_last_not_of(whitespaceDelimiters) + 1);
			str.erase(0, str.find_first_not_of(whitespaceDelimiters));

			return str;
		}

		inline String Trim(const char* s) 
		{
			String str(s);
			return Trim(str);
		}

		uint64_t MurmurHash64A(const void* Key, size_t Len, uint64_t Seed = 0);

		// https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
		String GetLastErrorToStr(DWORD err, const String& namefunc);
		String GetLastErrorToStr(const String& namefunc);

		// DirNameOf https://stackoverflow.com/questions/8518743/get-directory-from-file-path-c/34740989
		static inline String DirNameOf(const String& path)
		{
			size_t pos = path.find_last_of("\\/");
			return (std::string::npos == pos)
				? ""
				: path.substr(0, pos + 1);
		}
	}
}