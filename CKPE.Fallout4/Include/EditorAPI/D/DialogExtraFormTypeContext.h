// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraFormTypeContext : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::DialogExtraFormTypeContext;
		constexpr static Common::ID VTABLE = IDs::VTABLE::DialogExtraFormTypeContext[0];

		enum : std::uint8_t { kDialogExtraType = 0x20 };
		/* 0x000 */ virtual ~DialogExtraFormTypeContext();

		/* 0x018 */ std::uint32_t FormType;
		/* 0x01C */ std::uint8_t pad01C[0x04];
	};
	static_assert(sizeof(DialogExtraFormTypeContext) == 0x20);
	static_assert(offsetof(DialogExtraFormTypeContext, FormType) == 0x18);
}
