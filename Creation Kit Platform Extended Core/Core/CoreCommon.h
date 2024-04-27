// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		enum EDITOR_EXECUTABLE_TYPE {
			EDITOR_UNKNOWN,
			EDITOR_SKYRIM_SE_1_5_3,
			EDITOR_SKYRIM_SE_1_5_73,
			EDITOR_SKYRIM_SE_1_6_438,
			EDITOR_SKYRIM_SE_1_6_1130 = EDITOR_SKYRIM_SE_1_6_438 + 1,
			EDITOR_SKYRIM_SE_LAST = EDITOR_SKYRIM_SE_1_6_1130,
			EDITOR_FALLOUT_C4_1_10_162_0,
			EDITOR_FALLOUT_C4_1_10_943_1,
			EDITOR_FALLOUT_C4_LAST = EDITOR_FALLOUT_C4_1_10_943_1
		};

		// Не использовать менеджер на данном этапе, считаем, что его ещё нет
		// Связи с этим стандартные STL с обычным аллокатором

		// Список проверенных исполняемых файлов, допущенных к запуску
		static std::unordered_map<uint32_t, EDITOR_EXECUTABLE_TYPE> allowedEditorVersion = {
			{ 0xA9CD60C7ul, EDITOR_SKYRIM_SE_1_5_3			},	// Default
			{ 0xED7DB054ul, EDITOR_SKYRIM_SE_1_5_3			},	// Redirect Steam
			{ 0x624E8C84ul, EDITOR_SKYRIM_SE_1_5_73			},	// Default
			{ 0xF7E929A4ul, EDITOR_SKYRIM_SE_1_5_73			},	// Redirect Steam
			{ 0x668F3CB3ul, EDITOR_SKYRIM_SE_1_5_73			},	// Unoffical patch 6.3
			{ 0x748A3CC4ul, EDITOR_SKYRIM_SE_1_6_438		},	// No Steam
			{ 0x3FDB3994ul, EDITOR_SKYRIM_SE_1_6_438		},	// With Steam
			{ 0xFDCAEE10ul, EDITOR_SKYRIM_SE_1_6_438		},	// No Steam and Redirect Steam
			{ 0x2EF668CEul, EDITOR_SKYRIM_SE_1_6_1130		},	// Redirect Steam
			{ 0x0085AA4Cul, EDITOR_SKYRIM_SE_1_6_1130		},	// Default
			{ 0xDF67F346ul, EDITOR_FALLOUT_C4_1_10_162_0	},	// Default
			{ 0x4BA2E086ul, EDITOR_FALLOUT_C4_1_10_943_1	},	// Redirect Steam
			{ 0xBAAF1B07ul, EDITOR_FALLOUT_C4_1_10_943_1	},	// No Steam and Redirect Steam
		};
		
		// Список ключевых смещений в исполняемых файлах, допущенных к запуску
		static std::unordered_map<uint32_t,
			std::pair<std::string_view, EDITOR_EXECUTABLE_TYPE>> allowedEditorVersion2 = {
			{ 0x3078988ul, { "1.5.3.0",		EDITOR_SKYRIM_SE_1_5_3			} },
			{ 0x3062CC8ul, { "1.5.73.0",	EDITOR_SKYRIM_SE_1_5_73			} },
			{ 0x2E835D8ul, { "1.6.438.0",	EDITOR_SKYRIM_SE_1_6_438		} },
			{ 0x2F3E698ul, { "1.6.1130.0",	EDITOR_SKYRIM_SE_1_6_1130		} },
			{ 0x3896168ul, { "1.10.162.0",	EDITOR_FALLOUT_C4_1_10_162_0	} },
			{ 0x2F8D1C8ul, { "1.10.943.1",	EDITOR_FALLOUT_C4_1_10_943_1	} },
		};
		
		// Список названий редакторов
		static std::vector<std::string_view> allowedEditorVersionStr = {
			"Unknown version",
			"Skyrim Special Edition [v1.5.3]",
			"Skyrim Special Edition [v1.5.73]",
			"Skyrim Special Edition [v1.6.438]",
			"Skyrim Special Edition [v1.6.1130]",
			"Fallout 4 [v1.10.162.0]",
			"Fallout 4 [v1.10.943.1]",
		};

		// Список имён файлов базы данных
		static std::unordered_map<EDITOR_EXECUTABLE_TYPE, std::string_view> allowedDatabaseVersion = {
			{ EDITOR_SKYRIM_SE_1_5_3,		"CreationKitPlatformExtended_SSE_1_5_3.database"		},
			{ EDITOR_SKYRIM_SE_1_5_73,		"CreationKitPlatformExtended_SSE_1_5_73.database"		},
			{ EDITOR_SKYRIM_SE_1_6_438,		"CreationKitPlatformExtended_SSE_1_6_438.database"		},
			{ EDITOR_SKYRIM_SE_1_6_1130,	"CreationKitPlatformExtended_SSE_1_6_1130.database"		},
			{ EDITOR_FALLOUT_C4_1_10_162_0,	"CreationKitPlatformExtended_FO4_1_10_162.database"		},
			{ EDITOR_FALLOUT_C4_1_10_943_1,	"CreationKitPlatformExtended_FO4_1_10_943_1.database"	},
		};

		// Список имён файлов базы данных
		static std::unordered_map<EDITOR_EXECUTABLE_TYPE, std::string_view> allowedDialogsPackageFile = {
			{ EDITOR_SKYRIM_SE_1_5_3,		"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_SKYRIM_SE_1_5_73,		"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_SKYRIM_SE_1_6_438,		"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_SKYRIM_SE_1_6_1130,	"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_FALLOUT_C4_1_10_162_0,	"CreationKitPlatformExtended_FO4_Dialogs.pak"	},
			{ EDITOR_FALLOUT_C4_1_10_943_1,	"CreationKitPlatformExtended_FO4_Dialogs.pak"	},
		};

		static std::vector<std::string_view> allowedEditorFileNameStr = {
			"creationkit",
			"creationkit_se_1_5_3",
			"creationkit_se_1_5_73",
			"creationkit_se_1_6_438",
			"creationkit_se_1_6_1130",
			"creationkit_f4_1_10_162",
			"creationkit_f4_1_10_943_1",
		};

		inline bool CheckFileNameProcess(const char* name)
		{
			for (auto It = allowedEditorFileNameStr.begin();
				It != allowedEditorFileNameStr.end(); It++)
			{
				if (!strcmp(name, It->data()))
					return true;
			}

			return false;
		}
	}
}