// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObject.h>
#include <EditorAPI/T/TESRaceForm.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;
				class BGSListForm;

				class BGSArtObject;

				class BGSFootstepSet;

				struct OBJ_ARMA
				{
					std::int8_t priorities[2]; // 00
					std::int8_t modelRange[2]; // 02
					std::int8_t unused[2]; // 04
					std::int8_t soundValue; // 06
					float weaponAdjust; // 08
				};
				static_assert(sizeof(OBJ_ARMA) == 0xC);

				// 1.11.240.0: size 0x298, vtable RVA 0x30939E8, 136 virtual functions
				class TESObjectARMA :
					public TESObject,           // 000
					public TESRaceForm,         // 030
					public BGSBipedObjectForm   // 040
				{
				public:
					virtual ~TESObjectARMA() = default;

				public:
					OBJ_ARMA data; // 50
					std::uint32_t unk5C; // 5C
					BGSModelMaterialSwap bipedModel[2]; // 60
					BGSModelMaterialSwap bipedModelFacebones[2]; // 100
					BGSModelMaterialSwap bipedModel1stPerson[2]; // 1A0
					BGSTextureSet* skinTextures[2]; // 240
					BGSListForm* skinTextureSwapLists[2]; // 250
					BSTArray<void*> additionalRaces; // 260 - BSTArray<TESRace*>
					BGSFootstepSet* footstepSet; // 278
					BGSArtObject* artObject; // 280
					std::uint64_t unk288; // 288
					std::uint64_t unk290; // 290
				};
				static_assert(sizeof(TESObjectARMA) == 0x298);
			}
		}
	}
}
