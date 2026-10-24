// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraWorkingData : public BSExtraData
	{
	public:
		enum : std::uint8_t { kDialogExtraType = 8 };
		/* 0x000 */ virtual ~DialogExtraWorkingData();

		/* 0x018 */ std::uint8_t pad018[0x08];
		/* 0x020 */ void* WorkingData;
	};
	static_assert(sizeof(DialogExtraWorkingData) == 0x28);
	static_assert(offsetof(DialogExtraWorkingData, WorkingData) == 0x20);
}
