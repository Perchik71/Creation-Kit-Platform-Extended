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
	namespace Starfield
	{
		namespace EditorAPI
		{
			BSString::BSString() noexcept(true) :
				m_dataLen(0), m_bufLen(0)
			{
				memset(val.fixed.m_data, 0, 12);
			}

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

				if (newSize <= 12)
				{
					if (m_bufLen > 12)
					{
						char ss[12];
						memcpy_s(ss, 12, val.dinamic.m_data, newLen);
						Clear();
						memcpy_s(val.fixed.m_data, 12, ss, newLen);
					}

					m_bufLen = newSize;
					m_dataLen = newLen;
				}
				else
				{
					auto newData = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, newSize);
					if (!newData)
						return false;

					if ((m_bufLen > 12) && val.dinamic.m_data)
					{
						strncpy(newData, val.dinamic.m_data, newLen);
						NiAPI::NiMemoryManager::Free(nullptr, (void*)val.dinamic.m_data);
						newData[newLen] = 0;
					}
					else if (m_bufLen)
					{
						memcpy_s(newData, newLen, val.fixed.m_data, m_bufLen);
						newData[newLen] = 0;
					}

					val.dinamic.m_data = newData;
					val.dinamic.pad08 = 0;
					m_bufLen = newSize;
					m_dataLen = newLen;
				}

				return true;
			}

			bool BSString::Set(const char* string, std::uint16_t size) noexcept(true)
			{
				std::uint16_t wNeedLen, wNeedBuf;

				wNeedLen = (string) ? (std::uint16_t)strlen(string) : 0;
				if (size && (wNeedLen > size))
					wNeedLen = size;

				wNeedBuf = wNeedLen + 1;

				if (wNeedBuf <= 12)
				{
					Clear();

					if (wNeedLen)
					{
						memcpy_s(val.fixed.m_data, 12, string, wNeedLen);
						val.fixed.m_data[wNeedLen] = 0;
						m_bufLen = wNeedBuf;
						m_dataLen = wNeedLen;
					}
				}
				else
				{
					if (!IsEmpty())
						Clear();

					char* newData = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, wNeedBuf, 4);
					if (!newData) return false;
					memcpy(newData, string, wNeedLen);
					newData[wNeedLen] = 0;

					std::uint16_t wRealBuf = (std::uint16_t)NiAPI::NiMemoryManager::Size(nullptr, newData);

					val.dinamic.m_data = newData;
					m_bufLen = wRealBuf > wNeedBuf ? wRealBuf : wNeedBuf;
					m_dataLen = wNeedLen;
				}

				return true;
			}

			bool BSString::Set(const BSString& string, std::uint16_t size) noexcept(true)
			{
				if (string.IsEmpty())
				{
					Clear();
					return true;
				}

				return Set(string.Get(), size);
			}

			const char* BSString::Get() const noexcept(true)
			{ 
				if (IsEmpty()) return "";
				if (m_bufLen > 12) return val.dinamic.m_data ? val.dinamic.m_data : "";
				return val.fixed.m_data;
			}

			void BSString::Clear() noexcept(true)
			{
				if (m_bufLen > 12)
				{
					NiAPI::NiMemoryManager::Free(nullptr, (void*)val.dinamic.m_data);
					val.dinamic.m_data = nullptr;
					val.dinamic.pad08 = 0;	
				}
				else
					memset(val.fixed.m_data, 0, 12);

				m_dataLen = 0;
				m_bufLen = 0;
			}

			BSString BSString::Reverse() const noexcept(true)
			{
				if (!m_dataLen)
					return "";

				auto s = BSString(Get());
				return _strrev(*s);
			}

			BSString& BSString::Format(const char* format, ...) noexcept(true)
			{
				Clear();

				va_list va;
				va_start(va, format);
				auto size = _vsnprintf(nullptr, 0, format, va);
				if (size > 12) 
				{
					m_bufLen = size + 1;
					m_dataLen = size;
					val.dinamic.m_data = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, m_bufLen);
					if (!val.dinamic.m_data)
					{
						m_bufLen = 0;
						m_dataLen = 0;
					}
					else
					{
						vsprintf(val.dinamic.m_data, format, va);
						val.dinamic.m_data[size] = 0;
					}
				}
				else
				{
					m_bufLen = size + 1;
					m_dataLen = size;

					vsprintf_s(val.fixed.m_data, format, va);
					val.dinamic.m_data[m_dataLen] = 0;
				}
				va_end(va);

				return *this;
			}

			BSString& BSString::FormatVa(const char* format, va_list ap) noexcept(true)
			{
				auto size = _vsnprintf(nullptr, 0, format, ap);
				if (size > 12)
				{
					m_bufLen = size + 1;
					m_dataLen = size;
					val.dinamic.m_data = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, m_bufLen);
					if (!val.dinamic.m_data)
					{
						m_bufLen = 0;
						m_dataLen = 0;
					}
					else
					{
						vsprintf(val.dinamic.m_data, format, ap);
						val.dinamic.m_data[size] = 0;
					}
				}
				else
				{
					m_bufLen = size + 1;
					m_dataLen = size;

					vsprintf_s(val.fixed.m_data, format, ap);
					val.dinamic.m_data[m_dataLen] = 0;
				}

				return *this;
			}

			std::int32_t BSString::Compare(const char* string, bool ignoreCase) const noexcept(true)
			{
				if (ignoreCase)
					return _stricmp(Get(), string);
				else
					return strcmp(Get(), string);
			}

			BSString& BSString::Append(const char* str) noexcept(true)
			{
				if (str) 
				{
					std::uint16_t dwLen = (std::uint16_t)strlen(str);
					if (dwLen) 
					{
						if (m_dataLen) 
						{
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(const_cast<char*>(Get()), str);
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
									strcat(const_cast<char*>(Get()), str);
							}
							else
							{
								if (Reserved(m_dataLen + len + 1))
									strncat(const_cast<char*>(Get()), str, len);
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
								strcat(const_cast<char*>(Get()), string.Get());
						}
						else
							Set(string.Get());
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
									strcat(const_cast<char*>(Get()), string.Get());
							}
							else
							{
								if (Reserved(m_dataLen + len + 1))
									strncat(const_cast<char*>(Get()), string.Get(), len);
							}
						}
						else
							Set(string.Get(), len);
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
				return Assign(Get(), start, len);
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

				if (!start) 
				{
					if (len >= m_dataLen)
						Clear();
					else
						return Assign(Get(), len);
				}
				else 
				{
					auto s = Get();
					if (len >= m_dataLen)
						return Assign(s, 0, start);
					else
						return Assign(s, 0, start).Append(s + (start + len));
				}

				return *this;
			}

			std::uint16_t BSString::FindLastOf(char ch, std::uint16_t offset) const noexcept(true)
			{
				if (IsEmpty())
					return srNone;

				auto s = const_cast<char*>(Get());
				char* Ret = strrchr(s + offset, ch);
				return Ret ? (std::uint16_t)(Ret - s) : srNone;
			}

			std::uint16_t BSString::FindFirstOf(char ch, std::uint16_t offset) const noexcept(true)
			{
				if (IsEmpty())
					return srNone;

				auto s = const_cast<char*>(Get());
				char* Ret = strchr(s + offset, ch);
				return Ret ? (std::uint16_t)(Ret - s) : srNone;
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

				auto s = const_cast<char*>(Get());
				char* Ret = strpbrk(s + offset, chs);
				return Ret ? (std::uint16_t)(Ret - s) : srNone;
			}

			BSString BSString::UpperCase() const noexcept(true)
			{
				if (IsEmpty())
					return "";

				auto s = BSString(Get());
				UpperCase(*s);
				return s;
			}

			BSString BSString::LowerCase() const noexcept(true)
			{
				if (IsEmpty())
					return "";

				auto s = BSString(Get());
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
				auto s = const_cast<char*>(Get());
				if (flags == sfInsensitiveCase)
					Ret = strstr(*LowerCase(), *(BSString(substr).LowerCase()));
				else
					char* Ret = strstr(s, substr);

				return Ret ? (std::uint16_t)(Ret - s) : srNone;
			}

			BSString BSString::Trim() const noexcept(true)
			{
				if (IsEmpty())
					return "";

				return StringUtils::Trim(Get()).c_str();
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