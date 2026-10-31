// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESTopic;
				// size = 0xE0
				// func = 126
				class TESTopicInfo :
					public TESForm   // 000
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTopicInfo;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESTopicInfo[0];

					virtual ~TESTopicInfo() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 0F0 */ [[nodiscard]] bool IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true) override;
					/* 0F8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 128 */ void DebugInfo(char* lpBuffer, std::uint32_t nBufferSize) const noexcept(true) override;
					/* 198 */ void MarkAsDeleted(bool bValue) noexcept(true) override;
					/* 1A0 */ void MarkAsChanged(bool bValue) noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ bool BelongsInGroup(TESGroupHeader* Group, bool bAllowParentGroups, bool bCurrentOnly) override;
					/* 228 */ void CreateGroupData(TESGroupHeader* OutGroup, TESGroupHeader* ParentGroup) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2C0 */ bool CheckOut(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 2C8 */ bool UpdateTrackingData() noexcept(true) override;
					/* 2D8 */ void PrepareForRevert() noexcept(true) override;
					/* 2E0 */ bool Revert() noexcept(true) override;
					/* 2E8 */ void OnFormIDsRemapped(BSTHashMap<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 320 */ void OnDialogBoxShown(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;
					/* 360 */ [[nodiscard]] bool HasRequiredUniqueEditorID() const noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
					virtual bool CopyInfoDataTo(void* Context, TESTopicInfo* Target) const noexcept(true) { return false; } // 124
					virtual bool CopyResponseDataTo(void* Context, TESTopicInfo* Target) const noexcept(true) { return false; } // 125
				public:
					struct Response
					{
						BGSLocalizedStringIL ResponseText;
						Response* Next;
						char pad18[0x2];
						std::uint8_t Index;

						inline bool End() const { return Next == nullptr; }
					};

					inline TESTopic* GetParentTopic() const { return _ParentTopic; }
					inline Response* GetFirstResponse() const { return _First; }
				private:
					char pad28[0x8];
					TESTopic* _ParentTopic;
					char pad38[0x30];
					Response* _First;
					char pad70[0x70];
				};
				static_assert(sizeof(TESTopicInfo) == 0xE0);
			}
		}
	}
}