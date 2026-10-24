// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraFormTypeContext : public BSExtraData
	{
	public:
		enum : std::uint8_t { kDialogExtraType = 0x20 };
		/* 0x000 */ virtual ~DialogExtraFormTypeContext();

		/* 0x018 */ std::uint32_t FormType;
		/* 0x01C */ std::uint8_t pad01C[0x04];
	};
	static_assert(sizeof(DialogExtraFormTypeContext) == 0x20);
	static_assert(offsetof(DialogExtraFormTypeContext, FormType) == 0x18);
}
