// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x58, vtable RVA 0x305C430, 124 virtual functions
				class BGSListForm :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~BGSListForm() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftListForm;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSListForm[0];
				public:
					BSTArray<TESForm*> arrayOfForms; // 40
				};
				static_assert(sizeof(BGSListForm) == 0x58);
			}
		}
	}
}
