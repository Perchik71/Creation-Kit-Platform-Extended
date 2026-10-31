// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;
				class BGSSoundDescriptorForm;

				// 1.11.240.0: size 0xB8, vtable RVA 0x2FBAC48, 124 virtual functions
				class BGSSoundKeywordMapping :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSoundKeywordMapping() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 300 */ [[nodiscard]] bool DoVerificationEditorID(HWND hDialogBox) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 360 */ [[nodiscard]] bool HasRequiredUniqueEditorID() const noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftSoundKeywordMapping;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSoundKeywordMapping[0];
				public:
					BSTSet<BGSKeyword*> keywordSet; // 28 - KNAM, 0x38 byte table (capacity 34, entries 58, 16 byte entries)
					BSTHashMap<std::uint32_t, BGSSoundDescriptorForm*> reverbDescMap; // 60 - PTRN, 0x38 byte table (capacity 6C, entries 90, 24 byte entries)
					BGSSoundDescriptorForm* descriptor; // 98 - KNAM
					BGSSoundDescriptorForm* exteriorTail; // A0 - KNAM
					BGSSoundDescriptorForm* vatsDescriptor; // A8 - PTRN
					float vatsDescThreshold; // B0 - PTRN
					std::uint32_t unkB4; // B4
				};
				static_assert(sizeof(BGSSoundKeywordMapping) == 0xB8);
			}
		}
	}
}
