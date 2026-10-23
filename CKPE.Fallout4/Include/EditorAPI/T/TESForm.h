// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTBTree.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <cstdint>
#include <windows.h>
#include <commctrl.h>
#include <CKPE.Common.h>
#include <EditorAPI/T/TESFile.h>
#include <EditorAPI/C/CheckInFormList.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/N/NiFlags.h>
#include <EditorAPI/B/BGSMod.h>
#include <EditorAPI/B/BGSSoundTagComponent.h>
#include <EditorAPI/B/BGSPreviewTransform.h>

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
					TESFile** entries;
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
						std::uint16_t date;
						std::uint16_t pad02;
						std::uint8_t lastUser;		// 04 userID that last had this form checked out
						std::uint8_t currentUser;	// 05 userID that has this form checked out

						inline std::uint8_t GetDay() const noexcept(true) { return (std::uint8_t)(date & 0x1F); }
						inline std::uint8_t GetMonth() const noexcept(true) { return (std::uint8_t)((date >> 5) & 0xF); }
						inline std::uint16_t GetYear() const noexcept(true) { return (date >> 9) + 0x7D0; }
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
					/* 0D8 */ virtual bool SavePlugin(TESFile* File);
					/* 0E0 */ virtual void LoadPlugin(TESFile* File);
					/* 0E8 */ virtual bool SaveForm(TESFile* File);
					/* 0F0 */ [[nodiscard]] virtual bool IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true);
					/* 0F8 */ [[nodiscard]] virtual bool IsBeforeForm(TESForm* Rhs) const noexcept(true);
					// CopyMap maps original to copy (the CK's 0x38 byte scatter table).
					/* 100 */ virtual TESForm* CreateDuplicateForm(bool bCreateEditorID, BSTHashMap38<TESForm*, TESForm*>* CopyMap);
					/* 108 */ virtual void FixupDuplicateForm(BSTHashMap38<TESForm*, TESForm*>* CopyMap) noexcept(true);
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
					/* 220 */ virtual bool BelongsInGroup(TESGroupHeader* Group, bool bAllowParentGroups, bool bCurrentOnly);
					/* 228 */ virtual void CreateGroupData(TESGroupHeader* OutGroup, TESGroupHeader* ParentGroup);
					/* 230 */ [[nodiscard]] virtual std::uint32_t GetFormEditorIDLength() const;
					/* 238 */ [[nodiscard]] virtual const char* GetFormEditorID() const;
					/* 240 */ [[nodiscard]] virtual bool IsParentForm() const noexcept(true);
					/* 248 */ [[nodiscard]] virtual bool IsParentFormTree() const noexcept(true);
					/* 250 */ [[nodiscard]] virtual bool IsFormTypeChild(FormType Type) const noexcept(true);
					/* 258 */ virtual void AddReference(TESForm* ReferencingForm) noexcept(true);
					/* 260 */ virtual bool CanCheckOut() noexcept(true);
					/* 268 */ [[nodiscard]] virtual bool CanReferenceDuringCheckIn(TESForm* ReferencingForm, const CheckInFormList* Forms) const noexcept(true);
					/* 270 */ [[nodiscard]] virtual bool HasAlteredChildren() const noexcept(true);
					/* 278 */ virtual void GetObjectWindowTreePath(BSString& Path) const noexcept(true);
					/* 280 */ virtual bool RemoveChildFormsFromList(CheckInFormList* Forms) noexcept(true);
					/* 288 */ virtual bool CanCheckOutChildren() noexcept(true);
					/* 290 */ virtual bool CheckOutChildren(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true);
					/* 298 */ virtual bool MarkChildFormsAsDeleted(bool bValue, TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true);
					/* 2A0 */ virtual void RemoveReferencesToForm(TESForm* Form) noexcept(true);
					/* 2A8 */ [[nodiscard]] virtual bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true);
					/* 2B0 */ virtual void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true);
					/* 2B8 */ virtual void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true);
					/* 2C0 */ virtual bool CheckOut(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true);
					/* 2C8 */ virtual bool UpdateTrackingData() noexcept(true);
					/* 2D0 */ virtual bool UndoCheckOut(TESBitArrayFile* UserFile) noexcept(true);
					/* 2D8 */ virtual void PrepareForRevert() noexcept(true);
					/* 2E0 */ virtual bool Revert() noexcept(true);
					// FormIDMap maps old to new form ids.
					/* 2E8 */ virtual void OnFormIDsRemapped(BSTHashMap38<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true);
					// Context is forwarded only; observed callers pass nullptr.
					/* 2F0 */ virtual bool RecompileScripts(void* Context) noexcept(true);
					/* 2F8 */ [[nodiscard]] virtual bool HasRequiredEditorID() const noexcept(true);
					/* 300 */ [[nodiscard]] virtual bool DoVerificationEditorID(HWND hDialogBox) const noexcept(true);
					// Names differ from BaseFormComponent to avoid overriding its slots
					/* 308 */ virtual bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam,
						LRESULT* pResult) noexcept(true);
					/* 310 */ [[nodiscard]] virtual bool IsSuitableDialogBox(HWND hDialogBox) const noexcept(true);
					/* 318 */ virtual void WriteDataToDialogBox(HWND hDialogBox) noexcept(true);
					/* 320 */ virtual void OnDialogBoxShown(HWND hDialogBox) noexcept(true);
					/* 328 */ virtual void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true);
					/* 330 */ virtual void CleanupDialogBox(HWND hDialogBox) noexcept(true);
					/* 338 */ virtual void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true);
					// Reserved flag is stored in DialogExtraParam; no reader is known.
					// Template requests use -1 to suppress the object-template dialog.
					/* 340 */ virtual void EditFormToWindow(HWND hParentWnd, bool bModalWindow,
						[[maybe_unused]] bool bReserved, std::int32_t nObjectTemplateRequest) noexcept(true);
					/* 348 */ virtual void FillListViewItem(NMLVDISPINFOA* pDispInfo) const noexcept(true);
					// Column is 1-based; negative values select descending order
					/* 350 */ [[nodiscard]] virtual std::int32_t CompareListViewItems(const TESForm* Rhs, LPARAM nSortColumn) const noexcept(true);
					/* 358 */ virtual void InitializeListView(HWND hListView) const noexcept(true);
					/* 360 */ [[nodiscard]] virtual bool HasRequiredUniqueEditorID() const noexcept(true);
					/* 368 */ virtual void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true);
					// Visited forms and the required resource file names.
					/* 370 */ virtual void CollectRequiredFiles(BSTBTree<TESForm*>* VisitedForms, BSTBTree<BSFixedString>* RequiredFiles) noexcept(true);
					/* 378 */ virtual BOOL SaveSelectedDialogTab(HWND hDialogBox, std::int32_t nTabIndex) noexcept(true);
					/* 380 */ [[nodiscard]] virtual std::int32_t GetSavedDialogTab(HWND hDialogBox) const noexcept(true);
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
					// CommonLibF4: TESForm::formEditorID-like name entry (BSFixedString at +0x18); for BGSHeadPart it is the name used to find the head part geometry.
					[[nodiscard]] inline const BSFixedString& GetEntryEditID() const noexcept(true) { return _EntryEditID; }

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

					[[nodiscard]] inline const BSModInfo* GetModInfo() const noexcept(true) { return _ModInfo; }
					[[nodiscard]] inline BSTrackingData GetTrackingData() const noexcept(true) { return _TrackingData; }

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