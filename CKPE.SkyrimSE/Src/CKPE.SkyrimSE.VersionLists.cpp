// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <unordered_map>
#include <windows.h>
#include <CKPE.Module.h>
#include <CKPE.SkyrimSE.VersionLists.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		VersionLists::EDITOR_EXECUTABLE_TYPE _seditor_ver{ VersionLists::EDITOR_UNKNOWN };

		// Список проверенных исполняемых файлов, допущенных к запуску
		static std::unordered_map<uint32_t, VersionLists::EDITOR_EXECUTABLE_TYPE> _sallowedEditorVersion =
		{
			//{ 0xA9CD60C7ul, VersionLists::EDITOR_SKYRIM_SE_1_5_3			},	// Default
			//{ 0xED7DB054ul, VersionLists::EDITOR_SKYRIM_SE_1_5_3			},	// Redirect Steam
			{ 0x624E8C84ul, VersionLists::EDITOR_SKYRIM_SE_1_5_73			},	// Default
			{ 0xF7E929A4ul, VersionLists::EDITOR_SKYRIM_SE_1_5_73			},	// Redirect Steam
			{ 0x668F3CB3ul, VersionLists::EDITOR_SKYRIM_SE_1_5_73			},	// Unoffical patch 6.3
			//{ 0x748A3CC4ul, VersionLists::EDITOR_SKYRIM_SE_1_6_438		},	// No Steam
			//{ 0x3FDB3994ul, VersionLists::EDITOR_SKYRIM_SE_1_6_438		},	// With Steam
			//{ 0xFDCAEE10ul, VersionLists::EDITOR_SKYRIM_SE_1_6_438		},	// No Steam and Redirect Steam
			{ 0x2EF668CEul, VersionLists::EDITOR_SKYRIM_SE_1_6_1130			},	// Redirect Steam
			{ 0x0085AA4Cul, VersionLists::EDITOR_SKYRIM_SE_1_6_1130			},	// Default
			{ 0x0085AA4Cul, VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1		},	// Redirect Steam
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
			//{ 0x3078988ul,	{ "1.5.3.0",	VersionLists::EDITOR_SKYRIM_SE_1_5_3			} },
			{ 0x3062CC8ul,		{ "1.5.73.0",	VersionLists::EDITOR_SKYRIM_SE_1_5_73			} },
			//{ 0x2E835D8ul,	{ "1.6.438.0",	VersionLists::EDITOR_SKYRIM_SE_1_6_438			} },
			{ 0x2F3E698ul,		{ "1.6.1130.0",	VersionLists::EDITOR_SKYRIM_SE_1_6_1130			} },
			{ 0x2F7F018ul,		{ "1.6.1378.1",	VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1		} },
		};

		// Список версий
		static std::vector<std::uint64_t> _sEditorVersion =
		{
			0,
			MAKE_EXE_VERSION_EX(1, 5, 3, 0),
			MAKE_EXE_VERSION_EX(1, 5, 73, 0),
			MAKE_EXE_VERSION_EX(1, 6, 438, 0),
			MAKE_EXE_VERSION_EX(1, 6, 1130, 0),
			MAKE_EXE_VERSION_EX(1, 6, 1378, 1),
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
		};

		// Список имён файлов базы данных
		static std::unordered_map<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view> _sallowedDatabaseVersion =
		{
			//{ VersionLists::EDITOR_SKYRIM_SE_1_5_3,		L"CreationKitPlatformExtended_SSE_1_5_3.database"		},
			{ VersionLists::EDITOR_SKYRIM_SE_1_5_73,		L"CreationKitPlatformExtended_SSE_1_5_73.database"		},
			//{ VersionLists::EDITOR_SKYRIM_SE_1_6_438,		L"CreationKitPlatformExtended_SSE_1_6_438.database"		},
			{ VersionLists::EDITOR_SKYRIM_SE_1_6_1130,		L"CreationKitPlatformExtended_SSE_1_6_1130.database"	},
			{ VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1,	L"CreationKitPlatformExtended_SSE_1_6_1378_1.database"	},
		};

		void VersionLists::Verify()
		{
			for (auto editorVersionIterator2 = _sallowedEditorVersion2.begin();
				editorVersionIterator2 != _sallowedEditorVersion2.end();
				editorVersionIterator2++)
			{
				// Защита в случаи выхода за пределы при проверке
				__try
				{
					// Сравнение по указанному смещению нужной строки
					if (!_stricmp((const char*)((std::uintptr_t)GetModuleHandleA(nullptr) + editorVersionIterator2->first),
						editorVersionIterator2->second.first.data()))
					{
						_seditor_ver = editorVersionIterator2->second.second;
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

		std::uint64_t VersionLists::GetEditorVersionByNum() noexcept(true)
		{
			return _sEditorVersion[_seditor_ver];
		}

		std::wstring VersionLists::GetDatabaseFileName() noexcept(true)
		{
			auto it = _sallowedDatabaseVersion.find(_seditor_ver);
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