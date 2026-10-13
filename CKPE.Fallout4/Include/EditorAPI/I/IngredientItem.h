// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/M/MagicItem.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESValueForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x3B8, vtable RVA 0x30C0380, 188 virtual functions
				class IngredientItem :
					public MagicItem,                  // 000
					public BGSModelMaterialSwap,       // 2D0
					public TESIcon,                    // 320
					public TESWeightForm,              // 348
					public BGSEquipType,               // 358
					public BGSDestructibleObjectForm,  // 368
					public BGSPickupPutdownSounds,     // 378
					public TESValueForm                // 390
				{
				public:
					virtual ~IngredientItem() = default;

				public:
					std::uint8_t unk3A0[0x10]; // 3A0
					std::uint32_t unk3B0; // 3B0
					std::uint32_t unk3B4; // 3B4
				};
				static_assert(sizeof(IngredientItem) == 0x3B8);
			}
		}
	}
}
