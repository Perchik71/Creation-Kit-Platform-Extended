// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	namespace Forms { class TESForm; }

	// EditFormToWindow passes this descriptor, not a BaseDialog instance.
	struct TESFormEditParams
	{
		/* 0x000 */ std::uint8_t FormType;
		/* 0x001 */ std::uint8_t pad001[0x07];
		/* 0x008 */ Forms::TESForm* OriginalForm;
		/* 0x010 */ void* Context;
		/* 0x018 */ std::int32_t ObjectTemplateRequest;
		/* 0x01C */ bool Reserved;
		/* 0x01D */ bool Modal;
		/* 0x01E */ std::uint8_t pad01E[0x02];
	};
	static_assert(sizeof(TESFormEditParams) == 0x20);
	static_assert(offsetof(TESFormEditParams, OriginalForm) == 0x08);
	static_assert(offsetof(TESFormEditParams, ObjectTemplateRequest) == 0x18);
	static_assert(offsetof(TESFormEditParams, Modal) == 0x1D);
}
