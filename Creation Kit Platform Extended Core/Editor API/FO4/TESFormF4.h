//////////////////////////////////////////
/*
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 1)

#include "TESFileF4.h"
#include "BSEntryString.h"
#include "TESFormComponents.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESForm;

			typedef BSTArray<TESForm*>	TESFormArray;
			typedef BSTArray<uint32_t>	TESFormIDArray;

			class TESForm : public TESPersistent {
			public:
				// Form Type
				enum FormType : uint8_t {
					ftReserved000 = 0,			// NONE
					ftReserved001,				// TES4
					ftReserved002,				// GRUP
					ftReserved003,				// GMST
					ftKeyword,
					ftLocationRefType,
					ftAction,
					ftReserved007,				// TRNS
					ftReserved008,				// CMPO
					ftTextureSet,
					ftMenuIcon,
					ftGlobal,
					ftReserved012,				// DMGT
					ftClass,
					ftFaction,
					ftHeadPart,
					ftEyes,
					ftRace,
					ftSoundMarker,
					ftAcousticSpace,
					ftReserved020,				// SKIL
					ftEffectSetting,
					ftScript,
					ftLandTexture,
					ftEnchantment,
					ftSpell,
					ftScroll,
					ftActivator,
					ftTalkingActivator,
					ftArmor,
					ftBook,
					ftContainer,
					ftDoor,
					ftIngredient,
					ftLight,
					ftMiscItem,
					ftStatic,
					ftStaticCollection,
					ftMovableStatic,
					ftGrass,
					ftTree,
					ftFlora,
					ftFurniture,
					ftWeapon,
					ftAmmo,
					ftActor,
					ftLevelCharacter,
					ftKey,
					ftPotion,
					ftIdleMarker,
					ftNote,
					ftProjectile,
					ftHazard052,
					ftReserved053,				// BNDS
					ftSoulGem,
					ftTerminal,
					ftLeveledItem,
					ftWeather,
					ftClimate,
					ftShaderParticleGeometryData,
					ftReferenceEffect,
					ftRegion,
					ftNavMeshInfoMap,
					ftCell,
					ftReference,
					ftCharacter,
					ftMissile,
					ftArrow,
					ftGrenade,
					ftBeam,
					ftFlame,
					ftCone,
					ftBarrier,
					ftHazard073,				// Hazard ?
					ftWorldSpace,
					ftLandspace,
					ftNavMesh,
					ftReserved077,				// TLOD
					ftTopic,
					ftTopicInfo,
					ftQuest,
					ftIdle,
					ftPackage,
					ftCombatStyle,
					ftLoadScreen,
					ftLeveledSpell,
					ftUnknown086,				// ANIO
					ftWater,
					ftEffectShader,
					ftReserved089,				// TOFT
					ftExplosion,
					ftDebris,
					ftImageSpace,
					ftImageSpaceModifier,
					ftListForm,
					ftPerk,
					ftBodyPartData,
					ftAddonNode,
					ftActorValueInfo,
					ftCameraShot,
					ftCameraPath,
					ftVoice,
					ftMaterial,
					ftImpactData,
					ftImpactDataSet,
					ftReserved105,				// ARMA
					ftEncounterZone,
					ftLocation,
					ftMessage,
					ftRagdoll,
					ftReserved110,				// DOBJ
					ftReserved111,				// DFOB
					ftLightingTemplate,
					ftMusic,
					ftFootstep,
					ftFootstepSet,
					ftStoryManagerBranchNode,
					ftStoryManagerQuestNode,
					ftStoryManagerEventNode,
					ftDialogue,
					ftMusicTrackFormWrapper,
					ftReserved121,				// DLVW
					ftWordOfPower,
					ftShout,
					ftEquipSlot,
					ftRelationship,
					ftScene,
					ftAssociationType,
					ftOutfit,
					ftArt,
					ftMaterialObject,
					ftMovementType,
					ftSoundDescriptorForm,
					ftDualCastData,
					ftSoundCategory,
					ftSoundOutput,
					ftCollisionLayer,
					ftColorForm,
					ftReverbParameters,
					ftReserved139,				// PKIN
					ftReserved140,				// RFGP
					ftReserved141,				// AMDL
					ftLayer,
					ftReserved143,				// COBJ
					ftReserved144,				// OMOD
					ftMaterialSwap,
					ftReserved146,				// ZOOM
					ftReserved147,				// INNR
					ftReserved148,				// KSSM
					ftReserved149,				// AECH
					ftReserved150,				// SCCO
					ftReserved151,				// AORU
					ftReserved152,				// SCSN
					ftReserved153,				// STAG
					ftReserved154,				// NOCM
					ftReserved155,				// LENS
					ftReserved156,				// LSPR
					ftReserved157,				// GDRY
					ftReserved158,				// OVIS
					ftMax
				};

				// Form State
				enum FormFlags : uint32_t {
					fsMaster				= /*00*/ 0x1,			// form is from an esm file
					fsModified				= /*01*/ 0x2,			// form is overriden by active mod or save file
					fsLinked				= /*03*/ 0x8,			// set after formids have been resolved into TESForm*
					fsDeleted				= /*05*/ 0x20,			// set on deletion, not saved in CK
					fsQuestItem				= /*0A*/ 0x400,			// aka Persistent Reference for TESObjectREFR (from CSE)
					fsInitialDisabled		= /*0B*/ 0x800,			// Disable TESObjectREFR
					fsIgnored				= /*0C*/ 0x1000,		// ?? (from CSE)
					fsTemporary				= /*0E*/ 0x4000,		// not saved in CK
					fsVisibleWhenDistant	= /*0F*/ 0x8000,		// ?? (from CSE)
				};
			protected:
				// 06
				struct BSTrackingData {
					uint16_t date;        // 00 low byte is day of month, high byte is number of months with 1 = Jan. 2003 (Decembers are a little weird)
					uint16_t pad02;
					uint8_t lastUser;     // 04 userID that last had this form checked out
					uint8_t currentUser;  // 05 userID that has this form checked out
				};
			protected:
				BSModInfo* _modinfo;											// 08-10
				uint32_t _FormFlags;											// 10-14
				uint32_t _FormID;												// 14-18
				BSEntryString* _EntryEditID;									// 18-20
				BSTrackingData _trackingData;									// 20-26
				FormType _type;													// 26-27
				char pad27[0x1];												// 27-28
			public:
				void LoadForm(TESFile* file);									// vtbl->0D0
				void SaveForm(TESFile* file);									// vtbl->0D8
				FormType GetFormType() const;									// vtbl->120
				void DebugInfo(char* buffer, uint32_t dwSize) const;			// vtbl->128
				void MarkAsDeleted(bool state = true);							// vtbl->198 
				void MarkAsChanged(bool state = true);							// vtbl->1A0 
				uint32_t GetEditIDLength() const;								// vtbl->230
				const char* GetEditorID() const;								// vtbl->238
				const char* GetFullName() const;
				const char* GetFormTypeShortStr() const;
			public:
				// Deeply inaccurate, because there is a REFR class for this and it is of its own type (taken from SSE)
				inline bool IsReference() const { return (_type == FormType::ftReference) || (_type == FormType::ftCharacter); }
			public:
				inline bool IsFromMaster() const { return (_FormFlags & FormFlags::fsMaster); }
				inline bool IsModified() const { return (_FormFlags & FormFlags::fsModified); }
				inline bool IsLinked() const { return (_FormFlags & FormFlags::fsLinked); }
				inline bool IsDeleted() const { return (_FormFlags & FormFlags::fsDeleted); }
				inline bool IsInitialDisabled() const { return (_FormFlags & FormFlags::fsInitialDisabled); }
				inline bool IsIgnored() const { return (_FormFlags & FormFlags::fsIgnored); }
				inline bool IsTemporary() const { return (_FormFlags & FormFlags::fsTemporary); }
				inline bool IsQuestItem() const { return (_FormFlags & FormFlags::fsQuestItem); }
				inline bool IsVisibleWhenDistant() const { return (_FormFlags & FormFlags::fsVisibleWhenDistant); }
			public:
				BSString GetID() const;
				inline uint32_t GetFormFlags() const { return _FormFlags; }
				inline uint32_t GetFormID() const { return _FormID; }
				inline const char* GetEditorID_NoVTable() const { return _EntryEditID ? _EntryEditID->Get<char>(TRUE) : nullptr; }
			public:
				READ_PROPERTY(IsModified) bool Active;
				READ_PROPERTY(GetFormID) uint32_t FormID;
				READ_PROPERTY(GetFormFlags) uint32_t FormFlag;
				READ_PROPERTY(GetEditorID) const char* EditorID;
				READ_PROPERTY(GetFullName) const char* FullName;
				READ_PROPERTY(GetFormType) FormType Type;
				READ_PROPERTY(GetFormTypeShortStr) const char* TypeStr;
			};
			static_assert(sizeof(TESForm) == 0x28, "TESForm class should be the size of 0x28");

			TESForm* GetFormByNumericID(const uint32_t SearchID);
		}
	}
}

#pragma pack(pop)