// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>
#include <EditorAPI/T/TESFormEditParams.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraParam : public BSExtraData
	{
	public:
		enum : std::uint8_t { kDialogExtraType = 0 };
		/* 0x000 */ virtual ~DialogExtraParam();

		/* 0x018 */ TESFormEditParams Params;
	};
	static_assert(sizeof(DialogExtraParam) == 0x38);
	static_assert(offsetof(DialogExtraParam, Params) == 0x18);
}
