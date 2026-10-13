// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSFeaturedItemMessage.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectSTAT;

				// 1.11.240.0: size 0x248, vtable RVA 0x3095FA8, 154 virtual functions
				class TESObjectBOOK :
					public TESBoundObject,             // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public TESIcon,                    // 0F8
					public TESValueForm,               // 120
					public TESWeightForm,              // 130
					public TESDescription,             // 140
					public BGSDestructibleObjectForm,  // 168
					public BGSMessageIcon,             // 178
					public BGSPickupPutdownSounds,     // 1A8
					public BGSKeywordForm,             // 1C0
					public BGSFeaturedItemMessage      // 1E0
				{
				public:
					virtual ~TESObjectBOOK() = default;

				public:
					std::uint8_t data[0x18]; // 1F0 - OBJ_BOOK
					TESObjectSTAT* inventoryModel; // 208
					std::uint64_t unk210; // 210
					std::uint64_t unk218; // 218
					TESDescription unk220; // 220
				};
				static_assert(sizeof(TESObjectBOOK) == 0x248);
			}
		}
	}
}
