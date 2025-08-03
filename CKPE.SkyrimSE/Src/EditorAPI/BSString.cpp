// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>

#include <EditorAPI/BSString.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

#include <cstdarg>
#include <sstream>
#include <iomanip>

#define WDELIMITERS " \t\n\r\f\v"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			BSString::BSString() noexcept(true) :
				m_data(nullptr), m_dataLen(0), m_bufLen(0)
			{}

			BSString::BSString(const char* string, std::uint16_t size) noexcept(true) : BSString()
			{
				Set(string ? string : "", size);
			}

			BSString::BSString(const std::string& string, std::uint16_t size) noexcept(true) : BSString()
			{
				Set(string.empty() ? "" : string.c_str(), size);
			}

			BSString::BSString(const BSString& string, std::uint16_t size) noexcept(true) : BSString()
			{
				Set(string, size);
			}

			BSString::BSString(const BSString& string) noexcept(true) : BSString()
			{
				Set(string, string.m_bufLen);
			}

			BSString::~BSString() noexcept(true)
			{
				Clear();
			}

			bool BSString::Reserved(std::uint16_t size) noexcept(true)
			{
				std::uint16_t newLen = size;
				std::uint16_t newSize = newLen + 1;
				auto newData = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, newSize);
				if (!newData)
					return false;

				if (m_bufLen)
				{
					strncpy(newData, m_data, newLen);
					NiAPI::NiMemoryManager::Free(nullptr, (void*)m_data);
					newData[newLen] = 0;
				}
				else
					newData[0] = 0;

				m_data = newData;
				m_bufLen = newSize;
				m_dataLen = newLen;

				return true;
			}

			bool BSString::Set(const char* string, std::uint16_t size) noexcept(true)
			{
				pad0C = 0;

				std::uint16_t wNeedLen, wNeedBuf;

				wNeedLen = (string) ? (std::uint16_t)strlen(string) : 0;
				if (size && (wNeedLen > size))
					wNeedLen = size;

				wNeedBuf = wNeedLen + 1;

				if (wNeedBuf > m_bufLen) 
				{
					if (!wNeedLen)
						goto __dealloc;
					else {
						char* newData = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, wNeedBuf, 4);
						memcpy(newData, string, wNeedLen);
						newData[wNeedLen] = 0;

						if (m_data)
							NiAPI::NiMemoryManager::Free(nullptr, (void*)m_data);

						std::uint16_t wRealBuf = (std::uint16_t)NiAPI::NiMemoryManager::Size(nullptr, newData);

						m_data = newData;
						m_bufLen = wRealBuf > wNeedBuf ? wRealBuf : wNeedBuf;
						m_dataLen = wNeedLen;
					}
				}
				else 
				{
					if (!string)
					{
					__dealloc:
						if (m_data)
						{
							NiAPI::NiMemoryManager::Free(nullptr, (void*)m_data);

							m_data = nullptr;
							m_bufLen = 0;
							m_dataLen = 0;
						}
					}
					else 
					{
						memcpy(m_data, string, wNeedLen);
						m_dataLen = wNeedLen;
						m_data[wNeedLen] = 0;
					}
				}

				return true;
			}

			bool BSString::Set(const BSString& string, std::uint16_t size) noexcept(true)
			{
				return Set(string.m_data ? *string : "", size);
			}

			void BSString::Clear() noexcept(true)
			{
				if (m_data) 
				{
					NiAPI::NiMemoryManager::Free(nullptr, (void*)m_data);
					m_data = nullptr;
					m_dataLen = 0;
					m_bufLen = 0;
				}
			}

			BSString BSString::Reverse() const noexcept(true)
			{
				if (!m_dataLen)
					return "";

				auto s = BSString(m_data);
				return _strrev(*s);
			}

			BSString& BSString::Format(const char* format, ...) noexcept(true)
			{
				Clear();

				va_list va;
				va_start(va, format);
				auto size = _vsnprintf(nullptr, 0, format, va);
				if (size) {
					m_bufLen = size + 1;
					m_dataLen = size;
					m_data = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, m_bufLen);
					if (!m_data) 
					{
						m_bufLen = 0;
						m_dataLen = 0;
					}
					else
					{
						vsprintf(m_data, format, va);
						m_data[size] = 0;
					}
				}
				va_end(va);

				return *this;
			}

			BSString& BSString::FormatVa(const char* format, va_list ap) noexcept(true)
			{
				auto size = _vsnprintf(nullptr, 0, format, ap);
				if (size) {
					m_bufLen = size + 1;
					m_dataLen = size;
					m_data = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, m_bufLen);
					if (!m_data)
					{
						m_bufLen = 0;
						m_dataLen = 0;
					}
					else 
					{
						vsprintf(m_data, format, ap);
						m_data[size] = 0;
					}
				}

				return *this;
			}

			std::int32_t BSString::Compare(const char* string, bool ignoreCase) const noexcept(true)
			{
				if (ignoreCase)
					return _stricmp(m_data, string);
				else
					return strcmp(m_data, string);
			}

			BSString& BSString::Append(const char* str) noexcept(true)
			{
				if (str) {
					std::uint16_t dwLen = (std::uint16_t)strlen(str);
					if (dwLen) {
						if (m_dataLen) {
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(m_data, str);
						}
						else
							Set(str);
					}
				}

				return *this;
			}

			BSString& BSString::Append(const char* str, std::uint16_t len) noexcept(true)
			{
				if (str)
				{
					std::uint16_t dwLen = (std::uint16_t)strlen(str);
					if (dwLen)
					{
						if (m_dataLen)
						{
							if (dwLen <= len)
							{
								if (Reserved(m_dataLen + dwLen + 1))
									strcat(m_data, str);
							}
							else
							{
								if (Reserved(m_dataLen + len + 1))
									strncat(m_data, str, len);
							}
						}
						else
							Set(str, len);
					}
				}

				return *this;
			}

			BSString& BSString::Append(const BSString& string) noexcept(true)
			{
				if (!string.IsEmpty())
				{
					std::uint16_t dwLen = (std::uint16_t)string.m_dataLen;
					if (dwLen)
					{
						if (m_dataLen)
						{
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(m_data, string.m_data);
						}
						else
							Set(string.m_data);
					}
				}

				return *this;
			}

			BSString& BSString::Append(const BSString& string, std::uint16_t len) noexcept(true)
			{
				if (!string.IsEmpty())
				{
					std::uint16_t dwLen = (std::uint16_t)string.m_dataLen;
					if (dwLen)
					{
						if (m_dataLen)
						{
							if (dwLen <= len)
							{
								if (Reserved(m_dataLen + dwLen + 1))
									strcat(m_data, string.m_data);
							}
							else
							{
								if (Reserved(m_dataLen + len + 1))
									strncat(m_data, string.m_data, len);
							}
						}
						else
							Set(string.m_data, len);
					}
				}

				return *this;
			}

			BSString& BSString::Append(char ch) noexcept(true)
			{
				char Buf[2] = { ch, 0 };
				return Append(Buf);
			}

			BSString& BSString::AppendFormat(const char* format, ...) noexcept(true)
			{
				BSString fmt;
				va_list va;
				va_start(va, format);
				fmt.FormatVa(format, va);
				va_end(va);

				return fmt.IsEmpty() ? *this : Append(fmt);
			}

			BSString& BSString::AppendFormatVa(const char* format, va_list ap) noexcept(true)
			{
				BSString fmt;
				fmt.FormatVa(format, ap);
				return fmt.IsEmpty() ? *this : Append(fmt);
			}

			BSString& BSString::Copy(std::uint16_t start, std::uint16_t len) noexcept(true)
			{
				return Assign(m_data, start, len);
			}

			BSString& BSString::Assign(const char* str, std::uint16_t start, std::uint16_t len) noexcept(true)
			{
				if (str) {
					if (!len)
						len = (std::uint16_t)strlen(str);

					Set(*BSString(str + start), len);
				}

				return *this;
			}

			BSString& BSString::AssignUnsafe(const char* str, std::uint16_t start, std::uint16_t len) noexcept(true)
			{
				if (str) {
					if (!len)
						len = (std::uint16_t)strlen(str);

					Set(str + start, len);
				}

				return *this;
			}

			BSString& BSString::Erase(std::uint16_t start, std::uint16_t len) noexcept(true)
			{
				if ((!start && !len) || (start >= len))
					return *this;

				if (!start) {
					if (len >= m_dataLen)
						Clear();
					else
						return Assign(m_data, len);
				}
				else {
					if (len >= m_dataLen)
						return Assign(m_data, 0, start);
					else
						return Assign(m_data, 0, start).Append(m_data + (start + len));
				}

				return *this;
			}

			std::uint16_t BSString::FindLastOf(char ch, std::uint16_t offset) const noexcept(true)
			{
				if (!m_data)
					return srNone;

				char* Ret = strrchr(m_data + offset, ch);
				return Ret ? (std::uint16_t)(Ret - m_data) : srNone;
			}

			std::uint16_t BSString::FindFirstOf(char ch, std::uint16_t offset) const noexcept(true)
			{
				if (IsEmpty())
					return srNone;

				char* Ret = strchr(m_data + offset, ch);
				return Ret ? (std::uint16_t)(Ret - m_data) : srNone;
			}

			std::uint16_t BSString::FindLastOf(const char* chs, std::uint16_t offset) const noexcept(true)
			{
				std::uint16_t Ret = Reverse().FindFirstOf(chs, offset);
				return Ret == srNone ? srNone : m_dataLen - Ret;
			}

			std::uint16_t BSString::FindFirstOf(const char* chs, std::uint16_t offset) const noexcept(true)
			{
				if (IsEmpty() || !chs)
					return srNone;

				char* Ret = strpbrk(m_data + offset, chs);
				return Ret ? (std::uint16_t)(Ret - m_data) : srNone;
			}

			BSString BSString::UpperCase() const noexcept(true)
			{
				if (IsEmpty())
					return "";

				auto s = BSString(m_data);
				UpperCase(*s);
				return s;
			}

			BSString BSString::LowerCase() const noexcept(true)
			{
				if (IsEmpty())
					return "";

				auto s = BSString(m_data);
				LowerCase(*s);
				return s;
			}

			void BSString::UpperCase(const char* str) noexcept(true)
			{
				if (!str)
					return;

				_strupr(const_cast<char*>(str));
			}

			void BSString::LowerCase(const char* str) noexcept(true)
			{
				if (!str)
					return;

				_strlwr(const_cast<char*>(str));
			}

			std::uint16_t BSString::Find(const char* substr, EFlags flags) const noexcept(true)
			{
				if (IsEmpty() || !substr)
					return srNone;

				char* Ret = nullptr;
				if (flags == sfInsensitiveCase)
					Ret = strstr(*LowerCase(), *(BSString(substr).LowerCase()));
				else
					char* Ret = strstr(m_data, substr);

				return Ret ? (std::uint16_t)(Ret - m_data) : srNone;
			}

			BSString BSString::Trim() const noexcept(true)
			{
				if (IsEmpty())
					return "";

				return StringUtils::Trim(m_data).c_str();
			}

			BSString BSString::FormatString(const char* format, ...) noexcept(true)
			{
				BSString fmt;
				va_list va;

				va_start(va, format);
				fmt.FormatVa(format, va);
				va_end(va);

				return fmt;
			}

			BSString BSString::Utils::ExtractFilePath(const BSString& fname) noexcept(true)
			{
				std::uint16_t it = fname.FindLastOf("/\\");

				if (it != srNone)
					return BSString(*fname, it);

				return fname;
			}

			BSString BSString::Utils::ExtractFileName(const BSString& fname) noexcept(true)
			{
				std::uint16_t it = fname.FindLastOf("/\\");

				if (it != srNone)
					return BSString(*fname + it);

				return fname;
			}

			BSString BSString::Utils::ExtractFileExt(const BSString& fname) noexcept(true)
			{
				std::uint16_t it = fname.FindLastOf('.');

				if (it != srNone)
					return BSString(*fname + it);

				return "";
			}

			BSString BSString::Utils::GetCurrentPath() noexcept(true)
			{
				constexpr auto SIZE = 1024;
				char szTemp[SIZE];

				if (::GetCurrentDirectoryA(SIZE, szTemp))
					return BSString(szTemp) + "\\";

				return "";
			}

			BSString BSString::Utils::GetFileNameModule(const BSString& mname) noexcept(true)
			{
				constexpr auto SIZE = 1024;
				char szTemp[SIZE];

				if (::GetModuleFileNameA(GetModuleHandleA(mname.IsEmpty() ? nullptr : *mname), szTemp, MAX_PATH))
					return szTemp;

				return "";
			}

			BSString BSString::Utils::GetApplicationPath() noexcept(true)
			{
				return ExtractFilePath(GetFileNameModule(""));
			}

			BSString BSString::Utils::GetPluginsPath() noexcept(true)
			{
				return GetApplicationPath() + "CKPEPlugins\\";
			}

			BSString BSString::Utils::ChangeFileExt(const BSString& fname, const BSString& ext) noexcept(true)
			{
				std::uint16_t it = fname.FindLastOf('.');

				if (it != srNone)
					return BSString(*fname, it).Append(ext);

				return BSString(fname).Append(ext);
			}

			BSString BSString::Utils::GetRelativeDataPath() noexcept(true)
			{
				return "Data\\";
			}

			BSString BSString::Utils::GetDataPath() noexcept(true)
			{
				return GetApplicationPath() + "Data\\";
			}

			bool BSString::Utils::FileExists(const BSString& fname) noexcept(true)
			{
				return PathUtils::FileExists(*fname);
			}

			bool BSString::Utils::DirectoryExists(const BSString& fname) noexcept(true)
			{
				return PathUtils::DirExists(*fname);
			}

			BSString BSString::Transforms::BoolToStr(bool value, bool inText) noexcept(true)
			{
				if (inText)
					return (value) ? "True" : "False";
				else
					return (value) ? "1" : "0";
			}

			BSString BSString::Transforms::IntToStr(std::int64_t value) noexcept(true)
			{
				char szBuffer[128];
				return _i64toa(value, szBuffer, 10);
			}

			BSString BSString::Transforms::UIntToStr(std::uint64_t value) noexcept(true)
			{
				char szBuffer[128];
				return _ui64toa(value, szBuffer, 10);
			}

			BSString BSString::Transforms::FloatToStr(double value) noexcept(true)
			{
				return std::to_string(value);
			}

			bool BSString::Transforms::StrToBool(const BSString& value) noexcept(true)
			{
				if (*value && ((value == "1") || (value == "True")))
					return true;

				return false;
			}

			std::int64_t BSString::Transforms::StrToInt(const BSString& value) noexcept(true)
			{
				char* lpEnd = nullptr;
				return strtoll(*value, &lpEnd, 10);
			}

			std::uint64_t BSString::Transforms::StrToUInt(const BSString& value) noexcept(true)
			{
				char* lpEnd = nullptr;
				return strtoull(*value, &lpEnd, 10);
			}

			double BSString::Transforms::StrToFloat(const BSString& value) noexcept(true)
			{
				char* lpEnd = nullptr;
				return strtod(*value, &lpEnd);
			}

			std::int64_t BSString::Transforms::HexToInt(const BSString& value) noexcept(true)
			{
				char* lpEnd = nullptr;
				return strtoll(*value, &lpEnd, 16);
			}

			std::uint64_t BSString::Transforms::HexToUInt(const BSString& value) noexcept(true)
			{
				char* lpEnd = nullptr;
				return strtoull(*value, &lpEnd, 16);
			}

			BSString BSString::Transforms::IntToHex(std::int64_t value) noexcept(true)
			{
				std::stringstream stream;
				stream << "0x"
					<< std::setfill('0') << std::setw(sizeof(std::int64_t) * 2)
					<< std::hex << value;
				return stream.str();
			}

			BSString BSString::Transforms::UIntToHex(std::uint64_t value) noexcept(true)
			{
				std::stringstream stream;
				stream << "0x"
					<< std::setfill('0') << std::setw(sizeof(std::uint64_t) * 2)
					<< std::hex << value;
				return stream.str();
			}

			BSString BSString::Converts::Utf8ToAnsi(const BSString& str) noexcept(true)
			{
				if (StringUtils::IsUtf8(*str))
					return StringUtils::Utf8ToWinCP(*str).c_str();
				else
					return str;
			}

			BSString BSString::Converts::AnsiToUtf8(const BSString& str) noexcept(true)
			{
				if (!StringUtils::IsUtf8(*str))
					return StringUtils::WinCPToUtf8(*str).c_str();
				else
					return str;
			}

			BSString BSString::Converts::WideToAnsi(const wchar_t* str) noexcept(true)
			{
				return StringUtils::Utf16ToWinCP(str).c_str();
			}
		}
	}
}