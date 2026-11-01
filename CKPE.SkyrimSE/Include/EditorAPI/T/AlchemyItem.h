// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/MagicItem.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESWeightForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x398, vtable RVA 0x31251C8
				class AlchemyItem :
					public MagicItem,	// 00
					public TESModelTextureSwap,	// 268
					public TESIcon,	// 2B0
					public BGSMessageIcon,	// 2D8
					public TESWeightForm,	// 308
					public BGSEquipType,	// 318
					public BGSDestructibleObjectForm,	// 328
					public BGSPickupPutdownSounds	// 338
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftPotion;

					virtual ~AlchemyItem() = default;
				public:
					std::uint8_t unk350[0x48];	// 350 - data, layout not resolved
				};
				static_assert(offsetof(AlchemyItem, unk350) == 0x350);
				static_assert(sizeof(AlchemyItem) == 0x398);
			}
		}
	}
}