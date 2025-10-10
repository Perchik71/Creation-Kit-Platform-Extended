// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <unordered_map>
#include <windows.h>
#include <CKPE.Module.h>
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
			//{ 0x4BA2E086ul, VersionLists::EDITOR_FALLOUT_C4_1_10_943_1	},	// Redirect Steam
			//{ 0xBAAF1B07ul, VersionLists::EDITOR_FALLOUT_C4_1_10_943_1	},	// No Steam and Redirect Steam
			{ 0x481CCE95ul, VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		},	// Redirect Steam
			{ 0x55F7F580ul, VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		},	// No Steam and Redirect Steam
		};

		// Список устаревших версий редакторов
		static std::vector<VersionLists::EDITOR_EXECUTABLE_TYPE> _soutdatedEditorVersion =
		{
			VersionLists::EDITOR_FALLOUT_C4_1_10_943_1,
		};

		// Список ключевых смещений в исполняемых файлах, допущенных к запуску (но не точно)
		static std::unordered_map<uint32_t,
			std::pair<std::string_view, VersionLists::EDITOR_EXECUTABLE_TYPE>> _sallowedEditorVersion2 =
		{
			{ 0x3896168ul, { "1.10.162.0",	VersionLists::EDITOR_FALLOUT_C4_1_10_162_0		} },
			//{ 0x2F8D1C8ul, { "1.10.943.1",	VersionLists::EDITOR_FALLOUT_C4_1_10_943_1	} },
			{ 0x2F8D298ul, { "1.10.982.3",	VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		} },
		};

		// Список версий
		static std::vector<std::uint64_t> _sEditorVersion =
		{
			0,
			MAKE_EXE_VERSION_EX(1, 10, 162, 0),
			MAKE_EXE_VERSION_EX(1, 10, 943, 1),
			MAKE_EXE_VERSION_EX(1, 10, 982, 3),
		};

		// Список названий редакторов
		static std::vector<std::wstring_view> _sEditorVersionStr =
		{
			L"Unknown version",
			L"Fallout 4 [v1.10.162.0]",
			L"Fallout 4 [v1.10.943.1]",
			L"Fallout 4 [v1.10.982.3]",
		};

		// Список имён файлов базы данных
		static std::unordered_map<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view> _sallowedDatabaseVersion =
		{
			{ VersionLists::EDITOR_FALLOUT_C4_1_10_162_0,	L"CreationKitPlatformExtended_FO4_1_10_162.database"	},
			//{ VersionLists::EDITOR_FALLOUT_C4_1_10_943_1,	L"CreationKitPlatformExtended_FO4_1_10_943_1.database"	},
			{ VersionLists::EDITOR_FALLOUT_C4_1_10_982_3,	L"CreationKitPlatformExtended_FO4_1_10_982_3.database"	},
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
			return L"FO4";
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
			return L"CreationKitPlatformExtended_FO4_Resources.pak";
		}

		VersionLists::EDITOR_EXECUTABLE_TYPE VersionLists::GetEditorVersion() noexcept(true)
		{
			return _seditor_ver;
		}
	}
}