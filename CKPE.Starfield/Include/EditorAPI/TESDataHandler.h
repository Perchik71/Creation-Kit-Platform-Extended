// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <EditorAPI/NiAPI/NiTObjectArray.h>
#include <EditorAPI/BSTList.h>
#include <EditorAPI/TESFile.h>
#include <EditorAPI/Forms/TESForm.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			template<typename T>
			class BSTEventSource
			{
				std::uint32_t _Size;
				std::uint32_t _AllocSize;
				T* _Data;
				std::uint32_t _Unk[4];
			public:
				virtual ~BSTEventSource();
			};

			struct TESDataHandlerEditorEvent
			{
				char unk[0x20];
			};

			namespace BSTEventDetail
			{
				class SourceBase : public BSTEventSource<TESDataHandlerEditorEvent>
				{
				public:
					virtual ~SourceBase();
				};
			}

			//#3: flags(40) `struct BSTSingletonSDM<class TESDataHandler, struct BSTSingletonSDMOpStaticBuffer>`:`. ? AU ? $BSTSingletonSDM@VTESDataHandler@@UBSTSingletonSDMOpStaticBuffer@@@@`
			//#2: flags(40) `struct BSTSingletonSDMBase<struct BSTSDMTraits<class TESDataHandler, struct BSTSingletonSDMOpStaticBuffer<class TESDataHandler> > >`:`. ? AU ? $BSTSingletonSDMBase@U ? $BSTSDMTraits@VTESDataHandler@@U ? $BSTSingletonSDMOpStaticBuffer@VTESDataHandler@@@@@@@@`
			//#0: flags(40) `struct BSTSDMTraits<class TESDataHandler, struct BSTSingletonSDMOpStaticBuffer<class TESDataHandler> >`:`. ? AU ? $BSTSDMTraits@VTESDataHandler@@U ? $BSTSingletonSDMOpStaticBuffer@VTESDataHandler@@@@@@`
			struct BSTSingletonSDMOpStaticBuffer
			{
				char unk[0x38];
			public:
				virtual ~BSTSingletonSDMOpStaticBuffer();
			};

			struct BSService
			{
				char unk[0x80];
			};

			namespace TESFormDetail
			{
				template<typename T>
				struct Fixed : public T
				{};
			}

			template<EditorAPI::Forms::TESForm::FormType _type = 
				EditorAPI::Forms::TESForm::FormType::ftNone>
			struct BSFormStorage
			{
				std::uint64_t Type{ _type };
				EditorAPI::Forms::TESForm::Array Array;
			};
			static_assert(sizeof(BSFormStorage<>) == 0x18);
			static_assert(offsetof(BSFormStorage<>, Array) == 0x8);

			template<EditorAPI::Forms::TESForm::FormType _type =
				EditorAPI::Forms::TESForm::FormType::ftNone>
			struct BSFormStorageWithPad
			{
				std::uint64_t Type{ _type };
				EditorAPI::Forms::TESForm::Array Array;
				std::uint64_t Pad{ 0 };
			};
			static_assert(sizeof(BSFormStorage<>) == 0x18);
			static_assert(offsetof(BSFormStorage<>, Array) == 0x8);

			class TESRegionList : public BSSimpleList<EditorAPI::Forms::TESForm*>
			{
			public:
				virtual ~TESRegionList() = default;
			private:
				std::uint64_t Unk18;
			};

			class TESDataHandler : public BSTEventDetail::SourceBase, public BSTSingletonSDMOpStaticBuffer, public BSService
			{
				TESFormDetail::Fixed<EditorAPI::Forms::TESForm> _ClipboardDependencyForm;
				char _unk[0x10];
			public:
				virtual ~TESDataHandler();

				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftNone> ArrayFormNone;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPluginInfo> ArrayFormPluginInfo;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFormGroup> ArrayFormGroup;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGameSetting> ArrayFormGameSetting;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftKeyword> ArrayFormKeyword;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFolderKeywordList> ArrayFormKeywordList;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLocationRefType> ArrayFormLocationRefType;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAction> ArrayFormAction;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTransform> ArrayFormTransform;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTextureSet> ArrayFormTextureSet;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGlobal> ArrayFormGlobal;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDamageType> ArrayFormDamageType;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftClass> ArrayFormClass;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFaction> ArrayFormFaction;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAffinityEvent> ArrayFormAffinityEvent;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftHeadPart> ArrayFormHeadPart;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEyes> ArrayFormEyes;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftRace> ArrayFormRace;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSoundMarker> ArrayFormSoundMarker;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSoundEcho> ArrayFormSoundEcho;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAcousticSpace> ArrayFormAcousticSpace;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAudioOcclusionPrimitive> ArrayFormAudioOcclusionPrimitive;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved022> ArrayFormReserved022;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEffectSetting> ArrayFormEffectSetting;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftScript> ArrayFormScript;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLandTexture> ArrayFormLandTexture;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftProjectedDecal> ArrayFormProjectedDecal;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEnchantment> ArrayFormEnchantment;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSpell> ArrayFormSpell;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftScroll> ArrayFormScroll;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftActivator> ArrayFormActivator;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTalkingActivator> ArrayFormTalkingActivator;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCurve> ArrayFormCurve;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCurve3D> ArrayFormCurve3D;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftArmor> ArrayFormArmor;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBook> ArrayFormBook;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftContainer> ArrayFormContainer;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDoor> ArrayFormDoor;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftIngredient> ArrayFormIngredient;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLight> ArrayFormLight;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMiscItem> ArrayFormMiscItem;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftStatic> ArrayFormStatic;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftStaticCollection> ArrayFormStaticCollection;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPackIn> ArrayFormPackIn;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMovableStatic> ArrayFormMovableStatic;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGrass> ArrayFormGrass;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFlora> ArrayFormFlora;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFurniture> ArrayFormFurniture;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWeapon> ArrayFormWeapon;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAmmo> ArrayFormAmmo;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftActor> ArrayFormActor;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLevelCharacter> ArrayFormLevelCharacter;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLevelPackIn> ArrayFormLevelPackIn;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftKey> ArrayFormKey;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPotion> ArrayFormPotion;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftIdleMarker> ArrayFormIdleMarker;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBiomeMarkerObject> ArrayFormBiomeMarkerObject;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftNote> ArrayFormNote;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftProjectile> ArrayFormProjectile;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved059> ArrayFormReserved059;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBendableSpline> ArrayFormBendableSpline;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSoulGem> ArrayFormSoulGem;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTerminal> ArrayFormTerminal;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLeveledItem> ArrayFormLeveledItem;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGenericBaseTemplate> ArrayFormGenericBaseTemplate;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGenericBase> ArrayFormGenericBase;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLevelGenericBase> ArrayFormLevelGenericBase;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWeather> ArrayFormWeather;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWeatherSettingsForm> ArrayFormWeatherSettingsForm;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftClimate> ArrayFormClimate;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftShaderParticleGeometryData> ArrayFormShaderParticleGeometryData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftRegion> ArrayFormRegion;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftNavMeshInfoMap> ArrayFormNavMeshInfoMap;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCell> ArrayFormCell;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReference> ArrayFormReference;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCharacter> ArrayFormCharacter;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMissile> ArrayFormMissile;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftArrow> ArrayFormArrow;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGrenade> ArrayFormGrenade;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBeam> ArrayFormBeam;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFlame> ArrayFormFlame;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCone> ArrayFormCone;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPlasma> ArrayFormPlasma;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBarrier> ArrayFormBarrier;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEmitter> ArrayFormEmitter;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved085> ArrayFormReserved085;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWorldSpace> ArrayFormWorldSpace;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftNavMesh> ArrayFormNavMesh;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved088> ArrayFormReserved088;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTopic> ArrayFormTopic;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTopicInfo> ArrayFormTopicInfo;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftQuest> ArrayFormQuest;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftIdle> ArrayFormIdle;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPackage> ArrayFormPackage;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCombatStyle> ArrayFormCombatStyle;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLoadScreen> ArrayFormLoadScreen;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLeveledSpell> ArrayFormLeveledSpell;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved097> ArrayFormReserved097;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWater> ArrayFormWater;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEffectShader> ArrayFormEffectShader;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved100> ArrayFormReserved100;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftExplosion> ArrayFormExplosion;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDebris> ArrayFormDebris;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftImageSpace> ArrayFormImageSpace;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftImageSpaceModifier> ArrayFormImageSpaceModifier;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftListForm> ArrayFormListForm;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPerk> ArrayFormPerk;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBodyPartData> ArrayFormBodyPartData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAddonNode> ArrayFormAddonNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftActorValueInfo> ArrayFormActorValueInfo;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCameraShot> ArrayFormCameraShot;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCameraPath> ArrayFormCameraPath;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftVoice> ArrayFormVoice;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMaterial> ArrayFormMaterial;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftImpactData> ArrayFormImpactData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftImpactDataSet> ArrayFormImpactDataSet;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved116> ArrayFormReserved116;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLocation> ArrayFormLocation;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMessage> ArrayFormMessage;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftRagdoll> ArrayFormRagdoll;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDefaultObjectManager> ArrayFormDefaultObjectManager;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDefaultObject> ArrayFormDefaultObject;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLightingTemplate> ArrayFormLightingTemplate;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMusic> ArrayFormMusic;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFootstep> ArrayFormFootstep;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFootstepSet> ArrayFormFootstepSet;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftStoryManagerBranchNode> ArrayFormStoryManagerBranchNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftStoryManagerQuestNode> ArrayFormStoryManagerQuestNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftStoryManagerEventNode> ArrayFormStoryManagerEventNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDialogue> ArrayFormDialogue;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMusicTrackFormWrapper> ArrayFormMusicTrackFormWrapper;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved131> ArrayFormReserved131;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWordOfPower> ArrayFormWordOfPower;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftShout> ArrayFormShout;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEquipSlot> ArrayFormEquipSlot;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftRelationship> ArrayFormRelationship;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftScene> ArrayFormScene;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAssociationType> ArrayFormAssociationType;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftOutfit> ArrayFormOutfit;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftArt> ArrayFormArt;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMaterialObject> ArrayFormMaterialObject;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMovementType> ArrayFormMovementType;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftDualCastData> ArrayFormDualCastData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCollisionLayer> ArrayFormCollisionLayer;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftColorForm> ArrayFormColorForm;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReverbParameters> ArrayFormReverbParameters;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReferenceGroup> ArrayFormReferenceGroup;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAimModel> ArrayFormAimModel;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAimAssistModel> ArrayFormAimAssistModel;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMeleeAimAssistModel> ArrayFormMeleeAimAssistModel;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLayer> ArrayFormLayer;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftConstructibleObject> ArrayFormConstructibleObject;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAttachmentMod> ArrayFormAttachmentMod;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAimDownSightModel> ArrayFormAimDownSightModel;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftInstanceNamingRules> ArrayFormInstanceNamingRules;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSoundKeywordMapping> ArrayFormSoundKeywordMapping;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved156> ArrayFormReserved156;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAttractionRule> ArrayFormAttractionRule;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSoundTagSet> ArrayFormSoundTagSet;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftResource> ArrayFormResource;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBiome> ArrayFormBiome;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftNavMeshObstacleCoverManager> ArrayFormNavMeshObstacleCoverManager;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLensFlare> ArrayFormLensFlare;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved163> ArrayFormReserved163;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftObjectVisibilityManager> ArrayFormObjectVisibilityManager;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved165> ArrayFormReserved165;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSnapTemplateNode> ArrayFormSnapTemplateNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSnapTemplate> ArrayFormSnapTemplate;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGroundCover> ArrayFormGroundCover;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMorphableObject> ArrayFormMorphableObject;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTraversal> ArrayFormTraversal;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftResourceGenerationData> ArrayFormResourceGenerationData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftObjectSwap> ArrayFormObjectSwap;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAtmosphere> ArrayFormAtmosphere;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLevelSpaceCell> ArrayFormLevelSpaceCell;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSpeechChallengeObject> ArrayFormSpeechChallengeObject;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved176> ArrayFormReserved176;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAimAssistPoseData> ArrayFormAimAssistPoseData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftVolumetricLighting> ArrayFormVolumetricLighting;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSurfaceBlock> ArrayFormSurfaceBlock;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved180> ArrayFormReserved180;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSurfacePattern> ArrayFormSurfacePattern;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSurfaceTree> ArrayFormSurfaceTree;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPlanetContentManagerTree> ArrayFormPlanetContentManagerTree;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftBoneModifier> ArrayFormBoneModifier;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSnapBehavior> ArrayFormSnapBehavior;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPlanetData> ArrayFormPlanetData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved187> ArrayFormReserved187;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftConditionForm> ArrayFormConditionForm;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPlanetContentManagerBranchNode> ArrayFormPlanetContentManagerBranchNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPlanetContentManagerContentNode> ArrayFormPlanetContentManagerContentNode;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGalaxyStar> ArrayFormGalaxyStar;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWwiseEvent> ArrayFormWwiseEvent;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftResearch> ArrayFormResearch;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAimOpticalSightModel> ArrayFormAimOpticalSightModel;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAmbienceSet> ArrayFormAmbienceSet;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWeaponBarrelModel> ArrayFormWeaponBarrelModel;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPatternStyle> ArrayFormPatternStyle;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLayeredMaterialSwap> ArrayFormLayeredMaterialSwap;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftForceData> ArrayFormForceData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTerminalMenu> ArrayFormTerminalMenu;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftEffectSequence> ArrayFormEffectSequence;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftSecondaryDamageList> ArrayFormSecondaryDamageList;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftMaterialPath> ArrayFormMaterialPath;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftCloud> ArrayFormCloud;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftFogVolume> ArrayFormFogVolume;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftWwiseKeywordMapping> ArrayFormWwiseKeywordMapping;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftLegendaryItem> ArrayFormLegendaryItem;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftParticleSystemDefineCollection> ArrayFormParticleSystemDefineCollection;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftGalaxySunPreset> ArrayFormGalaxySunPreset;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPhotoModeFeature> ArrayFormPhotoModeFeature;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftTimeOfDayData> ArrayFormTimeOfDayData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftAVMData> ArrayFormAVMData;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftPersistentIDs> ArrayFormPersistentIDs;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftChallenge> ArrayFormChallenge;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved215> ArrayFormReserved215;
				BSFormStorageWithPad<EditorAPI::Forms::TESForm::FormType::ftReserved216> ArrayFormReserved216;
				BSFormStorage<EditorAPI::Forms::TESForm::FormType::ftReserved217> ArrayFormReserved217;

				TESRegionList* RegionList;
				EditorAPI::Forms::TESForm::Array Cells;
				char Pad1D60[0x8];
				EditorAPI::NiAPI::NiTObjectArray<void*> AddonNodes;
				char Pad1D80[0x18];
				TESFile* ActivePlugin;
				BSSimpleList<TESFile*> ModInfoList;
				BSTArray<TESFile*> LoadedMods;
				
				inline static ISingleton<bool> UserModdedSingleton;
				inline static ISingleton<TESDataHandler> Singleton;
			};
			static_assert(sizeof(TESDataHandler) == 0x1DC0);
		}
	}
}