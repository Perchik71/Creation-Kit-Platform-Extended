// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

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
			BSString();
			BSString(LPCSTR string, WORD size = 0);
			BSString(const std::string& string, WORD size = 0);
			BSString(const BSString& string, WORD size);
			BSString(const BSString& string);
			~BSString();
		public:
			BOOL Reserved(WORD size);
			BOOL Set(LPCSTR string, WORD size = 0);				// 0 to allocate automatically
			BOOL Set(const BSString& string, WORD size = 0);	// 0 to allocate automatically
			inline LPCSTR Get() const { return m_data ? m_data : ""; }
			inline LPCSTR c_str() const { return m_data ? m_data : ""; }
			inline WORD Length() const { return m_dataLen; }
			inline WORD Size() const { return m_bufLen; }
			INT Compare(LPCSTR string, BOOL ignoreCase = TRUE) const;
			inline INT Compare(const BSString& string, BOOL ignoreCase = TRUE) const { return Compare(string.c_str(), ignoreCase); }
			void Clear();
			BSString Reverse() const;
			BSString UpperCase() const;
			BSString LowerCase() const;
			static void UpperCase(LPCSTR str);
			static void LowerCase(LPCSTR str);
			BSString& Assign(LPCSTR str, WORD start = 0, WORD len = 0);
			BSString& AssignUnsafe(LPCSTR str, WORD start = 0, WORD len = 0);
			BSString& Append(LPCSTR str);
			BSString& Append(LPCSTR str, WORD len);
			BSString& Append(const BSString& string);
			BSString& Append(const BSString& string, WORD len);
			BSString& Append(CHAR ch);
			BSString& AppendFormat(LPCSTR format, ...);
			BSString& AppendFormatVa(LPCSTR format, va_list ap);
			BSString& Copy(WORD start = 0, WORD len = 0);
			BSString& Erase(WORD start = 0, WORD len = 0);
			BSString& Format(LPCSTR format, ...);
			BSString& FormatVa(LPCSTR format, va_list ap);
			BSString Trim() const;
			WORD FindLastOf(CHAR ch, WORD offset = 0) const;
			WORD FindFirstOf(CHAR ch, WORD offset = 0) const;
			WORD FindLastOf(LPCSTR chs, WORD offset = 0) const;
			WORD FindFirstOf(LPCSTR chs, WORD offset = 0) const;
			WORD Find(LPCSTR substr, EFlags flags = sfNone) const;
			inline BOOL IsEmpty() const { return !m_dataLen; }
		public:
			struct Utils {
				static BSString ExtractFilePath(const BSString& fname);
				static BSString ExtractFileName(const BSString& fname);
				static BSString ExtractFileExt(const BSString& fname);

				static BSString GetCurrentPath();
				static BSString GetFileNameModule(const BSString& mname);
				static BSString GetApplicationPath();
				static BSString GetPluginsPath();
				static BSString GetRelativeDataPath();
				static BSString GetDataPath();

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
			static BSString FormatStringVa(LPCSTR format, va_list ap) { return BSString().FormatVa(format, ap); }
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

		class BSStringEx {
		public:
			enum EResult : uint32_t {
				srNone = MAXDWORD,
			};
			enum EFlags : uint32_t {
				sfNone = 0,
				sfInsensitiveCase = 1,
			};
		public:
			BSStringEx();
			BSStringEx(const char* string, uint32_t size = 0);
			BSStringEx(const std::string& string, uint32_t size = 0);
			BSStringEx(const BSStringEx& string, uint32_t size);
			BSStringEx(const BSStringEx& string);
			~BSStringEx();
		public:
			bool Reserved(uint32_t size);
			bool Set(const char* string, uint32_t size = 0);				// 0 to allocate automatically
			bool Set(const BSStringEx& string, uint32_t size = 0);			// 0 to allocate automatically
			inline const char* Get() const { return m_data ? m_data : ""; }
			inline const char* c_str() const { return m_data ? m_data : ""; }
			inline uint32_t Length() const { return m_dataLen; }
			inline uint32_t Size() const { return m_bufLen; }
			int32_t Compare(const char* string, bool ignoreCase = true) const;
			inline int32_t Compare(const BSStringEx& string, bool ignoreCase = true) const { return Compare(string.c_str(), ignoreCase); }
			void Clear();
			BSStringEx Reverse() const;
			BSStringEx UpperCase() const;
			BSStringEx LowerCase() const;
			static void UpperCase(const char* str);
			static void LowerCase(const char* str);
			BSStringEx& Assign(const char* str, uint32_t start = 0, uint32_t len = 0);
			BSStringEx& AssignUnsafe(const char* str, uint32_t start = 0, uint32_t len = 0);
			BSStringEx& Append(const char* str);
			BSStringEx& Append(const char* str, uint32_t len);
			BSStringEx& Append(const BSStringEx& string);
			BSStringEx& Append(const BSStringEx& string, uint32_t len);
			BSStringEx& Append(char ch);
			BSStringEx& AppendFormat(const char* format, ...);
			BSStringEx& AppendFormatVa(const char* format, va_list ap);
			BSStringEx& Copy(uint32_t start = 0, uint32_t len = 0);
			BSStringEx& Erase(uint32_t start = 0, uint32_t len = 0);
			BSStringEx& Format(const char* format, ...);
			BSStringEx& FormatVa(const char* format, va_list ap);
			BSStringEx Trim() const;
			uint32_t FindLastOf(char ch, uint32_t offset = 0) const;
			uint32_t FindFirstOf(char ch, uint32_t offset = 0) const;
			uint32_t FindLastOf(const char* chs, uint32_t offset = 0) const;
			uint32_t FindFirstOf(const char* chs, uint32_t offset = 0) const;
			uint32_t Find(const char* substr, EFlags flags = sfNone) const;
			inline bool IsEmpty() const { return !m_dataLen; }
		public:
			static BSStringEx FormatString(const char* format, ...);
			static BSStringEx FormatStringVa(const char* format, va_list ap) { return BSStringEx().FormatVa(format, ap); }
		public:
			inline char& operator[](const uint32_t Pos) { return m_data[Pos]; }
			inline const char operator[](const uint32_t Pos) const { return m_data[Pos]; }
		public:
			inline BSStringEx& operator=(char ch) { char szCS[2] = { ch, 0 }; Set(szCS, 1); return *this; }
			inline BSStringEx& operator=(const char* string) { Set(string, (uint32_t)strlen(string)); return *this; }
			inline BSStringEx& operator=(const std::string& string) { Set(string, (uint32_t)string.length()); return *this; }
			inline BSStringEx& operator=(const BSStringEx& string) { Set(string, string.Length()); return *this; }
			inline BSStringEx& operator=(bool value) { Set(BSString::Transforms::BoolToStr(value).c_str()); return *this; }
			inline BSStringEx& operator=(int64_t value) { Set(BSString::Transforms::IntToStr(value).c_str()); return *this; }
			inline BSStringEx& operator=(uint64_t value) { Set(BSString::Transforms::UIntToStr(value).c_str()); return *this; }
			inline BSStringEx& operator=(double value) { Set(BSString::Transforms::FloatToStr(value).c_str()); return *this; }
			inline BSStringEx& operator+=(const char* string) { return Append(string); }
			inline BSStringEx& operator+=(const std::string& string) { return Append(string); }
			inline BSStringEx& operator+=(const BSStringEx& string) { return Append(string); }
			inline BSStringEx& operator+=(char ch) { return Append(ch); }
			inline BSStringEx& operator+=(bool value) { return Append(BSString::Transforms::BoolToStr(value).c_str()); }
			inline BSStringEx& operator+=(int64_t value) { return Append(BSString::Transforms::IntToStr(value).c_str()); }
			inline BSStringEx& operator+=(uint64_t value) { return Append(BSString::Transforms::UIntToStr(value).c_str()); }
			inline BSStringEx& operator+=(double value) { return Append(BSString::Transforms::FloatToStr(value).c_str()); }
			inline BSStringEx operator+(const char* string) const { return BSStringEx(*this).Append(string); }
			inline BSStringEx operator+(const std::string& string) const { return BSStringEx(*this).Append(string); }
			inline BSStringEx operator+(const BSStringEx& string) const { return BSStringEx(*this).Append(string.c_str()); }
			inline BSStringEx operator+(char ch) const { return BSStringEx(*this).Append(ch); }
			inline BSStringEx operator+(bool value) const { return BSStringEx(*this).Append(BSString::Transforms::BoolToStr(value).c_str()); }
			inline BSStringEx operator+(int64_t value) const { return BSStringEx(*this).Append(BSString::Transforms::IntToStr(value).c_str()); }
			inline BSStringEx operator+(uint64_t value) const { return BSStringEx(*this).Append(BSString::Transforms::UIntToStr(value).c_str()); }
			inline BSStringEx operator+(double value) const { return BSStringEx(*this).Append(BSString::Transforms::FloatToStr(value).c_str()); }
			inline char* operator*() { return m_data ? m_data : nullptr; }
			inline const char* operator*() const { return Get(); }
			inline bool operator==(const char* string) const { return !Compare(string); }
			inline bool operator==(const std::string& string) const { return !Compare(string); }
			inline bool operator==(const BSStringEx& string) const { return !Compare(string); }
			inline bool operator!=(const char* string) const { return Compare(string); }
			inline bool operator!=(const std::string& string) const { return Compare(string); }
			inline bool operator!=(const BSStringEx& string) const { return Compare(string); }
		private:
			char* m_data;		// 00
			uint32_t m_dataLen;	// 08
			uint32_t m_bufLen;	// 0C
		};
	}
}

#pragma pack(pop)