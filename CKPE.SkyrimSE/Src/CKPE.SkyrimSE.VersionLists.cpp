// Copyright © 2025-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <array>
#include <unordered_map>
#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.SkyrimSE.VersionLists.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		VersionLists::EDITOR_EXECUTABLE_TYPE _seditor_ver{ VersionLists::EDITOR_UNKNOWN };

		// Список проверенных исполняемых файлов, допущенных к запуску
		static std::unordered_map<uint32_t, VersionLists::EDITOR_EXECUTABLE_TYPE> _sallowedEditorVersion =
		{
			{ 0x624E8C84ul, VersionLists::EDITOR_SKYRIM_SE_1_5_73			},	// Default
			{ 0xF7E929A4ul, VersionLists::EDITOR_SKYRIM_SE_1_5_73			},	// Redirect Steam
			{ 0x668F3CB3ul, VersionLists::EDITOR_SKYRIM_SE_1_5_73			},	// Unoffical patch 6.3
			{ 0x2EF668CEul, VersionLists::EDITOR_SKYRIM_SE_1_6_1130			},	// Redirect Steam
			{ 0x0085AA4Cul, VersionLists::EDITOR_SKYRIM_SE_1_6_1130			},	// Default
			{ 0x0085AA4Cul, VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1		},	// Redirect Steam
			{ 0x4C73C37Bul, VersionLists::EDITOR_SKYRIM_SE_1_7_99_0			},	// Redirect Steam
		};

		// Список устаревших версий редакторов
		static std::vector<VersionLists::EDITOR_EXECUTABLE_TYPE> _soutdatedEditorVersion =
		{
			VersionLists::EDITOR_SKYRIM_SE_1_5_3,
			VersionLists::EDITOR_SKYRIM_SE_1_6_438,
		};

		// Список ключевых смещений в исполняемых файлах, допущенных к запуску (но не точно)
		static std::unordered_map<uint32_t,
			std::pair<std::string_view, VersionLists::EDITOR_EXECUTABLE_TYPE>> _sallowedEditorVersion2 =
		{
			{ 0x3078988ul,		{ "1.5.3.0",	VersionLists::EDITOR_SKYRIM_SE_1_5_3			} },
			{ 0x3062CC8ul,		{ "1.5.73.0",	VersionLists::EDITOR_SKYRIM_SE_1_5_73			} },
			{ 0x2E835D8ul,		{ "1.6.438.0",	VersionLists::EDITOR_SKYRIM_SE_1_6_438			} },
			{ 0x2F3E698ul,		{ "1.6.1130.0",	VersionLists::EDITOR_SKYRIM_SE_1_6_1130			} },
			{ 0x2F7F018ul,		{ "1.6.1378.1",	VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1		} },
			{ 0x3031590ul,		{ "1.7.99.0",	VersionLists::EDITOR_SKYRIM_SE_1_7_99_0			} },
		};

		// Список версий
		static std::vector<CKPE::Version> _sEditorVersion =
		{
			{},
			{ 1,	5,	3,		0 },
			{ 1,	5,	73,		0 },
			{ 1,	6,	438,	0 },
			{ 1,	6,	1130,	0 },
			{ 1,	6,	1378,	1 },
			{ 1,	7,	99,		0 },
		};

		// Список названий редакторов
		static std::vector<std::wstring_view> _sEditorVersionStr =
		{
			L"Unknown version",
			L"Skyrim Special Edition [v1.5.3]",
			L"Skyrim Special Edition [v1.5.73]",
			L"Skyrim Special Edition [v1.6.438]",
			L"Skyrim Special Edition [v1.6.1130]",
			L"Skyrim Special Edition [v1.6.1378.1]",
			L"Skyrim Special Edition [v1.7.99.0]",
		};

		// Список имён файлов базы данных
		// Обязательно ограничено Common::SUPPORT_RUNTIMECOUNT
		static std::array<std::pair<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view>, Common::SUPPORT_RUNTIMECOUNT> _sallowedDatabaseVersion =
		{{
			{ VersionLists::EDITOR_SKYRIM_SE_1_5_73,		L"CreationKitPlatformExtended_SSE_1_5_73.database"		},
			{ VersionLists::EDITOR_SKYRIM_SE_1_6_1130,		L"CreationKitPlatformExtended_SSE_1_6_1130.database"	},
			{ VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1,	L"CreationKitPlatformExtended_SSE_1_6_1378_1.database"	},
			{ VersionLists::EDITOR_SKYRIM_SE_1_7_99_0,		L"CreationKitPlatformExtended_SSE_1_7_99.database"		},
		}};
		static_assert(_sallowedDatabaseVersion.size() == Common::SUPPORT_RUNTIMECOUNT);

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
					// Сравнение по указанному смещению нужной строки
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
			return L"SSE";
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

		std::wstring VersionLists::GetEditorVersionByString() noexcept(true)
		{
			return (_sEditorVersionStr.size() > _seditor_ver) ? _sEditorVersionStr[_seditor_ver].data() : _sEditorVersionStr[0].data();
		}

		std::wstring VersionLists::GetExternalResourcePackageFileName() noexcept(true)
		{
			return L"";
		}

		VersionLists::EDITOR_EXECUTABLE_TYPE VersionLists::GetEditorVersion() noexcept(true)
		{
			return _seditor_ver;
		}
	}
}