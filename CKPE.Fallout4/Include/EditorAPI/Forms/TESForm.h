// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>
#include <EditorAPI/TESFile.h>
#include <EditorAPI/BSTArray.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/NiAPI/NiFlags.h>
#include <EditorAPI/BGSMod.h>
#include <EditorAPI/BGSSoundTagComponent.h>
#include <EditorAPI/BGSPreviewTransform.h>

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

				class TESForm : public BaseFormComponent
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
					};

					struct ENUM_FORM_ID
					{
						std::uint32_t ID;
						char* StrType;
						char StrType2[4];
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

					/* 0A8 */ virtual void InitializeData();
					/* 0B0 */ virtual void ClearData();
					/* 0B8 */ virtual void Load(TESFile* File);
					/* 0C0 */ virtual void LoadPartial(TESFile* File);
					/* 0C8 */ virtual void LoadEdit(TESFile* File);
					/* 0D0 */ virtual void Save(TESFile* File);
					/* 0D8 */ virtual void SavePlugin(TESFile* File);
					/* 0E0 */ virtual void LoadPlugin(TESFile* File);
					/* 0E8 */ virtual void sub_0E8();
					/* 0F0 */ virtual void sub_0F0();
					/* 0F8 */ virtual void sub_0F8();
					/* 100 */ virtual TESForm* CreateDuplicateForm(bool bCreateEditorID, void* Unk);
					/* 108 */ virtual void sub_108();
					/* 110 */ virtual void InitItemImpl() noexcept(true);
					/* 118 */ [[nodiscard]] virtual TESFile* GetDescriptionOwnerFile() const noexcept(true);
					/* 120 */ [[nodiscard]] virtual FormType GetFormType() const noexcept(true);
					/* 128 */ virtual void DebugInfo(char* lpBuffer, std::uint32_t nBufferSize) const noexcept(true);
					/* 130 */ [[nodiscard]] virtual bool HasKnown() const noexcept(true);
					/* 138 */ [[nodiscard]] virtual bool HasRandomAnim() const noexcept(true);
					/* 140 */ [[nodiscard]] virtual bool HasPlayable() const noexcept(true);
					/* 148 */ [[nodiscard]] virtual bool IsHeadingMarker() const noexcept(true);
					/* 150 */ [[nodiscard]] virtual bool IsHeadtrackMarker() const noexcept(true);
					/* 158 */ [[nodiscard]] virtual bool HasDangerous() const noexcept(true);
					/* 160 */ [[nodiscard]] virtual bool HasCurrents() const noexcept(true);
					/* 168 */ [[nodiscard]] virtual bool HasObstacle() const noexcept(true);
					/* 170 */ [[nodiscard]] virtual bool IsLODLandObject() const noexcept(true);
					/* 178 */ [[nodiscard]] virtual bool HasOnLocalMap() const noexcept(true);
					/* 180 */ [[nodiscard]] virtual bool HasMustUpdate() const noexcept(true);
					/* 188 */ [[nodiscard]] virtual bool SetOnLocalMap(bool bValue) noexcept(true);
					/* 190 */ [[nodiscard]] virtual bool HasIgnoredBySandbox() const noexcept(true);
					/* 198 */ virtual void MarkAsDeleted(bool bValue = true) noexcept(true);
					/* 1A0 */ virtual void MarkAsChanged(bool bValue = true) noexcept(true);
					/* 1A8 */ [[nodiscard]] virtual NavMeshGenerationImportOption GetNavMeshGenerationImportOption() const noexcept(true);
					/* 1B0 */ virtual void SaveObjectBound();
					/* 1B8 */ virtual void LoadObjectBound(TESFile* pFile);
					/* 1C0 */ virtual void SavePreviewTransform();
					/* 1C8 */ virtual void LoadPreviewTransform(TESFile* pFile);
					/* 1D0 */ [[nodiscard]] virtual bool IsBoundObject() const noexcept(true);
					/* 1D8 */ [[nodiscard]] virtual bool IsObject() const noexcept(true);
					/* 1E0 */ [[nodiscard]] virtual bool IsMagicItem() const noexcept(true);
					/* 1E8 */ [[nodiscard]] virtual bool IsWater() const noexcept(true);
					/* 1F0 */ [[nodiscard]] virtual TESForm* AsReference() noexcept(true);
					/* 1F8 */ [[nodiscard]] virtual const TESForm* AsReference() const noexcept(true);
					/* 200 */ virtual std::uint32_t GetRefCount() const noexcept(true);
					/* 208 */ virtual const char* GetTextForParsedSubTag(const BSFixedString& sSubTag) const noexcept(true);
					/* 210 */ virtual void CopyFrom(TESForm* Source) noexcept(true);
					/* 218 */ [[nodiscard]] virtual std::int32_t Compare(TESForm* Rhs) const noexcept(true);
					/* 220 */ virtual bool BelongsInGroup(TESForm* Group, bool bAllowParentGroups, bool bCurrentOnly);
					/* 228 */ virtual void CreateGroupData(TESForm* OutGroupForm, TESForm* ParentGroup);
					/* 230 */ [[nodiscard]] virtual std::uint32_t GetFormEditorIDLength() const;
					/* 238 */ [[nodiscard]] virtual const char* GetFormEditorID() const;
					/* 240 */ virtual void sub_240() { return; }
					/* 248 */ virtual void sub_248() { return; }
					/* 250 */ virtual void sub_250() { return; }
					/* 258 */ virtual void sub_258() { return; }
					/* 260 */ virtual void sub_260() { return; }
					/* 268 */ virtual void sub_268() { return; }
					/* 270 */ virtual void sub_270() { return; }
					/* 278 */ virtual void sub_278() { return; }
					/* 280 */ virtual void sub_280() { return; }
					/* 288 */ virtual void sub_288() { return; }
					/* 290 */ virtual void sub_290() { return; }
					/* 298 */ virtual void sub_298() { return; }
					/* 2A0 */ virtual void sub_2A0() { return; }
					/* 2A8 */ virtual void sub_2A8() { return; }
					/* 2B0 */ virtual void sub_2B0() { return; }
					/* 2B8 */ virtual void sub_2B8() { return; }
					/* 2C0 */ virtual void sub_2C0() { return; }
					/* 2C8 */ virtual bool UpdateTrackingData() noexcept(true);
					/* 2D0 */ virtual void sub_2D0() { return; }
					/* 2D8 */ virtual void sub_2D8() { return; }
					/* 2E0 */ virtual void sub_2E0() { return; }
					/* 2E8 */ virtual void sub_2E8() { return; }
					/* 2F0 */ virtual void sub_2F0() { return; }
					/* 2F8 */ [[nodiscard]] virtual bool HasRequiredEditorID() const noexcept(true);
					/* 300 */ [[nodiscard]] virtual bool DoVerificationEditorID(HWND hDialogBox) const noexcept(true);
					/* 308 */ virtual void sub_308() { return; }
					/* 310 */ virtual void sub_310() { return; }
					/* 318 */ virtual void sub_318() { return; }
					/* 320 */ virtual void sub_320() { return; }
					/* 328 */ virtual void sub_328() { return; }
					/* 330 */ virtual void sub_330() { return; }
					/* 338 */ virtual void sub_338() { return; }
					/* 340 */ virtual void EditFormToWindow(HWND hParentWnd, bool bModalWindow,
						[[maybe_unused]] bool bUnk1, [[maybe_unused]] std::int32_t nUnk2) noexcept(true);
					/* 348 */ virtual void sub_348() { return; }
					/* 350 */ virtual void sub_350() { return; }
					/* 358 */ virtual void sub_358() { return; }
					/* 360 */ [[nodiscard]] virtual bool HasRequiredUniqueEditorID() const noexcept(true);
					/* 368 */ virtual void sub_368() { return; }
					/* 370 */ virtual void sub_370() { return; }
					/* 378 */ virtual bool WriteWidthTabToSettingFile([[maybe_unused]] std::int64_t nUnk, std::int32_t nWidth) noexcept(true);
					/* 380 */ virtual std::int32_t ReadWidthTabFromSettingFile() const noexcept(true);
					/* 388 */ virtual std::int32_t GetSafeFormEditorID(char* lpBuffer, std::uint32_t nBufferSize) noexcept(true);
					/* 390 */ virtual std::int32_t GetSafeFormEditorID(char* lpBuffer, std::uint32_t nBufferSize) const noexcept(true);
					/* 398 */ virtual void SetNewFormID(std::uint32_t nNewFormID, bool bUpdateFile = true) noexcept(true);
					/* 3A0 */ [[nodiscard]] virtual const char* GetObjectTypeName() const noexcept(true);
					/* 3A8 */ [[nodiscard]] virtual bool HasAvailableInGame() const noexcept(true);
					/* 3B0 */ [[nodiscard]] virtual BGSMod::Template::Items* GetObjectTemplate() const noexcept(true);
					/* 3B8 */ [[nodiscard]] virtual BGSPreviewTransform* GetPreviewTransform() const noexcept(true);
					/* 3C0 */ [[nodiscard]] virtual BGSSoundTagComponent* GetSoundTagComponent() const noexcept(true);
					/* 3C8 */ [[nodiscard]] virtual std::uint32_t GetFilledSlots() const noexcept(true);
					/* 3D0 */ [[nodiscard]] virtual std::uint32_t GetFilledSlotsImpl() const noexcept(true);
					/* 3D8 */ [[nodiscard]] virtual float GetDesirability() const noexcept(true);

					[[nodiscard]] inline std::uint32_t GetFormFlags() const noexcept(true) { return _FormFlags.Flags; }
					[[nodiscard]] inline std::uint32_t GetFormID() const noexcept(true) { return _FormID; }

					// Not all forms have a localized name, but I'll add it here to simplify my life,
					// since not all types of forms are known to me.
					[[nodiscard]] const char* TryGetFullName() const noexcept(true);

					[[nodiscard]] inline bool HasFromMaster() const noexcept(true) { return _FormFlags.Has(fsMaster); }
					[[nodiscard]] inline bool HasModified() const noexcept(true) { return _FormFlags.Has(fsModified); }
					[[nodiscard]] inline bool HasLinked() const noexcept(true) { return _FormFlags.Has(fsLinked); }
					[[nodiscard]] inline bool HasDeleted() const noexcept(true) { return _FormFlags.Has(fsDeleted); }
					[[nodiscard]] inline bool HasInitialDisabled() const noexcept(true) { return _FormFlags.Has(fsInitialDisabled); }
					[[nodiscard]] inline bool HasIgnored() const noexcept(true) { return _FormFlags.Has(fsIgnored); }
					[[nodiscard]] inline bool HasTemporary() const noexcept(true) { return _FormFlags.Has(fsTemporary); }
					[[nodiscard]] inline bool HasQuestItem() const noexcept(true) { return _FormFlags.Has(fsQuestItem); }
				
					[[nodiscard]] bool IsPlayer() const noexcept(true) { return _FormID == 0x00000007; }
					[[nodiscard]] bool IsPlayerRef() const noexcept(true) { return _FormID == 0x00000014; }

					// Inaccurate, because there is a REFR class for this and it is of its own type (taken from SSE)
					[[nodiscard]] inline bool IsReference() const noexcept(true)
					{ 
						return
							(_FormType == FormType::ftReference) || 
							(_FormType == FormType::ftCharacter); 
					}

					[[nodiscard]] ENUM_FORM_ID GetSaveFormType() const noexcept(true);
					bool SetFormEditorID(const char* NewEditorID) noexcept(true);

					inline static TESForm* (*FindFormByFormID)(std::uint32_t) = nullptr;
					inline static bool (*SetFormEditorIDImpl)(TESForm* _This, const char* NewEditorID) = nullptr;
					inline static ENUM_FORM_ID* EnumFormIDs = nullptr;

					CKPE_PROPERTY(HasModified, MarkAsChanged) bool Actived;
					CKPE_PROPERTY(HasDeleted, MarkAsDeleted) bool Deleted;
					CKPE_PROPERTY(GetFormID, SetNewFormID) std::uint32_t FormID;
					CKPE_PROPERTY(GetFormEditorID, SetFormEditorID) const char* EditorID;
					CKPE_READ_PROPERTY(TryGetFullName) const char* FullName;
					CKPE_READ_PROPERTY(GetFormType) FormType Type;

					CKPE_READ_PROPERTY(GetObjectTemplate) BGSMod::Template::Items* ObjectTemplate;
					CKPE_READ_PROPERTY(GetPreviewTransform) BGSPreviewTransform* PreviewTransform;
					CKPE_READ_PROPERTY(GetSoundTagComponent) BGSSoundTagComponent* SoundTagComponent;
				};	
				static_assert(sizeof(TESForm) == 0x28);

				typedef TESForm::Array			TESFormArray;
				typedef BSTArray<std::uint32_t>	TESFormIDArray;
			}
		}
	}
}