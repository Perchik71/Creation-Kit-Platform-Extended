// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/D/DialogExtraLocalCopy.h>
#include <EditorAPI/O/ObjectRefHandle.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraLocalReferenceCopy : public DialogExtraLocalCopy
	{
	public:
		/* 0x000 */ virtual ~DialogExtraLocalReferenceCopy();
		/* 0x018 */ [[nodiscard]] Forms::TESForm* GetForm() override;

	protected:
		/* 0x020 */ void Unk020() override;

	public:
		/* 0x028 */ void SetForm(Forms::TESForm* Form) override;

		/* 0x018 */ ObjectRefHandle ReferenceHandle;
		/* 0x01C */ std::uint8_t pad01C[0x04];
	};
	static_assert(sizeof(DialogExtraLocalReferenceCopy) == 0x20);
	static_assert(offsetof(DialogExtraLocalReferenceCopy, ReferenceHandle) == 0x18);
}
