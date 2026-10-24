// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiObjectNET.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkExtraData : public NiAPI::NiExtraData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x18];
	};
	static_assert(sizeof(bhkExtraData) == 0x30);
}
