// Copyright © 2025-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <QtCore/qarraydata.h>
#include <CKPE.Common.h>
#include <EditorAPI/BSTArray.h>
#include <EditorAPI/BSReflection.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/Forms/BaseFormComponent.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// `class TESForm`: VTable [0x00000000086DD790, 0x0000000000000000 offset, 173 functions] `.?AVTESForm@@`
				class TESForm : public BaseFormComponent, public BGSFormDialogInterface, public BSReflection::IObject
				{
				public:
					struct BSTrackingData
					{
						// 0xXXXXXXXYYYYZZZZZ - X: years (with 2000), Y: months, Z: days
						union
						{
							struct { std::uint16_t date; };
							struct
							{
								std::uint16_t days : 5;
								std::uint16_t months : 4;
								std::uint16_t years : 7;
							};
						};

						std::uint16_t lastUser;     // userID that last had this form checked out
						std::uint16_t currentUser;  // userID that has this form checked out
					};

					// Form Type
					enum FormType : std::uint8_t {
						ftNone = 0,					// NONE
						ftPluginInfo,				// TES4
						ftFormGroup,				// GRUP
						ftGameSetting,				// GMST
						ftKeyword,
						ftFolderKeywordList,
						ftLocationRefType,
						ftAction,
						ftTransform,
						ftTextureSet,
						ftGlobal,
						ftDamageType,
						ftClass,
						ftFaction,
						ftAffinityEvent,
						ftHeadPart,
						ftEyes,
						ftRace,
						ftSoundMarker,
						ftSoundEcho,
						ftAcousticSpace,
						ftAudioOcclusionPrimitive,
						ftReserved022,
						ftEffectSetting,
						ftScript,
						ftLandTexture,
						ftProjectedDecal,
						ftEnchantment,
						ftSpell,
						ftScroll,
						ftActivator,
						ftTalkingActivator,
						ftCurve,
						ftCurve3D,
						ftArmor,
						ftBook,
						ftContainer,
						ftDoor,
						ftIngredient,
						ftLight,
						ftMiscItem,
						ftStatic,
						ftStaticCollection,
						ftPackIn,
						ftMovableStatic,
						ftGrass,
						ftFlora,
						ftFurniture,
						ftWeapon,
						ftAmmo,
						ftActor,
						ftLevelCharacter,
						ftLevelPackIn,
						ftKey,
						ftPotion,
						ftIdleMarker,
						ftBiomeMarkerObject,
						ftNote,
						ftProjectile,
						ftReserved059,
						ftBendableSpline,
						ftSoulGem,
						ftTerminal,
						ftLeveledItem,
						ftGenericBaseTemplate,
						ftGenericBase,
						ftLevelGenericBase,
						ftWeather,
						ftWeatherSettingsForm,
						ftClimate,
						ftShaderParticleGeometryData,
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
						ftPlasma,
						ftBarrier,
						ftEmitter,
						ftReserved085,				// Hazard ?		
						ftWorldSpace,
						ftNavMesh,
						ftReserved088,				// TLOD
						ftTopic,
						ftTopicInfo,
						ftQuest,
						ftIdle,
						ftPackage,
						ftCombatStyle,
						ftLoadScreen,
						ftLeveledSpell,
						ftReserved097,				// ANIO
						ftWater,
						ftEffectShader,
						ftReserved100,				// TOFT
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
						ftReserved116,				// ARMA
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
						ftReserved131,				// DLVW
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
						ftDualCastData,
						ftCollisionLayer,
						ftColorForm,
						ftReverbParameters,
						ftReferenceGroup,
						ftAimModel,
						ftAimAssistModel,
						ftMeleeAimAssistModel,
						ftLayer,
						ftConstructibleObject,
						ftAttachmentMod,
						ftAimDownSightModel,
						ftInstanceNamingRules,
						ftSoundKeywordMapping,
						ftReserved156,				// SCCO
						ftAttractionRule,
						ftSoundTagSet,				// STAG
						ftResource,
						ftBiome,
						ftNavMeshObstacleCoverManager,
						ftLensFlare,
						ftReserved163,				// LSPR
						ftObjectVisibilityManager,
						ftReserved165,				// DLYR
						ftSnapTemplateNode,
						ftSnapTemplate,
						ftGroundCover,
						ftMorphableObject,
						ftTraversal,
						ftResourceGenerationData,
						ftObjectSwap,
						ftAtmosphere,
						ftLevelSpaceCell,
						ftSpeechChallengeObject,
						ftReserved176,				// RESO
						ftAimAssistPoseData,
						ftVolumetricLighting,
						ftSurfaceBlock,
						ftReserved180,				// SFPC
						ftSurfacePattern,
						ftSurfaceTree,
						ftPlanetContentManagerTree,
						ftBoneModifier,
						ftSnapBehavior,
						ftPlanetData,
						ftReserved187,				// IUTF
						ftConditionForm,
						ftPlanetContentManagerBranchNode,
						ftPlanetContentManagerContentNode,
						ftGalaxyStar,
						ftWwiseEvent,
						ftResearch,
						ftAimOpticalSightModel,
						ftAmbienceSet,
						ftWeaponBarrelModel,
						ftPatternStyle,
						ftLayeredMaterialSwap,
						ftForceData,
						ftTerminalMenu,
						ftEffectSequence,
						ftSecondaryDamageList,
						ftMaterialPath,
						ftCloud,
						ftFogVolume,
						ftWwiseKeywordMapping,
						ftLegendaryItem,
						ftParticleSystemDefineCollection,
						ftGalaxySunPreset,
						ftPhotoModeFeature,
						ftTimeOfDayData,
						ftAVMData,
						ftPersistentIDs,
						ftChallenge,
						ftReserved215,				// FXPD
						ftReserved216,				// GPOF
						ftReserved217,				// GPOG
						ftMax
					};
					// Form State
					enum FormFlags : std::uint64_t 
					{
						fsMaster = /*00*/ 0x1,			// form is from an esm file
						fsModified = /*01*/ 0x2,		// form is overriden by active mod or save file
						fsLinked = /*03*/ 0x8,			// set after formids have been resolved into TESForm*
						fsDeleted = /*05*/ 0x20,		// set on deletion, not saved in CK
					};
				private:	
					inline static const char* szFormTypeStr[] =
					{
						"NONE", "TES4",	"GRUP",	"GMST",	"KYWD",	"FFKW",	"LCRT",	"AACT",	"TRNS",	"TXST",	"GLOB",	"DMGT",	"CLAS",	"FACT",	"AFFE",	"HDPT",
						"EYES",	"RACE",	"SOUN",	"SECH",	"ASPC",	"AOPF",	"SKIL",	"MGEF",	"SCPT",	"LTEX",	"PDCL",	"ENCH",	"SPEL",	"SCRL",	"ACTI",	"TACT",
						"CURV",	"CUR3",	"ARMO",	"BOOK",	"CONT",	"DOOR",	"INGR",	"LIGH",	"MISC",	"STAT",	"SCOL",	"PKIN",	"MSTT",	"GRAS",	"FLOR",	"FURN",
						"WEAP",	"AMMO",	"NPC_",	"LVLN",	"LVLP",	"KEYM",	"ALCH",	"IDLM",	"BMMO",	"NOTE",	"PROJ",	"HAZD",	"BNDS",	"SLGM",	"TERM",	"LVLI",
						"GBFT",	"GBFM",	"LVLB",	"WTHR",	"WTHS",	"CLMT",	"SPGD",	"REGN",	"NAVI",	"CELL",	"REFR",	"ACHR",	"PMIS",	"PARW",	"PGRE",	"PBEA",
						"PFLA",	"PCON",	"PPLA",	"PBAR",	"PEMI",	"PHZD",	"WRLD",	"NAVM",	"TLOD",	"DIAL",	"INFO",	"QUST",	"IDLE",	"PACK",	"CSTY",	"LSCR",
						"LVSP",	"ANIO",	"WATR",	"EFSH",	"TOFT",	"EXPL",	"DEBR",	"IMGS",	"IMAD",	"FLST",	"PERK",	"BPTD",	"ADDN",	"AVIF",	"CAMS",	"CPTH",
						"VTYP",	"MATT",	"IPCT",	"IPDS",	"ARMA",	"LCTN",	"MESG",	"RGDL",	"DOBJ",	"DFOB",	"LGTM",	"MUSC",	"FSTP",	"FSTS",	"SMBN",	"SMQN",
						"SMEN",	"DLBR",	"MUST",	"DLVW",	"WOOP",	"SHOU",	"EQUP",	"RELA",	"SCEN",	"ASTP",	"OTFT",	"ARTO",	"MATO",	"MOVT",	"DUAL",	"COLL",
						"CLFM",	"REVB",	"RFGP",	"AMDL",	"AAMD",	"MAAM",	"LAYR",	"COBJ",	"OMOD",	"ZOOM",	"INNR",	"KSSM",	"SCCO",	"AORU",	"STAG",	"IRES",
						"BIOM",	"NOCM",	"LENS",	"LSPR",	"OVIS",	"DLYR",	"STND",	"STMP",	"GCVR",	"MRPH",	"TRAV",	"RSGD",	"OSWP",	"ATMO",	"LVSC",	"SPCH",
						"RESO",	"AAPD",	"VOLI",	"SFBK",	"SFPC",	"SFPT",	"SFTR",	"PCMT",	"BMOD",	"STBH",	"PNDT",	"IUTF",	"CNDF",	"PCBN",	"PCCN",	"STDT",
						"WWED",	"RSPJ",	"AOPS",	"AMBS",	"WBAR",	"PTST",	"LMSW",	"FORC",	"TMLM",	"EFSQ",	"SDLT",	"MTPT",	"CLDF",	"FOGV",	"WKMF",	"LGDI",
						"PSDC",	"SUNP",	"PMFT",	"TODD",	"AVMD",	"PERS",	"CHAL",	"FXPD",	"GPOF",	"GPOG",
					};

					char pad50[0x8];
					BSFixedString _EditorID;
					BSFixedString _FullName;
					std::uint64_t _FormFlags;
					std::uint32_t _FormID;
					BSTrackingData _TrackingData;
					std::uint8_t _Unk7A;
					std::uint8_t _Unk7B;
					char pad7C[0xC];
					FormType _FormType;
					std::uint8_t _ModIndex;
				public:
					using Array = BSTArray<TESForm*>;

					virtual ~TESForm() = default;

					inline static TESForm* (*FindFormByFormID)(std::uint32_t);
					inline static TESForm* (*FindFormByEditorID)(const char*);

					[[nodiscard]] inline const char* GetFormTypeShortStr() const noexcept(true)
					{
						return szFormTypeStr[std::min(GetFormType(), ftReserved217)];
					}

					[[nodiscard]] inline bool IsFromMaster() const noexcept(true) { return (_FormFlags & FormFlags::fsMaster); }
					[[nodiscard]] inline bool IsModified() const noexcept(true) { return (_FormFlags & FormFlags::fsModified); }
					[[nodiscard]] inline bool IsLinked() const noexcept(true) { return (_FormFlags & FormFlags::fsLinked); }
					[[nodiscard]] inline bool IsDeleted() const noexcept(true) { return (_FormFlags & FormFlags::fsDeleted); }
					[[nodiscard]] inline FormType GetFormType() const noexcept(true) { return _FormType; }
					[[nodiscard]] inline std::uint64_t GetFormFlags() const noexcept(true) { return _FormFlags; }
					[[nodiscard]] inline std::uint32_t GetFormID() const noexcept(true) { return _FormID; }
					[[nodiscard]] inline const char* GetEditorID() const noexcept(true) { return _EditorID; }
					[[nodiscard]] inline const char* GetFullName() const noexcept(true) { return _FullName; }

					[[nodiscard]] FormType GetFormType_orig() const noexcept(true);
					[[nodiscard]] std::uint32_t GetEditIDLength() const noexcept(true);
					[[nodiscard]] const char* GetEditorID_orig() const noexcept(true);
					[[nodiscard]] const char* GetFullName_orig() const noexcept(true);

					void DebugInfo(char* buffer, std::uint32_t dwSize) const noexcept(true);
					void MarkAsDeleted(bool state = true) const noexcept(true);
					void MarkAsChanged(bool state = true) const noexcept(true);
					void ShowEditWindow(void* hParentWindow) const noexcept(true);
				public:
					CKPE_READ_PROPERTY(IsModified) bool Active;
					CKPE_READ_PROPERTY(GetFormID) std::uint32_t FormID;
					CKPE_READ_PROPERTY(GetFormFlags) std::uint64_t FormFlag;
					CKPE_READ_PROPERTY(GetEditorID) const char* EditorID;
					CKPE_READ_PROPERTY(GetFullName) const char* FullName;
					CKPE_READ_PROPERTY(GetFormType) FormType Type;
					CKPE_READ_PROPERTY(GetFormTypeShortStr) const char* TypeStr;
				};
				static_assert(sizeof(TESForm) == 0x90);

				typedef TESForm::Array TESFormArray;
			}
		}
	}
}