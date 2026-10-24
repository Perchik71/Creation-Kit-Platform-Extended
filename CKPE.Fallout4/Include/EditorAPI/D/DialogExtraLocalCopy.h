// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSExtraData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraLocalCopy : public BSExtraData
	{
	public:
		enum : std::uint8_t { kDialogExtraType = 1 };

		/* 0x000 */ virtual ~DialogExtraLocalCopy();
		/* 0x018 */ [[nodiscard]] virtual Forms::TESForm* GetForm() = 0;

	protected:
		/* 0x020 */ virtual void Unk020() = 0;

	public:
		/* 0x028 */ virtual void SetForm(Forms::TESForm* Form) = 0;
	};
	static_assert(sizeof(DialogExtraLocalCopy) == 0x18);
}
