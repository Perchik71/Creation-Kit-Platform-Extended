// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "BGStringLocalize.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		BGSStringCache StringCache;
		BGSConvertorString ConvertorString;

		LPCSTR BGSConvertorString::Utf8ToWinCP(LPCSTR src)
		{
			// Ansi verification is necessary because there are a lot of strings, especially short and system strings. 
			// The debug file without this option was more than 70 mb, compared to 2604 kb.
			// Translation of fallout4.esm has become significantly faster.

			if ((src == pre) || !IsValid(src) || !Conversion::IsUtf8Valid(src))
				return src;

			String wincp_str = Conversion::Utf8ToAnsi(src);

			// utf-8 takes up more memory than ansi, so I can use active memory
			pre = src;
			strcpy(const_cast<LPSTR>(pre), wincp_str.c_str());

			return pre;
		}

		LPCSTR BGSConvertorString::WinCPToUtf8(LPCSTR src)
		{
			// Not all strings are translated during loading and remain in Utf-8. 
			// They are loaded after opening the dialog. As an example "Description".

			if (!IsValid(src) || Conversion::IsUtf8Valid(src))
				return src;

			// in the Creation Kit code, the request to return a string occurs twice in a row.
			if ((pre == src) && (StringCache.Size() > 0))
				return StringCache.Last();

			// convert
			String utf8_str = Conversion::AnsiToUtf8(src);

			// Unicode initially takes up more memory than ansi. 
			// Therefore, a heap is created that will store memory for the duration of saving.
			// Lists work quickly on adding items.
			StringCache.Push(utf8_str);

			// pointer to the memory of the contents of the last line, it is of course unique
			pre = src;
			return StringCache.Last();
		}

		LPCSTR BGSConvertorString::Convert(LPCSTR s) {
			// Back to utf-8 (temporarily)
			if (mode == BGSConvertorString::MODE_UTF8)
				return WinCPToUtf8(s);
			// All APIs are implemented as ANSI. In this regard, utf-8 must be converted to the specified ANSI.
			else
				return Utf8ToWinCP(s);
		}
	}
}