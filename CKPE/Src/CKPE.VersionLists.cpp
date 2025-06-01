// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <unordered_map>
#include <windows.h>
#include <CKPE.VersionLists.h>

namespace CKPE
{
	VersionLists::EDITOR_EXECUTABLE_TYPE _seditor_ver{ VersionLists::EDITOR_UNKNOWN };

	// —писок проверенных исполн¤емых файлов, допущенных к запуску
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
		{ 0xDF67F346ul, VersionLists::EDITOR_FALLOUT_C4_1_10_162_0		},	// Default
		//{ 0x4BA2E086ul, VersionLists::EDITOR_FALLOUT_C4_1_10_943_1	},	// Redirect Steam
		//{ 0xBAAF1B07ul, VersionLists::EDITOR_FALLOUT_C4_1_10_943_1	},	// No Steam and Redirect Steam
		{ 0x481CCE95ul, VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		},	// Redirect Steam
		{ 0x55F7F580ul, VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		},	// No Steam and Redirect Steam
		//{ 0x6CDE4424ul, VersionLists::EDITOR_STARFIELD_1_13_61_0		},	// Redirect Steam
		//{ 0x8777A522ul, VersionLists::EDITOR_STARFIELD_1_14_70_0		},	// Redirect Steam
		//{ 0x01BF6FB3ul, VersionLists::EDITOR_STARFIELD_1_14_74_0		},	// Redirect Steam
		{ 0x8C475320ul, VersionLists::EDITOR_STARFIELD_1_14_78_0		},	// Redirect Steam
		{ 0x8C475320ul, VersionLists::EDITOR_STARFIELD_1_15_216_0		},	// Redirect Steam
	};

	// —писок устаревших версий редакторов
	static std::vector<VersionLists::EDITOR_EXECUTABLE_TYPE> _soutdatedEditorVersion = 
	{
		VersionLists::EDITOR_SKYRIM_SE_1_5_3,
		VersionLists::EDITOR_SKYRIM_SE_1_6_438,
		VersionLists::EDITOR_FALLOUT_C4_1_10_943_1,
		VersionLists::EDITOR_STARFIELD_1_13_61_0,
		VersionLists::EDITOR_STARFIELD_1_14_70_0,
		VersionLists::EDITOR_STARFIELD_1_14_74_0,
	};

	// —писок ключевых смещений в исполн¤емых файлах, допущенных к запуску (но не точно)
	static std::unordered_map<uint32_t,
		std::pair<std::string_view, VersionLists::EDITOR_EXECUTABLE_TYPE>> _sallowedEditorVersion2 = 
	{
		//{ 0x3078988ul, { "1.5.3.0",		VersionLists::EDITOR_SKYRIM_SE_1_5_3		} },
		{ 0x3062CC8ul, { "1.5.73.0",	VersionLists::EDITOR_SKYRIM_SE_1_5_73			} },
		//{ 0x2E835D8ul, { "1.6.438.0",	VersionLists::EDITOR_SKYRIM_SE_1_6_438			} },
		{ 0x2F3E698ul, { "1.6.1130.0",	VersionLists::EDITOR_SKYRIM_SE_1_6_1130			} },
		{ 0x2F7F018ul, { "1.6.1378.1",	VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1		} },
		{ 0x3896168ul, { "1.10.162.0",	VersionLists::EDITOR_FALLOUT_C4_1_10_162_0		} },
		//{ 0x2F8D1C8ul, { "1.10.943.1",	VersionLists::EDITOR_FALLOUT_C4_1_10_943_1	} },
		{ 0x2F8D298ul, { "1.10.982.3",	VersionLists::EDITOR_FALLOUT_C4_1_10_982_3		} },
		//{ 0x86DD768ul, { "1.13.61.0",	VersionLists::EDITOR_STARFIELD_1_13_61_0		} },
		//{ 0x873D2B8ul, { "1.14.70.0",	VersionLists::EDITOR_STARFIELD_1_14_70_0		} },
		//{ 0x875F450ul, { "1.14.74.0",	VersionLists::EDITOR_STARFIELD_1_14_74_0		} },
		{ 0x875F550ul, { "1.14.78.0",	VersionLists::EDITOR_STARFIELD_1_14_78_0		} },
		{ 0x84D9B40ul, { "1.15.216.0",	VersionLists::EDITOR_STARFIELD_1_15_216_0		} },
	};

	// —писок названий редакторов
	static std::vector<std::wstring_view> _sEditorVersionStr =
	{
		L"Unknown version",
		L"Skyrim Special Edition [v1.5.3]",
		L"Skyrim Special Edition [v1.5.73]",
		L"Skyrim Special Edition [v1.6.438]",
		L"Skyrim Special Edition [v1.6.1130]",
		L"Skyrim Special Edition [v1.6.1378.1]",
		L"Fallout 4 [v1.10.162.0]",
		L"Fallout 4 [v1.10.943.1]",
		L"Fallout 4 [v1.10.982.3]",
		L"Starfield [v1.13.61.0]",
		L"Starfield [v1.14.70.0]",
		L"Starfield [v1.14.74.0]",
		L"Starfield [v1.14.78.0]",
		L"Starfield [v1.15.216.0]",
	};

	// —писок имЄн файлов базы данных
	static std::unordered_map<VersionLists::EDITOR_EXECUTABLE_TYPE, std::wstring_view> _sallowedDatabaseVersion = 
	{
		//{ VersionLists::EDITOR_SKYRIM_SE_1_5_3,		L"CreationKitPlatformExtended_SSE_1_5_3.database"		},
		{ VersionLists::EDITOR_SKYRIM_SE_1_5_73,		L"CreationKitPlatformExtended_SSE_1_5_73.database"		},
		//{ VersionLists::EDITOR_SKYRIM_SE_1_6_438,		L"CreationKitPlatformExtended_SSE_1_6_438.database"		},
		{ VersionLists::EDITOR_SKYRIM_SE_1_6_1130,		L"CreationKitPlatformExtended_SSE_1_6_1130.database"	},
		{ VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1,	L"CreationKitPlatformExtended_SSE_1_6_1378_1.database"	},
		{ VersionLists::EDITOR_FALLOUT_C4_1_10_162_0,	L"CreationKitPlatformExtended_FO4_1_10_162.database"	},
		//{ VersionLists::EDITOR_FALLOUT_C4_1_10_943_1,	L"CreationKitPlatformExtended_FO4_1_10_943_1.database"	},
		{ VersionLists::EDITOR_FALLOUT_C4_1_10_982_3,	L"CreationKitPlatformExtended_FO4_1_10_982_3.database"	},
		//{ VersionLists::EDITOR_STARFIELD_1_13_61_0,	L"CreationKitPlatformExtended_SF_1_13_61_0.database"	},
		//{ VersionLists::EDITOR_STARFIELD_1_14_70_0,	L"CreationKitPlatformExtended_SF_1_14_70_0.database"	},
		//{ VersionLists::EDITOR_STARFIELD_1_14_74_0,	L"CreationKitPlatformExtended_SF_1_14_74_0.database"	},
		{ VersionLists::EDITOR_STARFIELD_1_14_78_0,		L"CreationKitPlatformExtended_SF_1_14_78_0.database"	},
		{ VersionLists::EDITOR_STARFIELD_1_15_216_0,	L"CreationKitPlatformExtended_SF_1_15_216_0.database"	},
	};

	// —писок коротких названий поддерживаемых игр
	static std::unordered_map<VersionLists::EDITOR_EXECUTABLE_SHORT_TYPE, std::wstring_view> _sallowedShortNameGame =
	{
		{ VersionLists::EDITOR_SHORT_SKYRIM_SE,		L"SSE" },
		{ VersionLists::EDITOR_SHORT_FALLOUT_C4,	L"FO4" },
		{ VersionLists::EDITOR_SHORT_STARFIELD,		L"SF"  },
	};

	// —писок имЄн файлов внешних ресурсов дл¤ qt
	static std::unordered_map<VersionLists::EDITOR_EXECUTABLE_SHORT_TYPE, std::wstring_view> _sqtExternalResourcePackageFile = 
	{
		{ VersionLists::EDITOR_SHORT_STARFIELD,	L"CreationKitPlatformExtended_SF_QResources.pak" },
	};

	void VersionLists::Verify()
	{
		for (auto editorVersionIterator2 = _sallowedEditorVersion2.begin();
			editorVersionIterator2 != _sallowedEditorVersion2.end();
			editorVersionIterator2++)
		{
			// «ащита в случаи выхода за пределы при проверке
			__try
			{
				// —равнение по указанному смещению нужной строки
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

	VersionLists::EDITOR_EXECUTABLE_SHORT_TYPE VersionLists::GetGameEditor() noexcept(true)
	{
		switch (_seditor_ver)
		{
		case EDITOR_SKYRIM_SE_1_5_3:
		case EDITOR_SKYRIM_SE_1_5_73:
		case EDITOR_SKYRIM_SE_1_6_438:
		case EDITOR_SKYRIM_SE_1_6_1130:
		case EDITOR_SKYRIM_SE_1_6_1378_1:
			return EDITOR_SHORT_SKYRIM_SE;
		case EDITOR_FALLOUT_C4_1_10_162_0:
		case EDITOR_FALLOUT_C4_1_10_943_1:
		case EDITOR_FALLOUT_C4_1_10_982_3:
			return EDITOR_SHORT_FALLOUT_C4;
		case EDITOR_STARFIELD_1_13_61_0:
		case EDITOR_STARFIELD_1_14_70_0:
		case EDITOR_STARFIELD_1_14_74_0:
		case EDITOR_STARFIELD_1_14_78_0:
		case EDITOR_STARFIELD_1_15_216_0:
			return EDITOR_SHORT_STARFIELD;
		default:
			return EDITOR_SHORT_UNKNOWN;
		}
	}

	std::wstring VersionLists::GetGameShortName() noexcept(true)
	{
		auto it = _sallowedShortNameGame.find(GetGameEditor());
		return (it != _sallowedShortNameGame.end()) ? it->second.data() : L"";
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
		auto it = _sqtExternalResourcePackageFile.find(GetGameEditor());
		return (it != _sqtExternalResourcePackageFile.end()) ? it->second.data() : L"";
	}
}