//////////////////////////////////////////
/*
* Fallout 4 Script Extender (F4SE)
* by Ian Patterson, Stephen Abel, and Brendan Borthwick (ianpatt, behippo, and purplelunchbox)
* 
* Contact the F4SE Team
*
* Entire Team
* Send email to team [at] f4se [dot] silverlock [dot] org
*
* Ian (ianpatt)
* Send email to ianpatt+f4se [at] gmail [dot] com
*
* Stephen (behippo)
* Send email to gamer [at] silverlock [dot] org
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		/*
			BSString class.
			Basic dynamically-sizeable string class.
			High level string manipulation - comparison, replacement, etc. seems to be done 'manually'
			using stdlib functions rather than through member functions.  That doesn't mean that
			member functions for comparison, etc. don't exist, but if they do they are very hard to find.
			The exception so far is 'sprintf', which has a member function wrapper.

			10
		*/
		/*
			PS: modify
			PS2: I want to create one string class.
			I will add here what is often needed.
		*/
		class BSString {
		public:
			enum EResult : WORD {
				srNone = MAXWORD,
			};
			enum EFlags : WORD {
				sfNone = 0,
				sfInsensitiveCase = 1,
			};
		public:
			BSString(VOID);
			BSString(LPCSTR string, WORD size = 0);
			BSString(const std::string& string, WORD size = 0);
			BSString(const BSString& string, WORD size);
			BSString(const BSString& string);
			~BSString(VOID);
		public:
			BOOL Reserved(WORD size);
			BOOL Set(LPCSTR string, WORD size = 0);				// 0 to allocate automatically
			BOOL Set(const BSString& string, WORD size = 0);	// 0 to allocate automatically
			inline LPCSTR Get(VOID) const { return m_data ? m_data : ""; }
			inline LPCSTR c_str(VOID) const { return m_data ? m_data : ""; }
			inline WORD Length(VOID) const { return m_dataLen; }
			inline WORD Size(VOID) const { return m_bufLen; }
			INT Compare(LPCSTR string, BOOL ignoreCase = TRUE) const;
			inline INT Compare(const BSString& string, BOOL ignoreCase = TRUE) const { return Compare(string, ignoreCase); }
			VOID Clear(VOID);
			BSString Reverse(VOID) const;
			BSString UpperCase(VOID) const;
			BSString LowerCase(VOID) const;
			static void UpperCase(LPCSTR str);
			static void LowerCase(LPCSTR str);
			BSString& Assign(LPCSTR str, WORD start = 0, WORD len = 0);
			BSString& AssignUnsafe(LPCSTR str, WORD start = 0, WORD len = 0);
			BSString& Append(LPCSTR str);
			BSString& Append(const BSString& string);
			BSString& Append(CHAR ch);
			BSString& AppendFormat(LPCSTR format, ...);
			BSString& AppendFormat(LPCSTR format, va_list ap);
			BSString& Copy(WORD start = 0, WORD len = 0);
			BSString& Erase(WORD start = 0, WORD len = 0);
			BSString& Format(LPCSTR format, ...);
			BSString& Format(LPCSTR format, va_list ap);
			BSString Trim(VOID) const;
			WORD FindLastOf(CHAR ch, WORD offset = 0) const;
			WORD FindFirstOf(CHAR ch, WORD offset = 0) const;
			WORD FindLastOf(LPCSTR chs, WORD offset = 0) const;
			WORD FindFirstOf(LPCSTR chs, WORD offset = 0) const;
			WORD Find(LPCSTR substr, EFlags flags = sfNone) const;
			inline BOOL IsEmpty(VOID) const { return !m_dataLen; }
		public:
			struct Utils {
				static BSString ExtractFilePath(const BSString& fname);
				static BSString ExtractFileName(const BSString& fname);
				static BSString ExtractFileExt(const BSString& fname);

				static BSString GetCurrentPath(VOID);
				static BSString GetFileNameModule(const BSString& mname);
				static BSString GetApplicationPath(VOID);
				static BSString GetRelativeDataPath(VOID);
				static BSString GetDataPath(VOID);

				static BSString ChangeFileExt(const BSString& fname, const BSString& ext);

				static BOOL FileExists(const BSString& fname);
				static BOOL DirectoryExists(const BSString& fname);
			};
			struct Transforms {
				static BSString BoolToStr(BOOL value, BOOL inText = TRUE);
				static BSString IntToStr(INT64 value);
				static BSString UIntToStr(UINT64 value);
				static BSString FloatToStr(DOUBLE value);

				static BOOL StrToBool(const BSString& value);
				static INT64 StrToInt(const BSString& value);
				static UINT64 StrToUInt(const BSString& value);
				static DOUBLE StrToFloat(const BSString& value);

				static INT64 HexToInt(const BSString& value);
				static UINT64 HexToUInt(const BSString& value);
				static BSString IntToHex(INT64 value);
				static BSString UIntToHex(UINT64 value);
			};
			struct Converts {
				static BSString Utf8ToAnsi(const BSString& str);
				static BSString AnsiToUtf8(const BSString& str);
				static BSString WideToAnsi(LPWSTR str);
			};
		public:
			static BSString FormatString(LPCSTR format, ...);
			static BSString FormatString(LPCSTR format, va_list ap) { return BSString().Format(format, ap); }
		public:
			inline CHAR& operator[](const WORD Pos) { return m_data[Pos]; }
			inline const CHAR operator[](const WORD Pos) const { return m_data[Pos]; }
		public:
			inline BSString& operator=(CHAR ch) { CHAR szCS[2] = { ch, 0 }; Set(szCS, 1); return *this; }
			inline BSString& operator=(LPCSTR string) { Set(string, (WORD)strlen(string)); return *this; }
			inline BSString& operator=(const std::string& string) { Set(string, (WORD)string.length()); return *this; }
			inline BSString& operator=(const BSString& string) { Set(string, string.Length()); return *this; }
			inline BSString& operator=(BOOL value) { Set(BSString::Transforms::BoolToStr(value)); return *this; }
			inline BSString& operator=(INT64 value) { Set(BSString::Transforms::IntToStr(value)); return *this; }
			inline BSString& operator=(UINT64 value) { Set(BSString::Transforms::UIntToStr(value)); return *this; }
			inline BSString& operator=(DOUBLE value) { Set(BSString::Transforms::FloatToStr(value)); return *this; }
			inline BSString& operator+=(LPCSTR string) { return Append(string); }
			inline BSString& operator+=(const std::string& string) { return Append(string); }
			inline BSString& operator+=(const BSString& string) { return Append(string); }
			inline BSString& operator+=(CHAR ch) { return Append(ch); }
			inline BSString& operator+=(BOOL value) { return Append(BSString::Transforms::BoolToStr(value)); }
			inline BSString& operator+=(INT64 value) { return Append(BSString::Transforms::IntToStr(value)); }
			inline BSString& operator+=(UINT64 value) { return Append(BSString::Transforms::UIntToStr(value)); }
			inline BSString& operator+=(DOUBLE value) { return Append(BSString::Transforms::FloatToStr(value)); }
			inline BSString operator+(LPCSTR string) const { return BSString(*this).Append(string); }
			inline BSString operator+(const std::string& string) const { return BSString(*this).Append(string); }
			inline BSString operator+(const BSString& string) const { return BSString(*this).Append(string); }
			inline BSString operator+(CHAR ch) const { return BSString(*this).Append(ch); }
			inline BSString operator+(BOOL value) const { return BSString(*this).Append(BSString::Transforms::BoolToStr(value)); }
			inline BSString operator+(INT64 value) const { return BSString(*this).Append(BSString::Transforms::IntToStr(value)); }
			inline BSString operator+(UINT64 value) const { return BSString(*this).Append(BSString::Transforms::UIntToStr(value)); }
			inline BSString operator+(DOUBLE value) const { return BSString(*this).Append(BSString::Transforms::FloatToStr(value)); }
			inline LPSTR operator*(VOID) { return m_data ? m_data : nullptr; }
			inline LPCSTR operator*(VOID) const { return Get(); }
			inline BOOL operator==(LPCSTR string) const { return !Compare(string); }
			inline BOOL operator==(const std::string& string) const { return !Compare(string); }
			inline BOOL operator==(const BSString& string) const { return !Compare(string); }
			inline BOOL operator!=(LPCSTR string) const { return Compare(string); }
			inline BOOL operator!=(const std::string& string) const { return Compare(string); }
			inline BOOL operator!=(const BSString& string) const { return Compare(string); }
		private:
			LPSTR	m_data;		// 00
			WORD	m_dataLen;	// 08
			WORD	m_bufLen;	// 0A
			DWORD	pad0C;		// 0C
		};
	}
}

#pragma pack(pop)