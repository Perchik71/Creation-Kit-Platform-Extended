// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptor;

				// 1.11.240.0: size 0x48, vtable RVA 0x3062A10, 124 virtual functions
				class BGSSoundDescriptorForm :
					public TESForm,             // 000
					public BSISoundDescriptor   // 028
				{
				public:
					virtual ~BGSSoundDescriptorForm() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2E8 */ void OnFormIDsRemapped(BSTHashMap<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true) override;
					/* 300 */ [[nodiscard]] bool DoVerificationEditorID(HWND hDialogBox) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftSoundDescriptorForm;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSoundDescriptorForm[0];
				public:
					BSFixedString unk30; // 30
					BGSSoundDescriptor* impl; // 38 - CNAM, created from the sound descriptor type id through BSTCreateFactory
					BSFixedString notes; // 40 - NNAM
				};
				static_assert(sizeof(BGSSoundDescriptorForm) == 0x48);
			}
		}
	}
}
