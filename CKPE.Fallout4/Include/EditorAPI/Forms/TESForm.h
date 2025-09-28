// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>
#include <EditorAPI/BSTArray.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/NiAPI/NiFlags.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct BSModInfo 
				{
					void* entries;
					std::uint32_t size;
				};

				class TESForm
				{
				public:
					enum FormType : std::uint8_t
					{
						ftReserved000 = 0,			// NONE
						ftReserved001,				// TES4
						ftReserved002,				// GRUP
						ftReserved003,				// GMST
						ftKeyword,
						ftLocationRefType,
						ftAction,
						ftTransform,				// TRNS
						ftReserved008,				// CMPO
						ftTextureSet,
						ftMenuIcon,
						ftGlobal,
						ftDamageType,				// DMGT
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
						ftBendableSpline,			// BNDS
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
						ftReference,				// 0x40
						ftCharacter,				// 0x41
						ftMissile,					// 0x42
						ftArrow,					// 0x43
						ftGrenade,					// 0x44
						ftBeam,						// 0x45
						ftFlame,					// 0x46
						ftCone,						// 0x47
						ftBarrier,					// 0x48
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
						ftDefaultObjectManager,
						ftDefaultObject,
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
						ftReferenceGroup,
						ftAimModel,
						ftLayer,
						ftConstructibleObject,
						ftAttachmentMod,
						ftMaterialSwap,
						ftAimDownSightModel,
						ftInstanceNamingRules,
						ftSoundKeywordMapping,
						ftReserved149,				// AECH
						ftReserved150,				// SCCO
						ftAttractionRule,
						ftReserved152,				// SCSN
						ftSoundTagSet,
						ftNavMeshObstacleCoverManager,
						ftLensFlare,
						ftReserved156,				// LSPR
						ftReserved157,				// GDRY
						ftObjectVisibilityManager,
						ftMax
					};

					// Form State
					enum FormFlags : std::uint32_t
					{
						fsMaster			= /*00*/ 0x1,			// form is from an esm file
						fsModified			= /*01*/ 0x2,			// form is overriden by active mod or save file
						fsLinked			= /*03*/ 0x8,			// set after formids have been resolved into TESForm*
						fsDeleted			= /*05*/ 0x20,			// set on deletion, not saved in CK
						fsQuestItem			= /*0A*/ 0x400,			// aka Persistent Reference for TESObjectREFR (from CSE)
						fsInitialDisabled	= /*0B*/ 0x800,			// Disable TESObjectREFR
						fsIgnored			= /*0C*/ 0x1000,		// ?? (from CSE)
						fsTemporary			= /*0E*/ 0x4000,		// not saved in CK
					};

					// 06
					struct BSTrackingData 
					{
						std::uint16_t date;			// 00 low byte is day of month, high byte is number of months with 1 = Jan. 
													// 2003 (Decembers are a little weird)
						std::uint16_t pad02;
						std::uint8_t lastUser;		// 04 userID that last had this form checked out
						std::uint8_t currentUser;	// 05 userID that has this form checked out
					};

					using Array = BSTArray<TESForm*>;
				protected:
					BSModInfo* _ModInfo;											// 08-10
					NiAPI::NiTFlags<std::uint32_t, TESForm> _FormFlags;				// 10-14
					std::uint32_t _FormID;											// 14-18
					BSFixedString _EntryEditID;										// 18-20
					BSTrackingData _TrackingData;									// 20-26
					FormType _FormType;												// 26-27
					char pad27[0x1];												// 27-28
				public:
					virtual ~TESForm() = default;

					void MarkAsDeleted(bool Value = true) const noexcept(true);
					void MarkAsChanged(bool Value = true) const noexcept(true);
					void EditFormToWindow(std::int64_t hParentWnd, std::int64_t a1, std::int64_t a2) noexcept(true);
					void SetNewFormID(std::uint32_t NewIndex, bool Unk = true) noexcept(true);
					bool CanPreview() const noexcept(true);
					void DebugInfo(char* Buffer, std::uint32_t BufferSize) const noexcept(true);

					[[nodiscard]] inline std::uint32_t GetFormID() const noexcept(true) { return _FormID; }
					[[nodiscard]] inline FormType GetFormType() const noexcept(true) { return _FormType; }
					[[nodiscard]] inline const char* GetEditorID() const noexcept(true) { return _EntryEditID; }
					[[nodiscard]] const char* GetEditorID_orig() const noexcept(true);
					// Not all forms have a localized name, but I'll add it here to simplify my life,
					// since not all types of forms are known to me.
					[[nodiscard]] const char* TryGetFullName() const noexcept(true);

					[[nodiscard]] inline bool IsFromMaster() const noexcept(true) { return _FormFlags.Has(fsMaster); }
					[[nodiscard]] inline bool IsModified() const noexcept(true) { return _FormFlags.Has(fsModified); }
					[[nodiscard]] inline bool IsLinked() const noexcept(true) { return _FormFlags.Has(fsLinked); }
					[[nodiscard]] inline bool IsDeleted() const noexcept(true) { return _FormFlags.Has(fsDeleted); }
					[[nodiscard]] inline bool IsInitialDisabled() const noexcept(true) { return _FormFlags.Has(fsInitialDisabled); }
					[[nodiscard]] inline bool IsIgnored() const noexcept(true) { return _FormFlags.Has(fsIgnored); }
					[[nodiscard]] inline bool IsTemporary() const noexcept(true) { return _FormFlags.Has(fsTemporary); }
					[[nodiscard]] inline bool IsQuestItem() const noexcept(true) { return _FormFlags.Has(fsQuestItem); }
				
					// Inaccurate, because there is a REFR class for this and it is of its own type (taken from SSE)
					[[nodiscard]] inline bool IsReference() const noexcept(true)
					{ 
						return
							(_FormType == FormType::ftReference) || 
							(_FormType == FormType::ftCharacter); 
					}

					inline static TESForm* (*FindFormByFormID)(std::uint32_t);

					CKPE_READ_PROPERTY(IsModified) bool Active;
					CKPE_READ_PROPERTY(GetFormID) std::uint32_t FormID;
					CKPE_READ_PROPERTY(GetEditorID) const char* EditorID;
					CKPE_READ_PROPERTY(TryGetFullName) const char* FullName;
					CKPE_READ_PROPERTY(GetFormType) FormType Type;
				};	
				static_assert(sizeof(TESForm) == 0x28);

				typedef TESForm::Array			TESFormArray;
				typedef BSTArray<std::uint32_t>	TESFormIDArray;
			}
		}
	}
}