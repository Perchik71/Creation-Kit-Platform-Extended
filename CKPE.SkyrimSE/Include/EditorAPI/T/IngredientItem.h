// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/MagicItem.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x348, vtable RVA 0x31291E8
				class IngredientItem :
					public MagicItem,	// 00
					public TESModelTextureSwap,	// 268
					public TESIcon,	// 2B0
					public TESWeightForm,	// 2D8
					public BGSEquipType,	// 2E8
					public BGSDestructibleObjectForm,	// 2F8
					public BGSPickupPutdownSounds,	// 308
					public TESValueForm	// 320
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftIngredient;

					virtual ~IngredientItem() = default;
				public:
					std::uint8_t unk330[0x18];	// 330 - data, layout not resolved
				};
				static_assert(offsetof(IngredientItem, unk330) == 0x330);
				static_assert(sizeof(IngredientItem) == 0x348);
			}
		}
	}
}