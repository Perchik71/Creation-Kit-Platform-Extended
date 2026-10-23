#include <CKPE.Utils.h>
#include <CKPE.Common.RTTI.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				void TESForm::InitializeData()
				{
					CKPE::vtbl_call<void>(0xA8, this);
				}

				void TESForm::ClearData()
				{
					CKPE::vtbl_call<void>(0xB0, this);
				}

				void TESForm::Load(TESFile* File)

				{
					CKPE::vtbl_call<void>(0xB8, this, File);
				}

				void TESForm::LoadPartial(TESFile* File)
				{
					CKPE::vtbl_call<void>(0xC0, this, File);
				}

				void TESForm::LoadEdit(TESFile* File)
				{
					CKPE::vtbl_call<void>(0xC8, this, File);
				}

				void TESForm::Save(TESFile* File)
				{
					CKPE::vtbl_call<void>(0xD0, this, File);
				}

				bool TESForm::SavePlugin(TESFile* File)
				{
					return CKPE::vtbl_call<bool>(0xD8, this, File);
				}

				void TESForm::LoadPlugin(TESFile* File)
				{
					CKPE::vtbl_call<void>(0xE0, this, File);
				}

				bool TESForm::SaveForm(TESFile* File)
				{
					return CKPE::vtbl_call<bool>(0xE8, this, File);
				}

				bool TESForm::IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0xF0, this, GroupRecord);
				}

				bool TESForm::IsBeforeForm(TESForm* Rhs) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0xF8, this, Rhs);
				}

				TESForm* TESForm::CreateDuplicateForm(bool bCreateEditorID, BSTHashMap<TESForm*, TESForm*>* CopyMap)
				{
					return CKPE::vtbl_call<TESForm*>(0x100, this, bCreateEditorID, CopyMap);
				}

				void TESForm::FixupDuplicateForm(BSTHashMap<TESForm*, TESForm*>* CopyMap) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x108, this, CopyMap);
				}

				void TESForm::InitItemImpl() noexcept(true)
				{
					CKPE::vtbl_call<void>(0x110, this);
				}

				TESFile* TESForm::GetDescriptionOwnerFile() const noexcept(true)
				{
					return CKPE::vtbl_call<TESFile*>(0x118, this);
				}

				TESForm::FormType TESForm::GetFormType() const noexcept(true)
				{ 
					return CKPE::vtbl_call<FormType>(0x120, this);
				}

				void TESForm::DebugInfo(char* lpBuffer, std::uint32_t nBufferSize) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x128, this, lpBuffer, nBufferSize);
				}

				bool TESForm::HasKnown() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x130, this);
				}

				bool TESForm::HasRandomAnim() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x138, this);
				}

				bool TESForm::HasPlayable() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x140, this);
				}

				bool TESForm::IsHeadingMarker() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x148, this);
				}

				bool TESForm::IsHeadtrackMarker() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x150, this);
				}

				bool TESForm::HasDangerous() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x158, this);
				}

				bool TESForm::HasCurrents() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x160, this);
				}

				bool TESForm::HasObstacle() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x168, this);
				}

				bool TESForm::IsLODLandObject() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x170, this);
				}

				bool TESForm::HasOnLocalMap() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x178, this);
				}

				bool TESForm::HasMustUpdate() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x180, this);
				}

				bool TESForm::SetOnLocalMap(bool bValue) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x188, this, bValue);
				}

				bool TESForm::HasIgnoredBySandbox() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x190, this);
				}

				void TESForm::MarkAsDeleted(bool bValue) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x198, this, bValue);
				}

				void TESForm::MarkAsChanged(bool bValue) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x1A0, this, bValue);
				}

				TESForm::NavMeshGenerationImportOption TESForm::GetNavMeshGenerationImportOption() const noexcept(true)
				{
					return CKPE::vtbl_call<TESForm::NavMeshGenerationImportOption>(0x1A8, this);
				}

				void TESForm::SaveObjectBound()
				{
					CKPE::vtbl_call<void>(0x1B0, this);
				}

				void TESForm::LoadObjectBound(TESFile* pFile)
				{
					CKPE::vtbl_call<void>(0x1B8, this, pFile);
				}

				void TESForm::SavePreviewTransform()
				{
					CKPE::vtbl_call<void>(0x1C0, this);
				}

				void TESForm::LoadPreviewTransform(TESFile* pFile)
				{
					CKPE::vtbl_call<void>(0x1C8, this, pFile);
				}

				bool TESForm::IsBoundObject() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x1D0, this);
				}

				bool TESForm::IsObject() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x1D8, this);
				}

				bool TESForm::IsMagicItem() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x1E0, this);
				}

				bool TESForm::IsWater() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x1E8, this);
				}

				TESForm* TESForm::AsReference() noexcept(true)
				{
					return CKPE::vtbl_call<TESForm*>(0x1F0, this);
				}

				const TESForm* TESForm::AsReference() const noexcept(true)
				{
					return CKPE::vtbl_call<const TESForm*>(0x1F8, this);
				}

				std::uint32_t TESForm::GetRefCount() const noexcept(true)
				{
					return CKPE::vtbl_call<std::uint32_t>(0x200, this);
				}

				const char* TESForm::GetTextForParsedSubTag(const BSFixedString& sSubTag) const noexcept(true)
				{
					return CKPE::vtbl_call<const char*>(0x208, this, &sSubTag);
				}
				
				void TESForm::CopyFrom(TESForm* Source) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x210, this, Source);
				}

				std::int32_t TESForm::Compare(TESForm* Rhs) const noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x218, this, Rhs);
				}

				bool TESForm::BelongsInGroup(TESGroupHeader* Group, bool bAllowParentGroups, bool bCurrentOnly)
				{
					return CKPE::vtbl_call<bool>(0x220, this, Group, bAllowParentGroups, bCurrentOnly);
				}

				void TESForm::CreateGroupData(TESGroupHeader* OutGroup, TESGroupHeader* ParentGroup)
				{
					CKPE::vtbl_call<void>(0x228, this, OutGroup, ParentGroup);
				}

				std::uint32_t TESForm::GetFormEditorIDLength() const
				{
					return CKPE::vtbl_call<std::uint32_t>(0x230, this);
				}

				const char* TESForm::GetFormEditorID() const
				{
					return CKPE::vtbl_call<const char*>(0x238, this);
				}

				bool TESForm::IsParentForm() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x240, this);
				}

				bool TESForm::IsParentFormTree() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x248, this);
				}

				bool TESForm::IsFormTypeChild(FormType Type) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x250, this, Type);
				}

				void TESForm::AddReference(TESForm* ReferencingForm) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x258, this, ReferencingForm);
				}

				bool TESForm::CanCheckOut() noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x260, this);
				}

				bool TESForm::CanReferenceDuringCheckIn(TESForm* ReferencingForm, const CheckInFormList* Forms) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x268, this, ReferencingForm, Forms);
				}

				bool TESForm::HasAlteredChildren() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x270, this);
				}

				void TESForm::GetObjectWindowTreePath(BSString& Path) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x278, this, &Path);
				}

				bool TESForm::RemoveChildFormsFromList(CheckInFormList* Forms) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x280, this, Forms);
				}

				bool TESForm::CanCheckOutChildren() noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x288, this);
				}

				bool TESForm::CheckOutChildren(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x290, this, UserFile, VersionFile);
				}

				bool TESForm::MarkChildFormsAsDeleted(bool bValue, TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x298, this, bValue, UserFile, VersionFile);
				}

				void TESForm::RemoveReferencesToForm(TESForm* Form) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x2A0, this, Form);
				}

				bool TESForm::CanCheckIn(const CheckInFormList* Forms) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2A8, this, Forms);
				}

				void TESForm::BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x2B0, this, Forms, &Message);
				}

				void TESForm::GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x2B8, this, OutForms);
				}

				bool TESForm::CheckOut(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2C0, this, UserFile, VersionFile);
				}

				bool TESForm::UpdateTrackingData() noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2C8, this);
				}

				bool TESForm::UndoCheckOut(TESBitArrayFile* UserFile) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2D0, this, UserFile);
				}

				void TESForm::PrepareForRevert() noexcept(true)
				{
					CKPE::vtbl_call<void>(0x2D8, this);
				}

				bool TESForm::Revert() noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2E0, this);
				}

				void TESForm::OnFormIDsRemapped(BSTHashMap<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x2E8, this, FormIDMap);
				}

				bool TESForm::RecompileScripts(void* Context) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2F0, this, Context);
				}

				bool TESForm::HasRequiredEditorID() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2F8, this);
				}

				bool TESForm::DoVerificationEditorID(HWND hDialogBox) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x300, this, hDialogBox);
				}

				bool TESForm::PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam,
					LRESULT* pResult) noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x308, this, hDialogBox, uMsg, wParam, lParam, pResult);
				}

				bool TESForm::IsSuitableDialogBox(HWND hDialogBox) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x310, this, hDialogBox);
				}

				void TESForm::WriteDataToDialogBox(HWND hDialogBox) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x318, this, hDialogBox);
				}

				void TESForm::OnDialogBoxShown(HWND hDialogBox) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x320, this, hDialogBox);
				}

				void TESForm::ReadDataFromDialogBox(HWND hDialogBox) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x328, this, hDialogBox);
				}

				void TESForm::CleanupDialogBox(HWND hDialogBox) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x330, this, hDialogBox);
				}

				void TESForm::OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x338, this, hDialogBox, SavedForm);
				}

				void TESForm::EditFormToWindow(HWND hParentWnd, bool bModalWindow,
					[[maybe_unused]] bool bReserved, std::int32_t nObjectTemplateRequest) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x340, this, hParentWnd, bModalWindow, bReserved, nObjectTemplateRequest);
				}

				void TESForm::FillListViewItem(NMLVDISPINFOA* pDispInfo) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x348, this, pDispInfo);
				}

				std::int32_t TESForm::CompareListViewItems(const TESForm* Rhs, LPARAM nSortColumn) const noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x350, this, Rhs, nSortColumn);
				}

				void TESForm::InitializeListView(HWND hListView) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x358, this, hListView);
				}

				bool TESForm::HasRequiredUniqueEditorID() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x360, this);
				}

				void TESForm::CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x368, this, &Files);
				}

				void TESForm::CollectRequiredFiles(BSTBTree<TESForm*>* VisitedForms, BSTBTree<BSFixedString>* RequiredFiles) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x370, this, VisitedForms, RequiredFiles);
				}

				/////////////

				BOOL TESForm::SaveSelectedDialogTab(HWND hDialogBox,
					std::int32_t nTabIndex) noexcept(true)
				{
					return CKPE::vtbl_call<BOOL>(0x378, this, hDialogBox, nTabIndex);
				}

				std::int32_t TESForm::GetSavedDialogTab(HWND hDialogBox) const noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x380, this, hDialogBox);
				}

				std::int32_t TESForm::GetSafeFormEditorID(char* lpBuffer, std::uint32_t nBufferSize) noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x388, this, lpBuffer, nBufferSize);
				}

				std::int32_t TESForm::GetSafeFormEditorID(char* lpBuffer, std::uint32_t nBufferSize) const noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x390, this, lpBuffer, nBufferSize);
				}

				void TESForm::SetNewFormID(std::uint32_t nNewFormID, bool bUpdateFile) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x398, this, nNewFormID, bUpdateFile);
				}

				const char* TESForm::GetObjectTypeName() const noexcept(true)
				{
					return CKPE::vtbl_call<const char*>(0x3A0, this);
				}

				bool TESForm::HasAvailableInGame() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x3A8, this);
				}

				BGSMod::Template::Items* TESForm::GetObjectTemplate() const noexcept(true)
				{
					return CKPE::vtbl_call<BGSMod::Template::Items*>(0x3B0, this);
				}

				BGSPreviewTransform* TESForm::GetPreviewTransform() const noexcept(true)
				{
					return CKPE::vtbl_call<BGSPreviewTransform*>(0x3B8, this);
				}

				BGSSoundTagComponent* TESForm::GetSoundTagComponent() const noexcept(true)
				{
					return CKPE::vtbl_call<BGSSoundTagComponent*>(0x3C0, this);
				}

				std::uint32_t TESForm::GetFilledSlots() const noexcept(true)
				{
					return CKPE::vtbl_call<std::uint32_t>(0x3C8, this);
				}

				std::uint32_t TESForm::GetFilledSlotsImpl() const noexcept(true)
				{
					return CKPE::vtbl_call<std::uint32_t>(0x3D0, this);
				}

				float TESForm::GetDesirability() const noexcept(true)
				{
					return CKPE::vtbl_call<float>(0x3D8, this);
				}

				const char* TESForm::TryGetFullName() const noexcept(true)
				{
					TESFullName* fullname = (TESFullName*)_DYNAMIC_CAST(this, 0, "class TESForm", "class TESFullName");
					return fullname ? fullname->GetFullName() : "";
				}

				TESForm::ENUM_FORM_ID TESForm::GetSaveFormType() const noexcept(true)
				{
					return EnumFormIDs[_FormType];
				}

				bool TESForm::SetFormEditorID(const char* NewEditorID) noexcept(true)
				{
					return SetFormEditorIDImpl ? SetFormEditorIDImpl(this, NewEditorID) : false;
				}
			}
		}
	}
}