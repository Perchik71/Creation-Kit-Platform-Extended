// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/M/MagicItem.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSCraftingUseSound.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x450, vtable RVA 0x30BC3C8, 188 virtual functions
				class AlchemyItem :
					public MagicItem,                  // 000
					public BGSModelMaterialSwap,       // 2D0
					public TESIcon,                    // 320
					public BGSMessageIcon,             // 348
					public TESWeightForm,              // 378
					public BGSEquipType,               // 388
					public BGSDestructibleObjectForm,  // 398
					public BGSPickupPutdownSounds,     // 3A8
					public BGSCraftingUseSound,        // 3C0
					public TESDescription              // 3D0
				{
				public:
					virtual ~AlchemyItem() = default;

					constexpr static std::uint8_t TYPE_ID = ftAlchemy;
					constexpr static Common::ID VTABLE = IDs::VTABLE::AlchemyItem[0];
				public:
					std::uint32_t unk3F8; // 3F8
					std::uint32_t flags; // 3FC
					std::uint64_t unk400; // 400
					std::uint32_t unk408; // 408
					std::uint32_t unk40C; // 40C
					std::uint64_t unk410; // 410
					BGSLocalizedString unk418; // 418
					TESIcon unk428; // 428
				};
				static_assert(offsetof(AlchemyItem, unk3F8) == 0x3F8);
				static_assert(offsetof(AlchemyItem, flags) == 0x3FC);
				static_assert(offsetof(AlchemyItem, unk428) == 0x428);
				static_assert(sizeof(AlchemyItem) == 0x450);
			}
		}
	}
}
