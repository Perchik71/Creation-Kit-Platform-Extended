// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "NiAPI/NiNode.h"
#include "TESFileSF.h"
#include "Editor API/FO4/BSFixedString.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class TESForm
			{
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

				char pad08[0x50];
				Fallout4::BSFixedString _EditorID;

			public:
				// Form Type
				enum FormType : uint8_t {
					ftReserved000 = 0,			// NONE
					ftReserved001,				// TES4
					ftReserved002,				// GRUP
					ftReserved003,				// GMST
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
					ftReserved020,				// SKIL
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
					ftHazard03b,
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
					ftHazard055,				// Hazard ?		
					ftWorldSpace,
					ftNavMesh,
					ftReserved058,				// TLOD
					ftTopic,
					ftTopicInfo,
					ftQuest,
					ftIdle,
					ftPackage,
					ftCombatStyle,
					ftLoadScreen,
					ftLeveledSpell,
					ftReserved061,				// ANIO
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
					ftReserved074,				// ARMA
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
					ftReserved083,				// DLVW
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
					ftReserved09C,				// SCCO
					ftAttractionRule,			
					ftSoundTagSet,				// STAG
					ftResource,
					ftBiome,
					ftNavMeshObstacleCoverManager,		
					ftLensFlare,			
					ftReserved0a3,				// LSPR
					ftObjectVisibilityManager,
					ftReserved0a5,				// DLYR
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
					ftReserved0b0,				// RESO
					ftAimAssistPoseData,
					ftVolumetricLighting,
					ftSurfaceBlock,
					ftReserved0b4,				// SFPC
					ftSurfacePattern,		
					ftSurfaceTree,		
					ftPlanetContentManagerTree,
					ftBoneModifier,	
					ftSnapBehavior,	
					ftPlanetData,
					ftReserved0bb,				// IUTF
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
					ftReserved0d7,				// FXPD
					ftReserved0d8,				// GPOF
					ftReserved0d9,				// GPOG
					ftMax
				};

				virtual ~TESForm() = default;

				inline static TESForm* (*GetFormByNumericID)(const uint32_t SearchID);
				inline static TESForm* (*GetFormByEditorID)(const char* EditorID);

				inline void LoadForm(TESFile* file)	const							// vtbl->160
				{ thisVirtualCall<void>(0x160, this, file); }
				inline void SaveForm(TESFile* file)	const							// vtbl->168
				{ thisVirtualCall<void>(0x168, this, file); }
				inline FormType GetFormType() const									// vtbl->1A8	(offset -> 0x88)
				{ return thisVirtualCall<FormType>(0x1A8, this); }
				inline void DebugInfo(char* buffer, uint32_t dwSize) const			// vtbl->1B0
				{ thisVirtualCall<void>(0x1B0, this, buffer, dwSize); }
				inline void MarkAsDeleted(bool state = true) const					// vtbl->230 
				{ thisVirtualCall<void>(0x230, this, state); }
				inline void MarkAsChanged(bool state = true) const					// vtbl->238 
				{ thisVirtualCall<void>(0x238, this, state); }
				inline uint32_t GetEditIDLength() const								// vtbl->308
				{ thisVirtualCall<void>(0x308, this); }
				inline const char* GetEditorID() const								// vtbl->310	(offset -> 0x58 -> 0x18)	
				{ thisVirtualCall<void>(0x310, this); }
				inline const char* GetFullName() const								// vtbl->2E0
				{ thisVirtualCall<void>(0x2E0, this); }
				inline const char* GetFormTypeShortStr() const
				{ return szFormTypeStr[std::min(GetFormType(), ftReserved0d9)]; }
			};
		}
	}
}

#pragma pack(pop)