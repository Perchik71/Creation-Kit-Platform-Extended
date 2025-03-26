// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/TESForm_CK.h

#pragma once

#include "../BSTArray.h"
#include "../BSString.h"
#include "BSStringCache.h"
#include "../BSHandleRefObject.h"
#include "NiAPI/NiNode.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESFullName
			{
			public:
				const char* GetName() const;

				READ_PROPERTY(GetName) const char* Name;
			};

#pragma pack(push, 1)
			class TESForm
			{
			public:
				enum FormType : uint8_t {
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
				enum FormFlags : DWORD {
					fsMaster				= /*00*/ 0x1,			// form is from an esm file
					fsModified				= /*01*/ 0x2,			// form is overriden by active mod or save file
					fsLinked				= /*03*/ 0x8,			// set after formids have been resolved into TESForm*
					fsDeleted				= /*05*/ 0x20,			// set on deletion, not saved in CK
					fsQuestItem				= /*0A*/ 0x400,			// aka Persistent Reference for TESObjectREFR (from CSE)
					fsInitialDisabled		= /*0B*/ 0x800,			// Disable TESObjectREFR
					fsIgnored				= /*0C*/ 0x1000,		// ?? (from CSE)
					fsTemporary				= /*0E*/ 0x4000,		// not saved in CK
				};
			protected:
				// 06
				struct BSTrackingData {
					WORD date;         // 00 low byte is day of month, high byte is number of months with 1 = Jan. 
									   // 2003 (Decembers are a little weird)
					WORD pad02;
					BYTE lastUser;     // 04 userID that last had this form checked out
					BYTE currentUser;  // 05 userID that has this form checked out
				};
			public:
				virtual ~TESForm() = default;

				using Array = BSTArray<TESForm*>;

				const char* GetEditorID() const;
				const char* GetFullName() const;
				void EditFormToWindow(HWND hParentWnd, __int64 a1, __int64 a2);
				void SetNewFormID(uint32_t NewIndex, bool Unk = true);
				bool CanPreview() const;
				void DebugInfo(char* Buffer, uint32_t BufferSize) const;
				void MarkAsDeleted(bool Value = true) const;
				void MarkAsChanged(bool Value = true) const;

				inline uint32_t GetFormID() const { return _FormID; }
				inline FormType GetFormType() const { return _FormType; }
				inline const char* GetEditorID_NoVTable() const { return _EditorID; }

				// Inaccurate, because there is a REFR class for this and it is of its own type (taken from SSE)
				inline bool IsReference() const { return (_FormType == FormType::ftReference) || (_FormType == FormType::ftCharacter); }

				inline bool IsFromMaster() const { return (_FormFlags & FormFlags::fsMaster); }
				inline bool IsModified() const { return (_FormFlags & FormFlags::fsModified); }
				inline bool IsLinked() const { return (_FormFlags & FormFlags::fsLinked); }
				inline bool IsDeleted() const { return (_FormFlags & FormFlags::fsDeleted); }
				inline bool IsInitialDisabled() const { return (_FormFlags & FormFlags::fsInitialDisabled); }
				inline bool IsIgnored() const { return (_FormFlags & FormFlags::fsIgnored); }
				inline bool IsTemporary() const { return (_FormFlags & FormFlags::fsTemporary); }
				inline bool IsQuestItem() const { return (_FormFlags & FormFlags::fsQuestItem); }
			public:
				static TESForm* GetFormByNumericID(uint32_t SearchID);
				static void* AlteredFormList_Create(Array* Array, uint32_t Unknown);
				static void AlteredFormList_RemoveAllEntries(Array* Array, bool Unknown);
				static void AlteredFormList_Insert(Array* Array, TESForm*& Entry);
				static void AlteredFormList_RemoveEntry(Array* Array, uint32_t Index, uint32_t Unknown);
				static bool AlteredFormList_ElementExists(Array* Array, TESForm*& Entry);

				inline static TESForm* (*OldGetFormByNumericID)(uint32_t);
				inline static void* (*OldAlteredFormList_Create)(Array*, uint32_t);
				inline static void (*OldAlteredFormList_RemoveAllEntries)(Array*, bool);
				inline static void (*OldAlteredFormList_Insert)(Array*, TESForm*&);
				inline static void (*OldAlteredFormList_RemoveEntry)(Array*, uint32_t, uint32_t);
				inline static const char* (*GetFormTypeStr)(FormType);

				static void FormReferenceMap_RemoveAllEntries();
				static Array* FormReferenceMap_FindOrCreate(uint64_t Key, bool Create);
				static void FormReferenceMap_RemoveEntry(uint64_t Key);
				static bool FormReferenceMap_Get(uint64_t Unused, uint64_t Key, Array** Value);

				inline static Array* (*OldFormReferenceMap_Find)(uint64_t Key);
				inline static Array* (*OldFormReferenceMap_Create)(Array*);
				inline static void (*OldFormReferenceMap_RemoveEntry)(Array*, int);
			public:
				READ_PROPERTY(IsModified) bool Active;
				READ_PROPERTY(GetFormID) uint32_t FormID;
				READ_PROPERTY(GetEditorID) const char* EditorID;
				READ_PROPERTY(GetFullName) const char* FullName;
				READ_PROPERTY(GetFormType) FormType Type;
			protected:
				char _pad0[0x8];
				uint32_t _FormFlags;
				uint32_t _FormID;
				char* _EditorID;
				BSTrackingData _trackingData;
				FormType _FormType;
				char _pad1[0x1];
			};
			static_assert(sizeof(TESForm) == 0x28);
#pragma pack(pop)

			class TESChildCell
			{
			public:
				virtual ~TESChildCell() = default;
				virtual void* GetSaveParentCell();
			};
			static_assert(sizeof(TESChildCell) == 0x8);

			class TESObjectREFR_Original : public TESForm, public TESChildCell, public BSHandleRefObject_Original
			{
			public:
				virtual ~TESObjectREFR_Original() = default;
				virtual void OtherTestFunction2();
				char _pad0[0x10];
			};
			static_assert(sizeof(TESObjectREFR_Original) == 0x50);

			class TESObjectREFR_Extremly : public TESForm, public TESChildCell, public BSHandleRefObject_Extremly
			{
			public:
				virtual ~TESObjectREFR_Extremly() = default;
				virtual void OtherTestFunction2();
				char _pad0[0x10];
			};
			static_assert(sizeof(TESObjectREFR_Extremly) == 0x50);

			typedef TESForm::Array		TESFormArray;
			typedef BSTArray<uint32_t>	TESFormIDArray;
		}
	}
}