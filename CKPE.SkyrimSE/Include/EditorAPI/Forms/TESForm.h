// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>
#include <EditorAPI/BaseFormComponent.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/BSTArray.h>
#include <EditorAPI/NiAPI/NiFlags.h>
#include <EditorAPI/TESFile.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct BSModInfo
				{
					TESFile** entries;
					std::uint32_t size;
				};

				class TESForm : public BaseFormComponent
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

					enum NavMeshGenerationImportOption : std::uint32_t
					{
						nmgiNone = 0,
						nmgiCollisionGeometry,
						nmgiBoundingBox,
						nmgiFilter,
						nmgiGround,
					};

					// 06
					struct BSTrackingData 
					{
						std::uint16_t date;			// 00 low byte is day of month, high byte is number of months with 1 = Jan. 
													// 2003 (Decembers are a little weird)
						std::uint16_t pad02;
						std::uint8_t lastUser;		// 04 userID that last had this form checked out
						std::uint8_t currentUser;	// 05 userID that has this form checked out

						// 2026: now BSTrackingData zeroed...
						inline std::uint8_t GetDay() const noexcept(true) { return (std::uint8_t)(date & 0xFF); }
						inline std::uint8_t GetMonth() const noexcept(true) { return (std::uint8_t)((date >> 8) % 12); }
						inline std::uint16_t GetYear() const noexcept(true) { return (std::uint8_t)((date >> 8) / 12) + 0x7D3; }
					};

					using Array = BSTArray<TESForm*>;
				protected:
					BSModInfo* _ModInfo;
					NiAPI::NiTFlags<std::uint32_t, TESForm> _FormFlags;
					std::uint32_t _FormID;
					char* _EditorID;
					BSTrackingData _TrackingData;
					FormType _FormType;
					char _pad1[0x1];
				public:
					virtual ~TESForm() = default;

					/* 070 */ virtual void sub_070();
					/* 078 */ virtual void sub_078();
					/* 080 */ virtual void sub_080();
					/* 088 */ virtual void sub_088();
					/* 090 */ virtual void sub_090();
					/* 098 */ virtual void sub_098();
					/* 0A0 */ virtual void sub_0A0();
					/* 0A8 */ virtual void sub_0A8();
					/* 0B0 */ virtual void sub_0B0();
					/* 0B8 */ virtual void sub_0B8();
					/* 0C0 */ virtual void sub_0C0();
					/* 0C8 */ virtual void sub_0C8();
					/* 0D0 */ virtual void sub_0D0();
					/* 0D8 */ virtual void sub_0D8();
					/* 0E0 */ virtual void sub_0E0();
					/* 0E8 */ [[nodiscard]] virtual TESFile* GetDescriptionOwnerFile() const noexcept(true);
					/* 0F0 */ [[nodiscard]] virtual FormType GetFormType_orig() const noexcept(true);
					/* 0F8 */ virtual void DebugInfo(char* Buffer, std::uint32_t BufferSize) const noexcept(true);
					/* 100 */ [[nodiscard]] virtual bool HasKnown() const noexcept(true);
					/* 108 */ [[nodiscard]] virtual bool HasRandomAnim() const noexcept(true);
					/* 110 */ [[nodiscard]] virtual bool HasPlayable() const noexcept(true);
					/* 118 */ [[nodiscard]] virtual bool IsHeadingMarker() const noexcept(true);
					/* 120 */ [[nodiscard]] virtual bool HasDangerous() const noexcept(true);
					/* 128 */ [[nodiscard]] virtual bool HasCurrents() const noexcept(true);
					/* 130 */ [[nodiscard]] virtual bool HasObstacle() const noexcept(true);
					/* 138 */ [[nodiscard]] virtual bool IsLODLandObject() const noexcept(true);
					/* 140 */ [[nodiscard]] virtual bool HasOnLocalMap() const noexcept(true);
					/* 148 */ [[nodiscard]] virtual bool HasMustUpdate() const noexcept(true);
					/* 150 */ [[nodiscard]] virtual bool SetOnLocalMap(bool bValue) noexcept(true);
					/* 158 */ [[nodiscard]] virtual bool HasIgnoredBySandbox() const noexcept(true);
					/* 160 */ virtual void MarkAsDeleted(bool Value = true) const noexcept(true);
					/* 168 */ virtual void MarkAsChanged(bool Value = true) const noexcept(true);
					/* 170 */ [[nodiscard]] virtual NavMeshGenerationImportOption GetNavMeshGenerationImportOption() const noexcept(true);
					/* 178 */ virtual void SaveObjectBound();
					/* 180 */ virtual void LoadObjectBound(TESFile* pFile);
					/* 188 */ [[nodiscard]] virtual bool IsBoundObject() const noexcept(true);
					/* 190 */ [[nodiscard]] virtual bool IsObject() const noexcept(true);
					/* 198 */ [[nodiscard]] virtual bool IsMagicItem() const noexcept(true);
					/* 1A0 */ [[nodiscard]] virtual bool IsWater() const noexcept(true);
					/* 1A8 */ [[nodiscard]] virtual TESForm* AsReference() noexcept(true);
					/* 1B0 */ [[nodiscard]] virtual const TESForm* AsReference() const noexcept(true);
					/* 1B8 */ virtual std::uint32_t GetRefCount() const noexcept(true);
					/* 1C0 */ virtual const char* GetTextForParsedSubTag(const BSFixedString& sSubTag) const noexcept(true);
					/* 1C8 */ virtual void CopyFrom(TESForm* Source) noexcept(true);
					/* 1D0 */ [[nodiscard]] virtual std::int32_t Compare(TESForm* Rhs) const noexcept(true);
					/* 1D8 */ virtual bool BelongsInGroup(TESForm* Group, bool bAllowParentGroups, bool bCurrentOnly);
					/* 1E0 */ virtual void CreateGroupData(TESForm* OutGroupForm, TESForm* ParentGroup);
					/* 1E8 */ [[nodiscard]] virtual const char* GetEditorID_orig() const noexcept(true);
					/* 1F0 */ virtual void sub_1F0();
					/* 1F8 */ virtual void sub_1F8();
					/* 200 */ virtual void sub_200();
					/* 208 */ virtual void sub_208();
					/* 210 */ virtual void sub_210();
					/* 218 */ virtual void sub_218();
					/* 220 */ virtual void sub_220();
					/* 228 */ virtual void sub_228();
					/* 230 */ virtual void sub_230();
					/* 238 */ virtual void sub_238();
					/* 240 */ virtual void sub_240();
					/* 248 */ virtual void sub_248();
					/* 250 */ virtual void sub_250();
					/* 258 */ virtual void sub_258();
					/* 260 */ virtual void sub_260();
					/* 268 */ virtual void sub_268();
					/* 270 */ virtual void sub_270();
					/* 278 */ virtual void sub_278();
					/* 280 */ virtual void sub_280();
					/* 288 */ virtual void sub_288();
					/* 290 */ [[nodiscard]] virtual bool HasRequiredEditorID() const noexcept(true);
					/* 298 */ [[nodiscard]] virtual bool DoVerificationEditorID(HWND hDialogBox) const noexcept(true);
					/* 2A0 */ virtual void sub_2A0();
					/* 2A8 */ virtual void sub_2A8();
					/* 2B0 */ virtual void sub_2B0();
					/* 2B8 */ virtual void sub_2B8();
					/* 2C0 */ virtual void sub_2C0();
					/* 2C8 */ virtual void sub_2C8();
					/* 2D0 */ virtual void EditFormToWindow(std::int64_t hParentWnd, 
						[[maybe_unused]] std::int64_t a1, [[maybe_unused]] std::int64_t a2) noexcept(true);
					/* 2D8 */ virtual void sub_2D8();
					/* 2E0 */ virtual void sub_2E0();
					/* 2E8 */ virtual void sub_2E8();
					/* 2F0 */ [[nodiscard]] virtual bool HasRequiredUniqueEditorID() const noexcept(true);
					/* 2F8 */ virtual void sub_2F8();
					/* 300 */ virtual bool WriteWidthTabToSettingFile([[maybe_unused]] std::int64_t nUnk, std::int32_t nWidth) noexcept(true);
					/* 308 */ [[nodiscard]] virtual std::int32_t ReadWidthTabFromSettingFile() const noexcept(true);
					/* 310 */ virtual void SetNewFormID(std::uint32_t NewIndex, bool Unk = true) noexcept(true);
					/* 318 */ virtual void sub_318();
					/* 320 */ virtual void sub_320();
					/* 328 */ virtual void sub_328();

					[[nodiscard]] inline std::uint32_t GetFormID() const noexcept(true) { return _FormID; }
					[[nodiscard]] inline FormType GetFormType() const noexcept(true) { return _FormType; }
					[[nodiscard]] inline const char* GetEditorID() const noexcept(true) { return _EditorID; }
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
				
					[[nodiscard]] inline const BSModInfo* GetModInfo() const noexcept(true) { return _ModInfo; }
					[[nodiscard]] inline BSTrackingData GetTrackingData() const noexcept(true) { return _TrackingData; }

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