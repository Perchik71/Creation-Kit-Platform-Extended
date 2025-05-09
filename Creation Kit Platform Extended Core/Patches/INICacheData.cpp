// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BSString.h"
#include "INICacheData.h"

#include <intrin.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		uintptr_t pointer_SettingCollectionList_ReadSetting_Read_code = 0;

		struct string_equal_to
		{
			inline bool operator()(const String& lhs, const String& rhs) const
			{
				return !_stricmp(lhs.c_str(), rhs.c_str());
			}
		};
#if 0
		HANDLE GlobalINICacheTriggerEvent = NULL;
#endif
		ConcurrencyMap<String, std::shared_ptr<INISettingCollection>, std::hash<String>, string_equal_to> GlobalINICache;

		INICacheDataPatch::INICacheDataPatch() : Module(GlobalEnginePtr)
		{
#if 0
			if (GetShortExecutableTypeFromFull(GlobalEnginePtr->GetEditorVersion()) == EDITOR_SHORT_STARFIELD)
				GlobalINICacheTriggerEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
#endif
		}

		INICacheDataPatch::~INICacheDataPatch()
		{
			ClearAndFlush();
		}

		bool INICacheDataPatch::HasOption() const
		{
			return true;
		}

		bool INICacheDataPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* INICacheDataPatch::GetOptionName() const
		{
			return "CreationKit:bINICache";
		}

		const char* INICacheDataPatch::GetName() const
		{
			return "INI Cache Data";
		}

		bool INICacheDataPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> INICacheDataPatch::GetDependencies() const
		{
			return {};
		}

		void INICacheDataPatch::ClearAndFlush()
		{
#if 0
			if (GetShortExecutableTypeFromFull(GlobalEnginePtr->GetEditorVersion()) == EDITOR_SHORT_STARFIELD)
				SetEvent(GlobalINICacheTriggerEvent);
			else
			{
#endif
				for (auto it = GlobalINICache.begin(); it != GlobalINICache.end(); it++)
				{
					if (it->second && !it->second->IsEmpty())
						it->second->Save();
				}

				GlobalINICache.clear();
#if 0
			}
#endif
		}

		UINT INICacheDataPatch::HKGetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName)
		{
			SetLastError(0);

			if (!lpKeyName || !lpAppName)
				return (UINT)nDefault;

			auto fileName = GetAbsoluteFileName(lpFileName);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(fileName);
			if (!ini_data)
				return GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpFileName);

			// Так как это единственная функция без защиты на тип опции, юзаю только это...
			auto s = ini_data->ReadString(lpAppName, lpKeyName, "\"");
			if (s == "\"")
				return (UINT)nDefault;

			if (s.find_first_of("0x") == 0)
				// hex
				return strtoul(s.c_str() + 2, nullptr, 16);
			else
				// dec
				return strtoul(s.c_str(), nullptr, 10);
		}

		DWORD INICacheDataPatch::HKGetPrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault,
			LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName)
		{
			SetLastError(0);

			if (!lpReturnedString || !nSize)
				return 0;

			auto fileName = GetAbsoluteFileName(lpFileName);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(fileName);
			if (!ini_data || !ini_data->IsOpen())
				return GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);

			String s;
			size_t l = 0;

			if (lpAppName && !lpKeyName)
			{
				auto ip = ini_data->ReadSection(lpAppName);

				for (auto& i : ip->second)
				{
					s.append(i.first).append("=").append(i.second).append("\n");
					l = std::min((size_t)nSize, s.length());
					if (l == nSize) break;
				}

				memcpy(lpReturnedString, (const void*)s.c_str(), l);
			}
			else if (!lpAppName)
			{
				for (auto j = ini_data->FirstUnsafe(); j != ini_data->LastUnsafe(); j++)
				{
					s.append("[").append(j->first).append("]\n");

					for (auto i = j->second.begin(); i != j->second.end(); i++)
					{
						s.append(i->first).append("=").append(i->second).append("\n");
						l = std::min((size_t)nSize, s.length());
						if (l == nSize) break;
					}
				}

				memcpy(lpReturnedString, (const void*)s.c_str(), l);
			}
			else
			{
				auto s = ini_data->ReadString(lpAppName, lpKeyName, "\"");
				if (s == "\"")
					s = lpDefault ? Utils::QuoteRemove(lpDefault) : "";

				l = std::min((size_t)nSize, s.length());
				memcpy(lpReturnedString, (const void*)s.c_str(), l);
			}

			lpReturnedString[(l == nSize) ? l - 1 : l] = '\0';
			return (DWORD)l;
		}

		BOOL INICacheDataPatch::HKGetPrivateProfileStructA(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,
			UINT uSizeStruct, LPCSTR szFile)
		{
			SetLastError(0);

			if (!lpszSection || !szFile || (uSizeStruct >= (UINT)0x7FFFFFFA)) return false;

			auto fileName = GetAbsoluteFileName(szFile);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(fileName);
			if (!ini_data)
				return GetPrivateProfileStructA(lpszSection, lpszKey, lpStruct, uSizeStruct, szFile);

			if (!ini_data->Has(lpszSection, lpszKey))
				return false;

			String value_str = ini_data->ReadString(lpszSection, lpszKey, "");
			UINT count = (uSizeStruct << 1) + 2;

			if (value_str.length() != count)
				return false;
			else
			{
				DWORD dec_val[2] = { 0 }, temp = 0;
				count = uSizeStruct;
				auto data = value_str.c_str();

				do
				{
					dec_val[0] = (DWORD)data[0];
					temp = dec_val[0] - 0x30;
					if (temp > 9) dec_val[0] -= 7;
					dec_val[1] = (DWORD)data[1];
					temp = dec_val[1] - 0x30;
					if (temp > 9)
					{
						dec_val[1] -= 7;
						dec_val[1] &= 0xF;
					}
					else dec_val[1] -= 0x30;
					((PBYTE)lpStruct)[uSizeStruct - count] = (BYTE)((dec_val[0] << 4) | dec_val[1]);
					data += 2;
					count--;
				} while (count);
			}

			return true;
		}

		BOOL INICacheDataPatch::HKWritePrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName)
		{
			SetLastError(0);

			if (!lpAppName || !lpFileName) return false;

			auto fileName = GetAbsoluteFileName(lpFileName);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(fileName);
			if (!ini_data || (ini_data == INVALID_HANDLE_VALUE) || !ini_data->IsOpen())
				return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);

			if (!lpKeyName)
				// The name of the key to be associated with a string.
				// If the key does not exist in the specified section, it is created.
				// If this parameter is NULL, the entire section, including all entries within the section, is deleted.
				ini_data->Remove(lpAppName);
			else if (!lpString)
				// A null - terminated string to be written to the file.
				// If this parameter is NULL, the key pointed to by the key_name parameter is deleted.
				ini_data->Remove(lpAppName, lpKeyName);
			else
				ini_data->WriteString(lpAppName, lpKeyName, lpString);

#ifdef _CKPE_WITH_QT5
			if (!ini_data->IsEmpty())
				ini_data->Save();
			else
				return false;
#else
			return true;
#endif
		}

		BOOL INICacheDataPatch::HKWritePrivateProfileStructA(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,
			UINT uSizeStruct, LPCSTR szFile)
		{
			SetLastError(0);

			if (!lpStruct)
				return HKWritePrivateProfileStringA(lpszSection, lpszKey, nullptr, szFile);

			if (!lpszSection || !szFile || (uSizeStruct >= (UINT)0x7FFFFFFA)) return false;

			auto fileName = GetAbsoluteFileName(szFile);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(fileName);
			if (!ini_data || (ini_data == INVALID_HANDLE_VALUE)) 
				return WritePrivateProfileStructA(lpszSection, lpszKey, lpStruct, uSizeStruct, szFile);

			static const char* ffmt_value = "0123456789ABCDEF\\";
			
			DWORD dec_val[2] = { 0 }, temp = 0, unk = 0;
			UINT count = uSizeStruct;

			std::string value_str;
			value_str.resize(((size_t)count << 1) + 2);
			auto data = value_str.data();

			if (count)
			{
				do
				{
					temp = ((PBYTE)lpStruct)[uSizeStruct - count];
					unk = (BYTE)(temp + unk);
					dec_val[0] = temp >> 4;
					dec_val[1] = temp & 0xF;

					data[0] = ffmt_value[dec_val[0]];
					data[1] = ffmt_value[dec_val[1]];

					data += 2;
					count--;
				} while (count);
			}

			dec_val[0] = unk >> 4;
			dec_val[1] = unk & 0xF;
			data[0] = ffmt_value[dec_val[0]];
			data[1] = ffmt_value[dec_val[1]];

			ini_data->WriteString(lpszSection, lpszKey, value_str.c_str());

#ifdef _CKPE_WITH_QT5
			if (!ini_data->IsEmpty())
				ini_data->Save();
			else
				return false;
#else
			return true;
#endif
		}

		UINT INICacheDataPatch::HKGetPrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, INT nDefault, LPCWSTR lpFileName)
		{
			SetLastError(0);

			if (!lpKeyName || !lpAppName)
				return (UINT)nDefault;

			auto fileName = GetAbsoluteFileNameUnicode(lpFileName);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(Conversion::WideToAnsi(fileName.c_str()).c_str());
			if (!ini_data || (ini_data == INVALID_HANDLE_VALUE))
				return GetPrivateProfileIntW(lpAppName, lpKeyName, nDefault, lpFileName);

			// Так как это единственная функция без защиты на тип опции, юзаю только это...
			auto s = ini_data->ReadString(Conversion::WideToAnsi(lpAppName).c_str(), Conversion::WideToAnsi(lpKeyName).c_str(), "\"");
			if (s == "\"")
				return (UINT)nDefault;

			if (s.find_first_of("0x") == 0)
				// hex
				return strtoul(s.c_str() + 2, nullptr, 16);
			else
				// dec
				return strtoul(s.c_str(), nullptr, 10);
		}

		DWORD INICacheDataPatch::HKGetPrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault,
			LPWSTR lpReturnedString, DWORD nSize, LPCWSTR lpFileName)
		{
			SetLastError(0);

			if (!lpReturnedString || !nSize)
				return 0;

			auto fileName = GetAbsoluteFileNameUnicode(lpFileName);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(Conversion::WideToAnsi(fileName.c_str()).c_str());
			if (!ini_data || (ini_data == INVALID_HANDLE_VALUE) || !ini_data->IsOpen())
				return GetPrivateProfileStringW(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);

			String s;
			size_t l = 0;

			if (lpAppName && !lpKeyName)
			{
				auto ip = ini_data->ReadSection(Conversion::WideToAnsi(lpAppName).c_str());

				for (auto i = ip->second.begin(); i != ip->second.end(); i++)
				{
					s.append(i->first).append("=").append(i->second).append("\n");
					l = std::min((size_t)nSize, s.length());
					if (l == nSize) break;
				}

				memcpy(lpReturnedString, (const void*)Conversion::Utf8ToUtf16(s.c_str()).c_str(), l << 1);
			}
			else if (!lpAppName)
			{
				for (auto j = ini_data->FirstUnsafe(); j != ini_data->LastUnsafe(); j++)
				{
					s.append("[").append(j->first).append("]\n");

					for (auto i = j->second.begin(); i != j->second.end(); i++)
					{
						s.append(i->first).append("=").append(i->second).append("\n");
						l = std::min((size_t)nSize, s.length());
						if (l == nSize) break;
					}
				}

				memcpy(lpReturnedString, (const void*)Conversion::Utf8ToUtf16(s.c_str()).c_str(), l << 1);
			}
			else
			{
				auto s = ini_data->ReadString(Conversion::WideToAnsi(lpAppName).c_str(), Conversion::WideToAnsi(lpKeyName).c_str(), "\"");
				if (s == "\"")
					s = lpDefault ? Conversion::Utf16ToUtf8(Utils::QuoteRemove(lpDefault)) : "";

				l = std::min((size_t)nSize, s.length());
				memcpy(lpReturnedString, (const void*)Conversion::Utf8ToUtf16(s.c_str()).c_str(), l << 1);
			}

			lpReturnedString[(l == nSize) ? l - 1 : l] = L'\0';
			return (DWORD)l;
		}

		BOOL INICacheDataPatch::HKWritePrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName)
		{
			SetLastError(0);

			if (!lpAppName || !lpFileName) return false;

			auto fileName = GetAbsoluteFileNameUnicode(lpFileName);
			auto ini_data = (INISettingCollection*)GetFileFromCacheOrOpen(Conversion::WideToAnsi(fileName.c_str()).c_str());
			if (!ini_data || (ini_data == INVALID_HANDLE_VALUE))
				return WritePrivateProfileStringW(lpAppName, lpKeyName, lpString, lpFileName);

			if (!lpKeyName)
				// The name of the key to be associated with a string.
				// If the key does not exist in the specified section, it is created.
				// If this parameter is NULL, the entire section, including all entries within the section, is deleted.
				ini_data->Remove(Conversion::WideToAnsi(lpAppName).c_str());
			else if (!lpString)
				// A null - terminated string to be written to the file.
				// If this parameter is NULL, the key pointed to by the key_name parameter is deleted.
				ini_data->Remove(Conversion::WideToAnsi(lpAppName).c_str(), (Conversion::WideToAnsi(lpKeyName).c_str()));
			else
				ini_data->WriteString(Conversion::WideToAnsi(lpAppName).c_str(), (Conversion::WideToAnsi(lpKeyName).c_str()),
					Conversion::Utf16ToUtf8(lpString).c_str());

#ifdef _CKPE_WITH_QT5
			if (!ini_data->IsEmpty())
				ini_data->Save();
			else
				return false;
#else
			return true;
#endif
		}

		std::string INICacheDataPatch::GetAbsoluteFileName(LPCSTR lpFileName)
		{
			if (!lpFileName) return "";
			if (PathIsRelativeA(lpFileName))
			{
				//_MESSAGE(lpFileName);

				CHAR szBuffer[MAX_PATH];
				std::string sname = lpFileName;
				if (!sname.find_first_of(".\\") || !sname.find_first_of("./"))
				{
					GetCurrentDirectoryA(MAX_PATH, szBuffer);
					sname = szBuffer;
					sname.append(++lpFileName);
				}
				else return "";
				return sname;
			}
			return lpFileName;
		}

		std::wstring INICacheDataPatch::GetAbsoluteFileNameUnicode(LPCWSTR lpFileName)
		{
			if (!lpFileName) return L"";
			if (PathIsRelativeW(lpFileName))
			{
				//_MESSAGE(lpFileName);

				WCHAR szBuffer[MAX_PATH];
				std::wstring sname = lpFileName;
				if (!sname.find_first_of(L".\\") || !sname.find_first_of(L"./"))
				{
					GetCurrentDirectoryW(MAX_PATH, szBuffer);
					sname = szBuffer;
					sname.append(++lpFileName);
				}
				else return L"";
				return sname;
			}
			return lpFileName;
		}

		HANDLE INICacheDataPatch::GetFileFromCacheOrOpen(const std::string& sFileName)
		{
			if (sFileName.empty()) return nullptr;
			// Skip.. there's not much, and generating this file in the root folder is unnecessary.
			if (!EditorAPI::BSString::Utils::ExtractFileName(sFileName.c_str()).Compare("ConstructionSetNetwork.ini")) 
				return nullptr;

			std::shared_ptr<INISettingCollection> ini_data;

			auto iterator_find = GlobalINICache.find(sFileName.c_str());
			if (iterator_find != GlobalINICache.end())
				ini_data = iterator_find->second;

			if (!ini_data)
			{
				if (!Utils::FileExists(sFileName.c_str())) 
					return nullptr;

				ini_data = std::make_shared<INISettingCollection>();
				if (!ini_data) return nullptr;

				ini_data->Open(sFileName.c_str());

				//_MESSAGE("Debug INI: %s", sFileName.c_str());

				GlobalINICache.insert(std::make_pair(sFileName.c_str(), ini_data));
			}

			return (HANDLE)(ini_data.get());
		}

		int INICacheDataPatch::HK_SettingCollectionList_ReadSetting_Read(void* lpCollectionList, EditorAPI::Setting* lpSetting,
			void* INISettingCollectionVTable)
		{
			if (lpSetting)
				_MESSAGE("INI::ReadSetting: %s %p", lpSetting->Name, lpSetting);

			return fastCall<int>(pointer_SettingCollectionList_ReadSetting_Read_code, lpCollectionList, lpSetting, 
				INISettingCollectionVTable);
		}

		bool INICacheDataPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool INICacheDataPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			PatchIAT(HKGetPrivateProfileIntA, "kernel32.dll", "GetPrivateProfileIntA");
			PatchIAT(HKGetPrivateProfileStringA, "kernel32.dll", "GetPrivateProfileStringA");
			PatchIAT(HKGetPrivateProfileStructA, "kernel32.dll", "GetPrivateProfileStructA");
			PatchIAT(HKWritePrivateProfileStringA, "kernel32.dll", "WritePrivateProfileStringA");
			PatchIAT(HKWritePrivateProfileStructA, "kernel32.dll", "WritePrivateProfileStructA");
			PatchIAT(HKGetPrivateProfileIntW, "kernel32.dll", "GetPrivateProfileIntW");
			PatchIAT(HKGetPrivateProfileStringW, "kernel32.dll", "GetPrivateProfileStringW");
			PatchIAT(HKWritePrivateProfileStringW, "kernel32.dll", "WritePrivateProfileStringW");
	
			//pointer_SettingCollectionList_ReadSetting_Read_code = lpRelocator->DetourFunction(0x259AB50, // F4OG 0x2604EB0
			//	(uintptr_t)&HK_SettingCollectionList_ReadSetting_Read);

#if 0
			if (GetShortExecutableTypeFromFull(GlobalEnginePtr->GetEditorVersion()) == EDITOR_SHORT_STARFIELD)
			{
				ResetEvent(GlobalINICacheTriggerEvent);
				std::thread t([]()
					{
						if (WaitForSingleObject(GlobalINICacheTriggerEvent, INFINITE) != WAIT_OBJECT_0)
							return;

						CloseHandle(GlobalINICacheTriggerEvent);

						for (auto it = GlobalINICache.begin(); it != GlobalINICache.end(); it++)
						{
							if (it->second)
							{
								mINI::INIFile file(it->first.c_str());
								file.write(*(it->second), false);
							}
						}

						GlobalINICache.clear();
					});
				t.detach();
			}
#endif

			return true;
		}

		bool INICacheDataPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}