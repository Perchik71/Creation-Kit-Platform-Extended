// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>
#include <EditorAPI/BSTArray.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm
				{
				public:
					enum FormType : std::uint8_t
					{
						ftReserved000 = 0,				//	NONE
						ftReserved001,					//	TES4
						ftReserved002,					//	GRUP	
						ftReserved003,					//	GMST
						ftKeyword,						//	KYWD	BGSKeyword
						ftLocationRef,					//	LCRT	BGSLocationRefType
						ftAction,						//	AACT	BGSAction
						ftTextureSet,					//	TXST	BGSTextureSet
						ftMenuIcon,						//	MICN	BGSMenuIcon
						ftGlobal,						//	GLOB	TESGlobal
						ftClass,						//	CLAS	TESClass
						ftFaction,						//	FACT	TESFaction
						ftHeadPart,						//	HDPT	BGSHeadPart
						ftEyes,							//	EYES	TESEyes
						ftRace,							//	RACE	TESRace
						ftSound,						//	SOUN	TESSound
						ftAcousticSpace,				//	ASPC	BGSAcousticSpace
						ftSkill,						//	SKIL	? unused ?
						ftEffectSetting,				//	MGEF	EffectSetting
						ftScript,						//	SCPT	Script
						ftLandTexture,					//	LTEX	TESLandTexture
						ftEnchantment,					//	ENCH	EnchantmentItem
						ftSpell,						//	SPEL	SpellItem
						ftScrollItem,					//	SCRL	ScrollItem
						ftActivator,					//	ACTI	TESObjectACTI
						ftTalkingActivator,				//	TACT	BGSTalkingActivator
						ftArmor,						//	ARMO	TESObjectARMO
						ftBook,							//	BOOK	TESObjectBOOK
						ftContainer,					//	CONT	TESObjectCONT
						ftDoor,							//	DOOR	TESObjectDOOR
						ftIngredient,					//	INGR	IngredientItem
						ftLight,						//	LIGH	TESObjectLIGH
						ftMisc,							//	MISC	TESObjectMISC
						ftApparatus,					//	APPA	BGSApparatus
						ftStatic,						//	STAT	TESObjectSTAT
						ftStaticCollection,				//	SCOL	BGSStaticCollection
						ftMovableStatic,				//	MSTT	BGSMovableStatic
						ftGrass,						//	GRAS	TESGrass
						ftTree,							//	TREE	TESObjectTREE
						ftFlora,						//	FLOR	TESFlora
						ftFurniture,					//	FURN	TESFurniture
						ftWeapon,						//	WEAP	TESObjectWEAP
						ftAmmo,							//	AMMO	TESAmmo
						ftNPC,							//	NPC_	TESNPC
						ftLeveledCharacter,				//	LVLN	TESLevCharacter
						ftKey,							//	KEYM	TESKey
						ftPotion,						//	ALCH	AlchemyItem
						ftIdleMarker,					//	IDLM	BGSIdleMarker
						ftNote,							//	NOTE	BGSNote
						ftConstructibleObject,			//	COBJ	BGSConstructibleObject
						ftProjectile,					//	PROJ	BGSProjectile
						ftHazard,						//	HAZD	BGSHazard
						ftSoulGem,						//	SLGM	TESSoulGem
						ftLeveledItem,					//	LVLI	TESLevItem
						ftWeather,						//	WTHR	TESWeather
						ftClimate,						//	CLMT	TESClimate
						ftSPGD,							//	SPGD	BGSShaderParticleGeometryData
						ftReferenceEffect,				//	RFCT	BGSReferenceEffect
						ftRegion,						//	REGN	TESRegion
						ftNavMeshInfo,					//	NAVI	NavMeshInfoMap
						ftCell,							//	CELL	TESObjectCELL
						ftReference,					//	REFR	TESObjectREFR
						ftCharacter,					//	ACHR	Character
						ftMissile,						//	PMIS	MissileProjectile
						ftArrow,						//	PARW	ArrowProjectile
						ftGrenade,						//	PGRE	GrenadeProjectile
						ftBeamProj,						//	PBEA	BeamProjectile
						ftFlameProj,					//	PFLA	FlameProjectile
						ftConeProj,						//	PCON	ConeProjectile
						ftBarrierProj,					//	PBAR	BarrierProjectile
						ftPHZD,							//	PHZD	Hazard
						ftWorldSpace,					//	WRLD	TESWorldSpace
						ftLandspace,					//	LAND	TESObjectLAND
						ftNavMesh,						//	NAVM	NavMesh
						ftTLOD,							//	TLOD	?
						ftTopic,						//	DIAL	TESTopic
						ftTopicInfo,					//	INFO	TESTopicInfo
						ftQuest,						//	QUST	TESQuest
						ftIdle,							//	IDLE	TESIdleForm
						ftPackage,						//	PACK	TESPackage
						ftCombatStyle,					//	CSTY	TESCombatStyle
						ftLoadScreen,					//	LSCR	TESLoadScreen
						ftLeveledSpell,					//	LVSP	TESLevSpell
						ftANIO,							//	ANIO	TESObjectANIO
						ftWater,						//	WATR	TESWaterForm
						ftEffectShader,					//	EFSH	TESEffectShader
						ftTOFT,							//	TOFT	?
						ftExplosion,					//	EXPL	BGSExplosion
						ftDebris,						//	DEBR	BGSDebris
						ftImageSpace,					//	IMGS	TESImageSpace
						ftImageSpaceMod,				//	IMAD	TESImageSpaceModifier
						ftFormList,						//	FLST	BGSListForm
						ftPerk,							//	PERK	BGSPerk
						ftBodyPartData,					//	BPTD	BGSBodyPartData
						ftAddonNode,					//	ADDN	BGSAddonNode
						ftActorValueInfo,				//	AVIF	ActorValueInfo
						ftCameraShot,					//	CAMS	BGSCameraShot
						ftCameraPath,					//	CPTH	BGSCameraPath
						ftVoiceType,					//	VTYP	BGSVoiceType
						ftMaterialType,					//	MATT	BGSMaterialType
						ftImpactData,					//	IPCT	BGSImpactData
						ftImpactDataSet,				//	IPDS	BGSImpactDataSet
						ftARMA,							//	ARMA	TESObjectARMA
						ftEncounterZone,				//	ECZN	BGSEncounterZone
						ftLocation,						//	LCTN	BGSLocation
						ftMessage,						//	MESH	BGSMessage
						ftRagdoll,						//	RGDL	BGSRagdoll
						ftDOBJ,							//	DOBJ	? (used for default objects, custom loader)
						ftLightingTemplate,				//	LGTM	BGSLightingTemplate
						ftMusicType,					//	MUSC	BGSMusicType
						ftFootstep,						//	FSTP	BGSFootstep
						ftFootstepSet,					//	FSTS	BGSFootstepSet
						ftStoryBranchNode,				//	SMBN	BGSStoryManagerBranchNode
						ftStoryQuestNode,				//	SMQN	BGSStoryManagerQuestNode
						ftStoryEventNode,				//	SMEN	BGSStoryManagerEventNode
						ftDialogueBranch,				//	DLBR	BGSDialogueBranch
						ftMusicTrack,					//	MUST	BGSMusicTrackFormWrapper
						ftDLVW,							//	DLVW	?
						ftWordOfPower,					//	WOOP	TESWordOfPower
						ftShout,						//	SHOU	TESShout
						ftEquipSlot,					//	EQUP	BGSEquipSlot
						ftRelationship,					//	RELA	BGSRelationship
						ftScene,						//	SCEN	BGSScene
						ftAssociationType,				//	ASTP	BGSAssociationType
						ftOutfit,						//	OTFT	BGSOutfit
						ftArt,							//	ARTO	BGSArtObject
						ftMaterial,						//	MATO	BGSMaterialObject
						ftMovementType,					//	MOVT	BGSMovementType
						ftSoundDescriptor,				//	SNDR	BGSSoundDescriptorForm
						ftDualCastData,					//	DUAL	BGSDualCastData
						ftSoundCategory,				//	SNCT	BGSSoundCategory
						ftSoundOutput,					//	SOPM	BGSSoundOutput
						ftCollisionLayer,				//	COLL	BGSCollisionLayer
						ftColorForm,					//	CLFM	BGSColorForm
						ftReverbParam,					//	REVB	BGSReverbParameters
						ftLensFlare,					//	LENS
						ftUnk88,						//	LSPR
						ftVolumetricLighting,			//	VOLI
						ftMax = ftVolumetricLighting	//	max of standard types
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
						std::uint16_t date;        // 00 low byte is day of month, high byte is number of months with 1 = Jan. 
						// 2003 (Decembers are a little weird)
						std::uint16_t pad02;
						std::uint8_t lastUser;     // 04 userID that last had this form checked out
						std::uint8_t currentUser;  // 05 userID that has this form checked out
					};

					using Array = BSTArray<TESForm*>;
				protected:
					char _pad0[0x8];
					std::uint32_t _FormFlags;
					std::uint32_t _FormID;
					char* _EditorID;
					BSTrackingData _TrackingData;
					FormType _FormType;
					char _pad1[0x1];
				public:
					virtual ~TESForm() = default;

					void MarkAsDeleted(bool Value = true) const noexcept(true);
					void MarkAsChanged(bool Value = true) const noexcept(true);
					void EditFormToWindow(std::int64_t hParentWnd, __int64 a1, __int64 a2) noexcept(true);
					void SetNewFormID(std::uint32_t NewIndex, bool Unk = true) noexcept(true);
					bool CanPreview() const noexcept(true);
					void DebugInfo(char* Buffer, std::uint32_t BufferSize) const noexcept(true);

					[[nodiscard]] inline std::uint32_t GetFormID() const noexcept(true) { return _FormID; }
					[[nodiscard]] inline FormType GetFormType() const noexcept(true) { return _FormType; }
					[[nodiscard]] inline const char* GetEditorID() const noexcept(true) { return _EditorID; }
					// Not all forms have a localized name, but I'll add it here to simplify my life,
					// since not all types of forms are known to me.
					[[nodiscard]] const char* TryGetFullName() const noexcept(true);

					[[nodiscard]] inline bool IsFromMaster() const noexcept(true) { return (_FormFlags & FormFlags::fsMaster); }
					[[nodiscard]] inline bool IsModified() const noexcept(true) { return (_FormFlags & FormFlags::fsModified); }
					[[nodiscard]] inline bool IsLinked() const noexcept(true) { return (_FormFlags & FormFlags::fsLinked); }
					[[nodiscard]] inline bool IsDeleted() const noexcept(true) { return (_FormFlags & FormFlags::fsDeleted); }
					[[nodiscard]] inline bool IsInitialDisabled() const noexcept(true) { return (_FormFlags & FormFlags::fsInitialDisabled); }
					[[nodiscard]] inline bool IsIgnored() const noexcept(true) { return (_FormFlags & FormFlags::fsIgnored); }
					[[nodiscard]] inline bool IsTemporary() const noexcept(true) { return (_FormFlags & FormFlags::fsTemporary); }
					[[nodiscard]] inline bool IsQuestItem() const noexcept(true) { return (_FormFlags & FormFlags::fsQuestItem); }
				
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