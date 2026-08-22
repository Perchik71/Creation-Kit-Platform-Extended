// Copyright © 2025-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <array>
#include <unordered_map>
#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Starfield.VersionLists.h>

namespace CKPE
{
	namespace Starfield
	{
		VersionLists::EDITOR_EXECUTABLE_TYPE _seditor_ver{ VersionLists::EDITOR_UNKNOWN };

		// Список проверенных исполняемых файлов, допущенных к запуску
		static std::unordered_map<uint32_t, VersionLists::EDITOR_EXECUTABLE_TYPE> _sallowedEditorVersion =
		{
			{ 0xAA061EEBul, VersionLists::EDITOR_STARFIELD_1_15_222_0		},	// Redirect Steam
			{ 0x79019BB6ul, VersionLists::EDITOR_STARFIELD_1_16_236_0		},	// Redirect Steam
			{ 0x13F32F60ul, VersionLists::EDITOR_STARFIELD_1_16_242_0		},	// Redirect Steam
			{ 0xF4BDC8ACul, VersionLists::EDITOR_STARFIELD_1_16_244_0		},	// Redirect Steam
		};

		// Список устаревших версий редакторов
		static std::vector<VersionLists::EDITOR_EXECUTABLE_TYPE> _soutdatedEditorVersion =
		{
			VersionLists::EDITOR_STARFIELD_1_13_61_0,
			VersionLists::EDITOR_STARFIELD_1_14_70_0,
			VersionLists::EDITOR_STARFIELD_1_14_74_0,
			VersionLists::EDITOR_STARFIELD_1_14_78_0,
		};

		// Список ключевых смещений в исполняемых файлах, допущенных к запуску (но не точно)
		
		struct VersionEditorAllowed
		{
			uint32_t offset;
			std::string_view text;
			VersionLists::EDITOR_EXECUTABLE_TYPE version;
		};
		
		static std::array<VersionEditorAllowed, Common::SUPPORT_RUNTIMECOUNT> _sallowedEditorVersion2
		{ {
			{ 0x84C7B20ul, "1.15.222.0", VersionLists::EDITOR_STARFIELD_1_15_222_0 },
			{ 0x8542C88ul, "1.16.236.0", VersionLists::EDITOR_STARFIELD_1_16_236_0 },
			{ 0x8542C88ul, "1.16.242.0", VersionLists::EDITOR_STARFIELD_1_16_242_0 },
			{ 0x8542c90ul, "1.16.244.0", VersionLists::EDITOR_STARFIELD_1_16_244_0 },
		} };
		static_assert(_sallowedEditorVersion2.size() == Common::SUPPORT_RUNTIMECOUNT);

		// Список названий редакторов
		static std::vector<std::wstring_view> _sEditorVersionStr =
		{
			L"Unknown version",
			L"Starfield [v1.13.61.0]",
			L"Starfield [v1.14.70.0]",
			L"Starfield [v1.14.74.0]",
			L"Starfield [v1.14.78.0]",
			L"Starfield [v1.15.216.0]",
			L"Starfield [v1.15.222.0]",
			L"Starfield [v1.16.236.0]",
			L"Starfield [v1.16.242.0]",
			L"Starfield [v1.16.244.0]",
		};

		// Список версий
		static std::vector<CKPE::Version> _sEditorVersion =
		{
			{},
			{ 1, 13, 61, 0 },
			{ 1, 14, 70, 0 },
			{ 1, 14, 74, 0 },
			{ 1, 14, 78, 0 },
			{ 1, 15, 216, 0 },
			{ 1, 15, 222, 0 },
			{ 1, 16, 236, 0 },
			{ 1, 16, 242, 0 },
			{ 1, 16, 244, 0 },
		};

		// Список имён файлов базы данных
		// Обязательно ограничено Common::SUPPORT_RUNTIMECOUNT
		static std::array<std::pair<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view>, Common::SUPPORT_RUNTIMECOUNT> _sallowedDatabaseVersion =
		{ {
			{ VersionLists::EDITOR_STARFIELD_1_15_222_0,	L"CreationKitPlatformExtended_SF_1_15_222_0.database"	},
			{ VersionLists::EDITOR_STARFIELD_1_16_236_0,	L"CreationKitPlatformExtended_SF_1_16_236_0.database"	},
			{ VersionLists::EDITOR_STARFIELD_1_16_242_0,	L"CreationKitPlatformExtended_SF_1_16_242_0.database"	},
			{ VersionLists::EDITOR_STARFIELD_1_16_244_0,	L"CreationKitPlatformExtended_SF_1_16_244_0.database"	},
		} };
		static_assert(_sallowedDatabaseVersion.size() == Common::SUPPORT_RUNTIMECOUNT);

		static constexpr auto QT_RESOURCE = L"CreationKitPlatformExtended_SF_QResources.pak";

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
					if (!_stricmp((const char*)((std::uintptr_t)GetModuleHandleA(nullptr) + it2.offset),
						it2.text.data()))
					{
						_seditor_ver = it2.version;
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
			return L"SF";
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

		std::wstring VersionLists::GetDatabaseFileName() noexcept(true)
		{
			auto it = std::find_if(_sallowedDatabaseVersion.begin(), _sallowedDatabaseVersion.end(), [](auto& it) {
				return it.first == _seditor_ver;
				});
			return (it != _sallowedDatabaseVersion.end()) ? it->second.data() : L"";
		}

		CKPE::Version VersionLists::GetEditorVersionByNum() noexcept(true)
		{
			return _sEditorVersion[_seditor_ver];
		}

		std::wstring VersionLists::GetEditorVersionByString() noexcept(true)
		{
			return (_sEditorVersionStr.size() > _seditor_ver) ? _sEditorVersionStr[_seditor_ver].data() : _sEditorVersionStr[0].data();
		}

		std::wstring VersionLists::GetExternalResourcePackageFileName() noexcept(true)
		{
			return QT_RESOURCE;
		}

		VersionLists::EDITOR_EXECUTABLE_TYPE VersionLists::GetEditorVersion() noexcept(true)
		{
			return _seditor_ver;
		}
	}
}