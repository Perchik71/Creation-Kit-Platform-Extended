// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <climits>
#include <string>

#pragma pack(push, 4)

namespace CKPE
{
	namespace Fallout4
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
			class BSString
			{
			public:
				enum EResult : std::uint16_t
				{
					srNone = (std::uint16_t)-1,
				};
				enum EFlags : std::uint16_t
				{
					sfNone = 0,
					sfInsensitiveCase = 1,
				};
			public:
				BSString() noexcept(true);
				BSString(const char* string, std::uint16_t size = 0) noexcept(true);
				BSString(const std::string& string, std::uint16_t size = 0) noexcept(true);
				BSString(const BSString& string, std::uint16_t size) noexcept(true);
				BSString(const BSString& string) noexcept(true);
				~BSString() noexcept(true);
			public:
				bool Reserved(std::uint16_t size) noexcept(true);
				bool Set(const char* string, std::uint16_t size = 0) noexcept(true);		// 0 to allocate automatically
				bool Set(const BSString& string, std::uint16_t size = 0) noexcept(true);	// 0 to allocate automatically
				[[nodiscard]] const char* Get() const noexcept(true);
				[[nodiscard]] inline const char* c_str() const noexcept(true) { return Get(); }
				[[nodiscard]] inline std::uint16_t Length() const noexcept(true) { return m_dataLen; }
				[[nodiscard]] inline std::uint16_t Size() const noexcept(true) { return m_bufLen; }
				[[nodiscard]] std::int32_t Compare(const char* string, bool ignoreCase = true) const noexcept(true);
				[[nodiscard]] inline std::int32_t Compare(const BSString& string, bool ignoreCase = true) const noexcept(true) { return Compare(string.c_str(), ignoreCase); }
				void Clear() noexcept(true);
				BSString Reverse() const noexcept(true);
				BSString UpperCase() const noexcept(true);
				BSString LowerCase() const noexcept(true);
				static void UpperCase(const char* str) noexcept(true);
				static void LowerCase(const char* str) noexcept(true);
				BSString& Assign(const char* str, std::uint16_t start = 0, std::uint16_t len = 0) noexcept(true);
				BSString& AssignUnsafe(const char* str, std::uint16_t start = 0, std::uint16_t len = 0) noexcept(true);
				BSString& Append(const char* str) noexcept(true);
				BSString& Append(const char* str, std::uint16_t len) noexcept(true);
				BSString& Append(const BSString& string) noexcept(true);
				BSString& Append(const BSString& string, std::uint16_t len) noexcept(true);
				BSString& Append(char ch) noexcept(true);
				BSString& AppendFormat(const char* format, ...) noexcept(true);
				BSString& AppendFormatVa(const char* format, va_list ap) noexcept(true);
				BSString& Copy(std::uint16_t start = 0, std::uint16_t len = 0) noexcept(true);
				BSString& Erase(std::uint16_t start = 0, std::uint16_t len = 0) noexcept(true);
				BSString& Format(const char* format, ...) noexcept(true);
				BSString& FormatVa(const char* format, va_list ap) noexcept(true);
				BSString Trim() const noexcept(true);
				[[nodiscard]] std::uint16_t FindLastOf(char ch, std::uint16_t offset = 0) const noexcept(true);
				[[nodiscard]] std::uint16_t FindFirstOf(char ch, std::uint16_t offset = 0) const noexcept(true);
				[[nodiscard]] std::uint16_t FindLastOf(const char* chs, std::uint16_t offset = 0) const noexcept(true);
				[[nodiscard]] std::uint16_t FindFirstOf(const char* chs, std::uint16_t offset = 0) const noexcept(true);
				[[nodiscard]] std::uint16_t Find(const char* substr, EFlags flags = sfNone) const noexcept(true);
				[[nodiscard]] inline bool IsEmpty() const noexcept(true) { return !m_dataLen; }
			public:
				struct Utils {
					static BSString ExtractFilePath(const BSString& fname) noexcept(true);
					static BSString ExtractFileName(const BSString& fname) noexcept(true);
					static BSString ExtractFileExt(const BSString& fname) noexcept(true);

					static BSString GetCurrentPath() noexcept(true);
					static BSString GetFileNameModule(const BSString& mname) noexcept(true);
					static BSString GetApplicationPath() noexcept(true);
					static BSString GetPluginsPath() noexcept(true);
					static BSString GetRelativeDataPath() noexcept(true);
					static BSString GetDataPath() noexcept(true);

					static BSString ChangeFileExt(const BSString& fname, const BSString& ext) noexcept(true);

					static bool FileExists(const BSString& fname) noexcept(true);
					static bool DirectoryExists(const BSString& fname) noexcept(true);
				};
				struct Transforms {
					static BSString BoolToStr(bool value, bool inText = true) noexcept(true);
					static BSString IntToStr(std::int64_t value) noexcept(true);
					static BSString UIntToStr(std::uint64_t value) noexcept(true);
					static BSString FloatToStr(double value) noexcept(true);

					static bool StrToBool(const BSString& value) noexcept(true);
					static std::int64_t StrToInt(const BSString& value) noexcept(true);
					static std::uint64_t StrToUInt(const BSString& value) noexcept(true);
					static double StrToFloat(const BSString& value) noexcept(true);

					static std::int64_t HexToInt(const BSString& value) noexcept(true);
					static std::uint64_t HexToUInt(const BSString& value) noexcept(true);
					static BSString IntToHex(std::int64_t value) noexcept(true);
					static BSString UIntToHex(std::uint64_t value) noexcept(true);
				};
				struct Converts {
					static BSString Utf8ToAnsi(const BSString& str) noexcept(true);
					static BSString AnsiToUtf8(const BSString& str) noexcept(true);
					static BSString WideToAnsi(const wchar_t* str) noexcept(true);
				};
			public:
				static BSString FormatString(const char* format, ...) noexcept(true);
				static BSString FormatStringVa(const char* format, va_list ap) noexcept(true) { return BSString().FormatVa(format, ap); }
			public:
				inline char& operator[](const std::uint16_t Pos) noexcept(true) { return const_cast<char*>(Get())[Pos]; }
				inline const char operator[](const std::uint16_t Pos) const noexcept(true) { return Get()[Pos]; }
			public:
				inline BSString& operator=(char ch) noexcept(true) { char szCS[2] = { ch, 0 }; Set(szCS, 1); return *this; }
				inline BSString& operator=(const char* string) noexcept(true) { Set(string, (std::uint16_t)strlen(string)); return *this; }
				inline BSString& operator=(const std::string& string) noexcept(true) { Set(string, (std::uint16_t)string.length()); return *this; }
				inline BSString& operator=(const BSString& string) noexcept(true) { Set(string, string.Length()); return *this; }
				inline BSString& operator=(bool value) noexcept(true) { Set(BSString::Transforms::BoolToStr(value)); return *this; }
				inline BSString& operator=(std::int64_t value) noexcept(true) { Set(BSString::Transforms::IntToStr(value)); return *this; }
				inline BSString& operator=(std::uint64_t value) noexcept(true) { Set(BSString::Transforms::UIntToStr(value)); return *this; }
				inline BSString& operator=(double value) noexcept(true) { Set(BSString::Transforms::FloatToStr(value)); return *this; }
				inline BSString& operator+=(const char* string) noexcept(true) { return Append(string); }
				inline BSString& operator+=(const std::string& string) noexcept(true) { return Append(string); }
				inline BSString& operator+=(const BSString& string) noexcept(true) { return Append(string); }
				inline BSString& operator+=(char ch) noexcept(true) { return Append(ch); }
				inline BSString& operator+=(bool value) noexcept(true) { return Append(BSString::Transforms::BoolToStr(value)); }
				inline BSString& operator+=(std::int64_t value) noexcept(true) { return Append(BSString::Transforms::IntToStr(value)); }
				inline BSString& operator+=(std::uint64_t value) noexcept(true) { return Append(BSString::Transforms::UIntToStr(value)); }
				inline BSString& operator+=(double value) noexcept(true) { return Append(BSString::Transforms::FloatToStr(value)); }
				inline BSString operator+(const char* string) const noexcept(true) { return BSString(*this).Append(string); }
				inline BSString operator+(const std::string& string) const noexcept(true) { return BSString(*this).Append(string); }
				inline BSString operator+(const BSString& string) const noexcept(true) { return BSString(*this).Append(string); }
				inline BSString operator+(char ch) const noexcept(true) { return BSString(*this).Append(ch); }
				inline BSString operator+(bool value) const noexcept(true) { return BSString(*this).Append(BSString::Transforms::BoolToStr(value)); }
				inline BSString operator+(std::int64_t value) const noexcept(true) { return BSString(*this).Append(BSString::Transforms::IntToStr(value)); }
				inline BSString operator+(std::uint64_t value) const noexcept(true) { return BSString(*this).Append(BSString::Transforms::UIntToStr(value)); }
				inline BSString operator+(double value) const noexcept(true) { return BSString(*this).Append(BSString::Transforms::FloatToStr(value)); }
				inline char* operator*() noexcept(true) { return const_cast<char*>(Get()); }
				inline const char* operator*() const noexcept(true) { return Get(); }
				inline bool operator==(const char* string) const noexcept(true) { return !Compare(string); }
				inline bool operator==(const std::string& string) const noexcept(true) { return !Compare(string); }
				inline bool operator==(const BSString& string) const noexcept(true) { return !Compare(string); }
				inline bool operator!=(const char* string) const noexcept(true) { return Compare(string); }
				inline bool operator!=(const std::string& string) const noexcept(true) { return Compare(string); }
				inline bool operator!=(const BSString& string) const noexcept(true) { return Compare(string); }
			private:
				union _val
				{
					struct _fixed
					{
						char			m_data[0xC];	// 00
					} fixed;
					struct _dinamic
					{
						char*			m_data;			// 00
						std::uint32_t	pad08;			// 08
					} dinamic;
				} val;
				std::uint16_t	m_dataLen;				// 0C
				std::uint16_t	m_bufLen;				// 0E
			};
			static_assert(sizeof(BSString) == 0x10);
			//static_assert(offsetof(BSString, m_dataLen) == 0xC);
		}
	}
}

#pragma pack(pop)