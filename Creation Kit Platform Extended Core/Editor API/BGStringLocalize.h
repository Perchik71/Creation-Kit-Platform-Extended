// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class BGSStringCache 
		{
		private:
			List<String> cache;
		public:
			BGSStringCache(VOID) = default;
			~BGSStringCache(VOID) = default;
		public:
			inline DWORD Size(VOID) const { return (DWORD)cache.size(); }
			inline VOID Clear(VOID) { cache.clear(); }
			inline VOID Push(const String& s) { cache.push_back(s); }
			inline LPCSTR Last(VOID) const { return &cache.back()[0]; }
		};

		extern BGSStringCache StringCache;

		class BGSConvertorString 
		{
		public:
			enum {
				MODE_ANSI = 0,
				MODE_UTF8
			};
		private:
			LPCSTR pre;
			BYTE mode;
		public:
			BGSConvertorString(VOID) = default;
			~BGSConvertorString(VOID) = default;
		private:
			inline BOOL IsValid(LPCSTR s) const 
			{
				return ((s != NULL) && (s != LPSTR_TEXTCALLBACKA) && (strlen(s) > 0));
			}

			LPCSTR Utf8ToWinCP(LPCSTR src);
			LPCSTR WinCPToUtf8(LPCSTR src);
		public:
			inline BYTE GetMode(VOID) const { return mode; }
			inline VOID SetMode(BYTE m) 
			{
				mode = m;
				StringCache.Clear();
			}

			LPCSTR Convert(LPCSTR s);
		};

		extern BGSConvertorString ConvertorString;

		typedef LPSTR(*lpfnGetStr_t)(LPCSTR);
		static lpfnGetStr_t BGSLocalizedString_OldGetStrProc;

		class BGSLocalizedString {
		public:
			static LPCSTR GetStr(LPCSTR str) 
			{
				return ConvertorString.Convert(BGSLocalizedString_OldGetStrProc(str));
			}
			static LPCSTR GetStr2(LPCSTR str)
			{
				return ConvertorString.Convert(str);
			}
		};
	}
}