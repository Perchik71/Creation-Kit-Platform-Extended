// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/S/SpellItem.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/T/TESValueForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x3D8, vtable RVA 0x30C2B80, 188 virtual functions
				class ScrollItem :
					public SpellItem,                  // 000
					public BGSModelMaterialSwap,       // 340
					public BGSDestructibleObjectForm,  // 390
					public BGSPickupPutdownSounds,     // 3A0
					public TESWeightForm,              // 3B8
					public TESValueForm                // 3C8
				{
				public:
					virtual ~ScrollItem() = default;

					constexpr static std::uint8_t TYPE_ID = ftScroll;
					constexpr static Common::ID VTABLE = IDs::VTABLE::ScrollItem[0];
				};
				static_assert(sizeof(ScrollItem) == 0x3D8);
			}
		}
	}
}
