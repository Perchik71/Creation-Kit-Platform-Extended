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
				// 1.11.240.0: size 0x1E8, vtable RVA 0x3099A10, 156 virtual functions
				class TESObjectMISC :
					public TESBoundObject,             // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public TESIcon,                    // 0F8
					public TESValueForm,               // 120
					public TESWeightForm,              // 130
					public BGSDestructibleObjectForm,  // 140
					public BGSMessageIcon,             // 150
					public BGSPickupPutdownSounds,     // 180
					public BGSKeywordForm,             // 198
					public BGSFeaturedItemMessage      // 1B8
				{
				public:
					virtual ~TESObjectMISC() = default;

					virtual void sub_4D0() { return; } // 154 - RVA 0xC4B350
					virtual void sub_4D8() { return; } // 155 - RVA 0xC4B340

				public:
					BSTArray<void*> componentData; // 1C8 - BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>*
					std::uint64_t unk1E0; // 1E0
				};
				static_assert(sizeof(TESObjectMISC) == 0x1E8);
			}
		}
	}
}
