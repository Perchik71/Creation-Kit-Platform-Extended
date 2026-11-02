// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/D/DialogExtraLocalCopy.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraLocalBaseObjectCopy : public DialogExtraLocalCopy
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::DialogExtraLocalBaseObjectCopy;
		constexpr static Common::ID VTABLE = IDs::VTABLE::DialogExtraLocalBaseObjectCopy[0];

		/* 0x000 */ virtual ~DialogExtraLocalBaseObjectCopy();
		/* 0x018 */ [[nodiscard]] Forms::TESForm* GetForm() override;

	protected:
		/* 0x020 */ void Unk020() override;

	public:
		/* 0x028 */ void SetForm(Forms::TESForm* Form) override;

		/* 0x018 */ Forms::TESForm* LocalForm;
	};
	static_assert(sizeof(DialogExtraLocalBaseObjectCopy) == 0x20);
	static_assert(offsetof(DialogExtraLocalBaseObjectCopy, LocalForm) == 0x18);
}
