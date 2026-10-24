// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>

namespace CKPE::Fallout4::EditorAPI
{
	namespace Forms { class TESForm; }

	namespace BGSObjectWindowUtils
	{
		// Only the 1.10.162 callback uses this stack descriptor.
		struct LegacyObjectListInsertData
		{
			/* 0x000 */ const char* TreePathFilter;
			/* 0x008 */ const char* IDFilter;
			/* 0x010 */ std::int32_t InsertedCount;
			/* 0x014 */ std::uint8_t pad014[0x04];
			/* 0x018 */ HWND ListWindow;
			/* 0x020 */ std::int32_t TreePathLength;
			/* 0x024 */ std::uint8_t pad024[0x04];
			/* 0x028 */ Forms::TESForm** FirstInsertedForm;
		};
		static_assert(sizeof(LegacyObjectListInsertData) == 0x30);
		static_assert(offsetof(LegacyObjectListInsertData, ListWindow) == 0x18);
		static_assert(offsetof(LegacyObjectListInsertData, TreePathLength) == 0x20);
		static_assert(offsetof(LegacyObjectListInsertData, FirstInsertedForm) == 0x28);
	}
}
