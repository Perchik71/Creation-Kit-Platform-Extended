#include <CKPE.Utils.h>
#include <CKPE.Common.RTTI.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/TESFullName.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>

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

				void TESForm::SavePlugin(TESFile* File)
				{
					CKPE::vtbl_call<void>(0xD8, this, File);
				}

				void TESForm::LoadPlugin(TESFile* File)
				{
					CKPE::vtbl_call<void>(0xE0, this, File);
				}

				void TESForm::sub_0E8()
				{
					CKPE::vtbl_call<void>(0xE8, this);
				}

				void TESForm::sub_0F0()
				{
					CKPE::vtbl_call<void>(0xF0, this);
				}

				void TESForm::sub_0F8()
				{
					CKPE::vtbl_call<void>(0xF8, this);
				}

				TESForm* TESForm::CreateDuplicateForm(bool bCreateEditorID, void* Unk) 
				{
					return CKPE::vtbl_call<TESForm*>(0x100, this, bCreateEditorID, Unk);
				}

				void TESForm::sub_108()
				{
					CKPE::vtbl_call<void>(0x108, this);
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

				bool TESForm::BelongsInGroup(TESForm* Group, bool bAllowParentGroups, bool bCurrentOnly)
				{
					return CKPE::vtbl_call<bool>(0x220, this, Group, bAllowParentGroups, bCurrentOnly);
				}

				void TESForm::CreateGroupData(TESForm* OutGroupForm, TESForm* ParentGroup)
				{
					CKPE::vtbl_call<void>(0x228, this, OutGroupForm, ParentGroup);
				}

				std::uint32_t TESForm::GetFormEditorIDLength() const
				{
					__try
					{
						if (!GetFormEditorIDLengthImpl)
							return CKPE::vtbl_call<std::uint32_t>(0x230, this);
						else
							return GetFormEditorIDLengthImpl(this);
					}
					__except (1)
					{
						Patch::Console::LogWarning(Patch::Console::FORMS, 
							"GetFormEditorIDLength return failed");

						return 0;
					}
				}

				const char* TESForm::GetFormEditorID() const
				{
					__try
					{
						if (!GetFormEditorIDImpl)
							return CKPE::vtbl_call<const char*>(0x238, this);
						else
							return GetFormEditorIDImpl(this);
					}
					__except (1)
					{
						Patch::Console::LogWarning(Patch::Console::FORMS,
							"GetFormEditorID return failed");

						return nullptr;
					}
				}

				/////////////

				bool TESForm::UpdateTrackingData() noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2C8, this);
				}

				/////////////

				bool TESForm::HasRequiredEditorID() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x2F8, this);
				}

				bool TESForm::DoVerificationEditorID(HWND hDialogBox) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x300, this, hDialogBox);
				}


				void TESForm::EditFormToWindow(HWND hParentWnd, bool bModalWindow,
					[[maybe_unused]] bool bUnk1, [[maybe_unused]] std::int32_t nUnk2) noexcept(true)
				{
					CKPE::vtbl_call<const char*>(0x340, this, hParentWnd, bModalWindow, bUnk1, nUnk2);
				}

				bool TESForm::HasRequiredUniqueEditorID() const noexcept(true)
				{
					CKPE::vtbl_call<const char*>(0x360, this);
				}

				/////////////

				bool TESForm::WriteWidthTabToSettingFile([[maybe_unused]] std::int64_t nUnk,
					std::int32_t nWidth) noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x378, this, nUnk, nWidth);
				}

				std::int32_t TESForm::ReadWidthTabFromSettingFile() const noexcept(true)
				{
					return CKPE::vtbl_call<std::int32_t>(0x380, this);
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