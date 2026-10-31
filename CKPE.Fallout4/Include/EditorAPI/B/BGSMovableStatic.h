// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectSTAT.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				struct MOVABLE_STATIC_DATA
				{
					std::int8_t flags; // 00
				};
				static_assert(sizeof(MOVABLE_STATIC_DATA) == 0x1);

				// 1.11.240.0: size 0x198, vtable RVA 0x3081EB0, 154 virtual functions
				class BGSMovableStatic :
					public TESObjectSTAT,              // 000
					public BGSDestructibleObjectForm,  // 148
					public BGSKeywordForm              // 158
				{
				public:
					virtual ~BGSMovableStatic() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftMovableStatic;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMovableStatic[0];
				public:
					BGSSoundDescriptorForm* soundLoop; // 178
					MOVABLE_STATIC_DATA data; // 180
					std::uint8_t unk181[0x7]; // 181
					std::uint8_t unk188[0x10]; // 188
				};
				static_assert(sizeof(BGSMovableStatic) == 0x198);
			}
		}
	}
}
