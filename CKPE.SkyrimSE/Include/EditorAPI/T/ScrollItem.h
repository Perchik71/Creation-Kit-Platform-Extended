// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/SpellItem.h>
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
				// 1.7.99.0: size 0x368, vtable RVA 0x312B8C8
				class ScrollItem :
					public SpellItem,	// 00
					public TESModelTextureSwap,	// 2D8
					public BGSDestructibleObjectForm,	// 320
					public BGSPickupPutdownSounds,	// 330
					public TESWeightForm,	// 348
					public TESValueForm	// 358
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftScrollItem;

					virtual ~ScrollItem() = default;
				};
				static_assert(sizeof(ScrollItem) == 0x368);
			}
		}
	}
}