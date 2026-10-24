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
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/B/BGSCharacterMorph.h>

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
				class BGSMovementType;
				class BGSTextureSet;
				class TESNPC;
				class BGSColorForm;
				class BGSHeadPart;
				class BGSMaterialType;
				class BGSArtObject;
				class BGSSoundDescriptorForm;
				class TESObjectWEAP;
				class TESQuest;

				// 1.11.246.0: size 0x78, an element of TESRace::speedOverrides (CommonLibF4 MOVEMENT_SPEED_OVERRIDE; the MTYP loader fills the
				// 0x70 byte speed block and stores the movement type form id in front of it)
				struct MovementSpeedOverride
				{
					BGSMovementType* moveType; // 00
					float overrideData[28]; // 08 - Movement::MaxSpeeds
				};
				static_assert(sizeof(MovementSpeedOverride) == 0x78);

				// 1.11.246.0: size 0xA0, allocated by TESRace Load/CopyFrom (constructor 0xB07200). The game's FaceRelatedData (0x68) holds the same
				// members; the CK object keeps the first nine as pointers to heap BSTArrays (names and element types from TESRace::CopyFrom, the Load
				// chunks TTGP/FTSF/RPRF/AHCF/HCLF/DFTF/FMRI and CommonLibF4's order) and then has a hash map in the CK's 0x38 byte layout.
				class FaceRelatedData
				{
				public:
					BGSCharacterTint::Template::Groups* tintingTemplate; // 00 - TTGP
					BSTArray<BGSTextureSet*>* faceDetailsTextureSets; // 08
					BGSTextureSet* defaultFaceDetailsTextureSet; // 10
					BSTArray<TESNPC*>* presetNPCs; // 18 - RPRF/RPRM
					BSTArray<BGSColorForm*>* availableHairColors; // 20 - AHCF/AHCM
					BGSColorForm* defaultHairColor; // 28 - HCLF
					BSTArray<BGSHeadPart*>* headParts; // 30
					BSTArray<BGSCharacterMorph::Group*>* morphGroups; // 38 - MPGN
					BSTArray<BGSCharacterMorph::FacialBoneRegion*>* facialBoneRegions; // 40 - FMRI
					// Same instantiation as TESNPC::morphSliderValues (BSTHashMap<std::uint32_t, float>); not copied by Copy and not part of the
					// game's FaceRelatedData.
					BSTHashMap<std::uint32_t, float> unk48; // 48
					BGSCharacterMorph::FacialBoneRegion* unk80; // 80 - by position the game's neckFatFacialBoneRegion
					NiAPI::NiPoint3 neckFatAdjustmentScales; // 88 - NTOP/NTOM
					BSFixedString wrinkleMapPath; // 98 - WMAP
				};
				static_assert(offsetof(FaceRelatedData, unk48) == 0x48);
				static_assert(offsetof(FaceRelatedData, unk80) == 0x80);
				static_assert(offsetof(FaceRelatedData, neckFatAdjustmentScales) == 0x88);
				static_assert(sizeof(FaceRelatedData) == 0xA0);

				// 1.11.246.0: size 0x70. Filled by the BSMS chunks (BSMB base scales, BSMM modifiers); both maps use the CK's 0x38 byte layout.
				class BodyMorphData
				{
				public:
					BSTHashMap<BSFixedString, BGSCharacterMorph::BoneBaseScales*> boneBaseScales; // 00
					BSTHashMap<BSFixedString, BGSCharacterMorph::BONE_MODIFIER_MIN_MAX*> boneModifiers; // 38
				};
				static_assert(sizeof(BodyMorphData) == 0x70);

				// 1.11.246.0: size 0x80, the object at TESRace + 0x400 (constructor 0x1406362A0): two qwords and two hash maps in the CK's 0x38 byte
				// layout (end of chain markers at 28 and 60 inside the object); key and value types are not identified.
				struct UnknownRaceTables
				{
					std::uint64_t unk00; // 00
					std::uint64_t unk08; // 08
					std::uint8_t tableA[0x38]; // 10
					std::uint8_t tableB[0x38]; // 48
				};
				static_assert(sizeof(UnknownRaceTables) == 0x80);

				// 1.11.246.0: size 0xC28, vtable RVA 0x304D8C8, 125 virtual functions
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

					virtual void UpdateRacePreview(HWND hDialogBox, bool ForceUpdate, void* Reference) noexcept(true) { return; } // 124

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
					BGSSoundTagComponent soundTagComponent; // 3D8
					BGSAttachParentArray attachParents; // 3E0
					UnknownRaceTables unk400; // 400 - CK only, constructed by 0x1406362A0 and not read by the form's own functions
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
					std::uint32_t pad6D4; // 6D4
					TESObjectWEAP* unarmedWeapon; // 6D8 - by order between validEquipTypes and armorParentRace
					TESRace* morphRace; // 6E0 - by order between validEquipTypes and armorParentRace
					TESRace* armorParentRace; // 6E8 - RNAM
					TESQuest* dialogueQuest; // 6F0 - by order after armorParentRace
					TESForm* sracForm; // 6F8 - SRAC chunk, form id resolved to a pointer
					TESForm* saddForm; // 700 - SADD chunk, form id resolved to a pointer
					BSTArray<MovementSpeedOverride> speedOverrides; // 708 - MTYP chunks (0x78 byte elements), by position like CommonLibF4's speedOverrides
					BSTArray<std::uint32_t> phonemeWeights[43]; // 720 - one PHWT chunk each (4 byte elements), saved right before the SRAC/SADD forms; CK only
					std::uint16_t tinl; // B28 - TINL chunk
					std::uint8_t padB2A[0x2]; // B2A
					std::uint32_t unkB2C; // B2C - zeroed with B30 by InitializeData/ClearData
					std::uint32_t mlsi; // B30 - MLSI chunk
					std::uint32_t unkB34; // B34
					BSTArray<BSFixedString> phonemeTargets; // B38 - 8 byte string elements released by ClearData, by position like CommonLibF4's phonemeTargets
					BSTHashMap<std::uint32_t, void*> morphSliders; // B50 - value BGSCharacterMorph::Slider*, by position after phonemeTargets (end-of-chain marker DEADBEEF at B68, entries pointer at B80)
					BGSMovementType* baseMoveTypes[4]; // B88 - zeroed by InitializeData, by position like CommonLibF4's baseMoveTypes
					FaceRelatedData* faceRelatedData[2]; // BA8 - heap objects of 160 bytes released by ClearData
					BodyMorphData* bodyMorphData[2]; // BB8 - heap objects of 112 bytes released by ClearData
					std::uint64_t unkBC8; // BC8
					std::uint64_t unkBD0; // BD0
					TESTexture hairColorLookupTexture; // BD8
					TESTexture hairColorLookupTextureExt; // C00
				};
				static_assert(offsetof(TESRace, unk400) == 0x400);
				static_assert(offsetof(TESRace, phonemeWeights) == 0x720);
				static_assert(offsetof(TESRace, mlsi) == 0xB30);
				static_assert(offsetof(TESRace, phonemeTargets) == 0xB38);
				static_assert(offsetof(TESRace, morphSliders) == 0xB50);
				static_assert(offsetof(TESRace, baseMoveTypes) == 0xB88);
				static_assert(offsetof(TESRace, faceRelatedData) == 0xBA8);
				static_assert(offsetof(TESRace, hairColorLookupTexture) == 0xBD8);
				static_assert(sizeof(TESRace) == 0xC28);
			}
		}
	}
}
