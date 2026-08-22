// Copyright © 2025-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <array>
#include <unordered_map>
#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Fallout4.VersionLists.h>

namespace CKPE
{
	namespace Fallout4
	{
		VersionLists::EDITOR_EXECUTABLE_TYPE _seditor_ver{ VersionLists::EDITOR_UNKNOWN };

		// Список проверенных исполняемых файлов, допущенных к запуску
		static std::unordered_map<uint32_t, VersionLists::EDITOR_EXECUTABLE_TYPE> _sallowedEditorVersion =
		{
			{ 0xDF67F346ul, VersionLists::EDITOR_FALLOUT_C4_1_10_162_0		},	// Default
			{ 0x481CCE95ul, VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		},	// Redirect Steam
			{ 0x55F7F580ul, VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		},	// No Steam and Redirect Steam
			{ 0x49E45284ul, VersionLists::EDITOR_FALLOUT_C4_1_11_137_0		},	// Default
			{ 0x94FA23E9ul, VersionLists::EDITOR_FALLOUT_C4_1_11_240_0		},	// Default
		};

		// Список устаревших версий редакторов - outdated versions
		static std::vector<VersionLists::EDITOR_EXECUTABLE_TYPE> _soutdatedEditorVersion =
		{
			VersionLists::EDITOR_FALLOUT_C4_1_10_943_1,
		};

		// Список ключевых смещений в исполняемых файлах, допущенных к запуску (но не точно) - offsets in exe holding name
		static std::unordered_map<uint32_t, std::pair<std::string_view, VersionLists::EDITOR_EXECUTABLE_TYPE>> _sallowedEditorVersion2 =
		{
			{ 0x3896168ul, { "1.10.162.0",	VersionLists::EDITOR_FALLOUT_C4_1_10_162_0		} },
			{ 0x2F8D1C8ul, { "1.10.943.1",	VersionLists::EDITOR_FALLOUT_C4_1_10_943_1		} },
			{ 0x2F8D298ul, { "1.10.982.3",	VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		} },
			{ 0x3017E68ul, { "1.11.137.0",	VersionLists::EDITOR_FALLOUT_C4_1_11_137_0		} },
			{ 0x30284D8ul, { "1.11.240.0",  VersionLists::EDITOR_FALLOUT_C4_1_11_240_0		} },
		};

		// Список версий - version list
		static std::vector<CKPE::Version> _sEditorVersion =
		{
			{},
			{ 1, 10, 162, 0 },
			{ 1, 10, 943, 1 },
			{ 1, 10, 982, 3 },
			{ 1, 11, 137, 0 },
			{ 1, 11, 240, 0 },
		};

		// Список названий редакторов - editor names
		static std::vector<std::wstring_view> _sEditorVersionStr =
		{
			L"Unknown version",
			L"Fallout 4 [v1.10.162.0]",
			L"Fallout 4 [v1.10.943.1]",
			L"Fallout 4 [v1.10.982.3]",
			L"Fallout 4 [v1.11.137.0]",
			L"Fallout 4 [v1.11.240.0]",
		};

		// Список имён файлов базы данных - db list
		// Обязательно ограничено Common::SUPPORT_RUNTIMECOUNT
		static std::array<std::pair<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view>, Common::SUPPORT_RUNTIMECOUNT> _sallowedDatabaseVersion =
		{ {
			{ VersionLists::EDITOR_FALLOUT_C4_1_10_162_0,	L"CreationKitPlatformExtended_FO4_1_10_162.database"	},
			{ VersionLists::EDITOR_FALLOUT_C4_1_10_982_3,	L"CreationKitPlatformExtended_FO4_1_10_982_3.database"	},
			{ VersionLists::EDITOR_FALLOUT_C4_1_11_137_0,	L"CreationKitPlatformExtended_FO4_1_11_137_0.database"	},
			{ VersionLists::EDITOR_FALLOUT_C4_1_11_240_0,	L"CreationKitPlatformExtended_FO4_1_11_240_0.database"	},
		} };
		static_assert(_sallowedDatabaseVersion.size() == Common::SUPPORT_RUNTIMECOUNT);
		
		// CK Address Library
		static std::unordered_map<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view> _sallowedAddressLibraryVersion =
		{
			{ VersionLists::EDITOR_FALLOUT_C4_1_11_240_0,	L"version-1-11-240-0.bin" },
		};

		void VersionLists::Verify()
		{
			auto it = _sallowedEditorVersion.find(Application::GetSingleton()->GetFileCRC32());
			if (it != _sallowedEditorVersion.end())
			{
				_seditor_ver = it->second;
				return;
			}

			for (auto& it2 : _sallowedEditorVersion2)
			{
				// Защита в случаи выхода за пределы при проверке
				__try
				{
					// Сравнение по указанному смещению нужной строки - string comparison at offset
					if (!_stricmp((const char*)((std::uintptr_t)GetModuleHandleA(nullptr) + it2.first),
						it2.second.first.data()))
					{
						_seditor_ver = it2.second.second;
						break;
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{}
			}
		}

		bool VersionLists::HasAllowedEditorVersion() noexcept(true)
		{
			if (HasOutdatedEditorVersion())
				return false;

			return _seditor_ver != VersionLists::EDITOR_UNKNOWN;
		}

		bool VersionLists::HasOutdatedEditorVersion() noexcept(true)
		{
			return std::find(_soutdatedEditorVersion.begin(), _soutdatedEditorVersion.end(), _seditor_ver)
				!= _soutdatedEditorVersion.end();
		}

		std::wstring VersionLists::GetGameName() noexcept(true)
		{
			return L"FO4";
		}

		std::uint8_t VersionLists::GetRuntimeIndex() noexcept(true)
		{
			static std::uint8_t cache = 0xFF;
			if (cache != 0xFF) return cache;

			std::uint8_t id{ 0 };
			for (auto& p : _sallowedDatabaseVersion)
			{
				if (p.first == _seditor_ver)
					return cache = id;
				id++;
			}

			return 0xFF;
		}

		CKPE::Version VersionLists::GetEditorVersionByNum() noexcept(true)
		{
			return _sEditorVersion[_seditor_ver];
		}

		std::wstring VersionLists::GetDatabaseFileName() noexcept(true)
		{
			auto it = std::find_if(_sallowedDatabaseVersion.begin(), _sallowedDatabaseVersion.end(), [](auto& it) {
				return it.first == _seditor_ver;
				});
			return (it != _sallowedDatabaseVersion.end()) ? it->second.data() : L"";
		}

		bool VersionLists::HasAddressLibrarySupport() noexcept(true)
		{
			return _sallowedAddressLibraryVersion.contains(_seditor_ver);
		}

		std::wstring VersionLists::GetAddressLibraryFileName() noexcept(true)
		{
			auto it = _sallowedAddressLibraryVersion.find(_seditor_ver);
			return (it != _sallowedAddressLibraryVersion.end()) ? it->second.data() : L"";
		}

		std::wstring VersionLists::GetEditorVersionByString() noexcept(true)
		{
			return (_sEditorVersionStr.size() > _seditor_ver) ? _sEditorVersionStr[_seditor_ver].data() : _sEditorVersionStr[0].data();
		}

		std::wstring VersionLists::GetExternalResourcePackageFileName() noexcept(true)
		{
			return L"CreationKitPlatformExtended_FO4_Resources.pak";
		}

		VersionLists::EDITOR_EXECUTABLE_TYPE VersionLists::GetEditorVersion() noexcept(true)
		{
			return _seditor_ver;
		}

		std::wstring VersionLists::GetAddressLibraryRelativePath() noexcept(true)
		{
			std::wstring path = L"CKPEBins\\";
			path += GetGameName();
			path += L"\\";
			path += GetAddressLibraryFileName();

			return path;
		}
	}
}
