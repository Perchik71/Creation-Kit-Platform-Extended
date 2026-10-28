// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/T/TESDescription.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xD0, vtable RVA 0x307C0F8, 124 virtual functions
				class TESShout :
					public TESForm,               // 000
					public TESFullName,           // 028
					public BGSMenuDisplayObject,  // 040
					public BGSEquipType,          // 050
					public TESDescription         // 060
				{
				public:
					virtual ~TESShout() = default;

					constexpr static std::uint8_t TYPE_ID = ftShout;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESShout[0];
				public:
					std::uint8_t variations[0x48]; // 88 - Variation
				};
				static_assert(sizeof(TESShout) == 0xD0);
			}
		}
	}
}
