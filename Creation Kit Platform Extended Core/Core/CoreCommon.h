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
			EDITOR_SKYRIM_SE_LAST = EDITOR_SKYRIM_SE_1_6_1130
		};

		// Не использовать менеджер на данном этапе, считаем, что его ещё нет
		// Связи с этим стандартные STL с обычным аллокатором

		// Список проверенных исполняемых файлов, допущенных к запуску
		static std::unordered_map<uint32_t, EDITOR_EXECUTABLE_TYPE> allowedEditorVersion = {
			{ 0x2EF668CEul, EDITOR_SKYRIM_SE_1_6_1130 },
			{ 0x0085AA4Cul, EDITOR_SKYRIM_SE_1_6_1130 },
		};
		
		// Список ключевых смещений в исполняемых файлах, допущенных к запуску
		static std::unordered_map<uint32_t,
			std::pair<std::string_view, EDITOR_EXECUTABLE_TYPE>> allowedEditorVersion2 = {
			{ 0x2F3E698ul, { "1.6.1130.0", EDITOR_SKYRIM_SE_1_6_1130 } },
		};

		// Список названий редакторов
		static std::vector<std::string_view> allowedEditorVersionStr = {
			"Unknown version",
			"Skyrim Special Edition [v1.5.3]",
			"Skyrim Special Edition [v1.5.73]",
			"Skyrim Special Edition [v1.6.438]",
			"Skyrim Special Edition [v1.6.1130]",
		};

		// Список имён файлов базы данных
		static std::unordered_map<EDITOR_EXECUTABLE_TYPE, std::string_view> allowedDatabaseVersion = {
			{ EDITOR_SKYRIM_SE_1_5_3,		"CreationKitPlatformExtended_SSE_1_5_3.database"	},
			{ EDITOR_SKYRIM_SE_1_5_73,		"CreationKitPlatformExtended_SSE_1_5_73.database"	},
			{ EDITOR_SKYRIM_SE_1_6_438,		"CreationKitPlatformExtended_SSE_1_6_438.database"	},
			{ EDITOR_SKYRIM_SE_1_6_1130,	"CreationKitPlatformExtended_SSE_1_6_1130.database" },
		};

		// Список имён файлов базы данных
		static std::unordered_map<EDITOR_EXECUTABLE_TYPE, std::string_view> allowedDialogsPackageFile = {
			{ EDITOR_SKYRIM_SE_1_5_3,		"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_SKYRIM_SE_1_5_73,		"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_SKYRIM_SE_1_6_438,		"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
			{ EDITOR_SKYRIM_SE_1_6_1130,	"CreationKitPlatformExtended_SSE_Dialogs.pak"	},
		};

		static std::vector<std::string_view> allowedEditorFileNameStr = {
			"creationkit",
			"creationkit_se_1_5_3",
			"creationkit_se_1_5_73",
			"creationkit_se_1_6_438",
			"creationkit_se_1_6_1130",
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