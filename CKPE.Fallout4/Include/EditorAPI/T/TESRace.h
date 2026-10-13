// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/T/TESSpellList.h>
#include <EditorAPI/B/BGSSkinForm.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSAttackDataForm.h>
#include <EditorAPI/B/BGSPropertySheet.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/BGSSoundTagComponent.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSTextureModel.h>
#include <EditorAPI/B/BGSBehaviorGraphModel.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSVoiceType;
				class BGSBodyPartData;
				class ActorValueInfo;


				class BGSMaterialType;
				class BGSArtObject;
				class BGSTextureSet;
				class BGSSoundDescriptorForm;
				class TESObjectWEAP;
				class TESQuest;

				// 1.11.240.0: size 0xC28, vtable RVA 0x304C8C8, 125 virtual functions
				class TESRace :
					public TESForm,             // 000
					public TESFullName,         // 028
					public TESDescription,      // 040
					public TESSpellList,        // 068
					public BGSSkinForm,         // 078
					public BGSBipedObjectForm,  // 090
					public BGSKeywordForm,      // 0A0
					public BGSAttackDataForm,   // 0C0
					public BGSPropertySheet,    // 0D0
					public BGSPreloadable       // 0E0
				{
				public:
					virtual ~TESRace() = default;

					virtual void sub_3E0() { return; } // 124 - RVA 0xB1C9E0

				public:
					TESModel skeletonModel[2]; // E8
					TESModel skeletonChargenModel[2]; // 158
					std::uint8_t data[0x100]; // 1C8 - RACE_DATA
					float clampFaceGeoValue; // 2C8
					float clampFaceGeoValue2; // 2CC
					BGSTextureModel bodyTextureModels[2]; // 2D0
					BGSBehaviorGraphModel behaviorGraph[2]; // 340
					BSFixedString rootBehaviorGraphName[2]; // 3B0 - the CK has a single name array (no behaviorGraphProjectName)
					BGSVoiceType* defaultVoiceType[2]; // 3C0
					BGSBodyPartData* bodyPartData; // 3D0
					BGSSoundTagComponent unk3D8; // 3D8
					BGSAttachParentArray unk3E0; // 3E0
					std::uint8_t unk400[0x80]; // 400
					std::uint64_t unk480; // 480
					BGSMaterialType* bloodImpactMaterial; // 488 - by order before meatCapTextureSet
					BGSArtObject* dismemberBlood; // 490 - by order before meatCapTextureSet
					BGSTextureSet* meatCapTextureSet; // 498 - CNAM
					BGSTextureSet* meatCollarTextureSet; // 4A0 - by order after meatCapTextureSet
					BGSSoundDescriptorForm* corpseOpenSound; // 4A8 - ONAM
					BGSSoundDescriptorForm* corpseCloseSound; // 4B0 - LNAM
					BSFixedString bipedObjectNames[32]; // 4B8
					ActorValueInfo* bipedObjectConditions[32]; // 5B8
					BSTArray<void*> equipSlotArray; // 6B8 - BSTArray<EquipSlotData>
					std::uint32_t validEquipTypes; // 6D0 - VNAM
					std::uint32_t unk6D4; // 6D4
					TESObjectWEAP* unarmedWeapon; // 6D8 - by order between validEquipTypes and armorParentRace
					TESRace* morphRace; // 6E0 - by order between validEquipTypes and armorParentRace
					TESRace* armorParentRace; // 6E8 - RNAM
					TESQuest* dialogueQuest; // 6F0 - by order after armorParentRace
					std::uint8_t unk6F8[0x10]; // 6F8
					BSTArray<void*> unk708; // 708 - element type not identified yet
					std::uint8_t unk720[0x418]; // 720
					BSTArray<void*> unkB38; // B38 - element type not identified yet
					std::uint8_t unkB50[0xC]; // B50
					std::uint8_t unkB5C[0x8]; // B5C
					std::uint32_t unkB64; // B64
					std::uint64_t unkB68; // B68
					std::uint32_t unkB70; // B70
					std::uint8_t unkB74[0xC]; // B74
					std::uint64_t unkB80; // B80
					std::uint8_t unkB88[0x40]; // B88
					std::uint64_t unkBC8; // BC8
					std::uint64_t unkBD0; // BD0
					TESTexture hairColorLookupTexture; // BD8
					TESTexture hairColorLookupTextureExt; // C00
				};
				static_assert(sizeof(TESRace) == 0xC28);
			}
		}
	}
}
