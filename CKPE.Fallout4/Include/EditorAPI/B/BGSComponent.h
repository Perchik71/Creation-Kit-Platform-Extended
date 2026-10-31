// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/B/BGSCraftingUseSound.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESGlobal;
				class TESObjectMISC;

				// 1.11.240.0: size 0xD8, vtable RVA 0x2FB8400, 154 virtual functions
				class BGSComponent :
					public TESBoundObject,       // 000
					public TESFullName,          // 090
					public TESValueForm,         // 0A8
					public BGSCraftingUseSound   // 0B8
				{
				public:
					virtual ~BGSComponent() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

				public:
					TESGlobal* modScrapScalar; // C8
					TESObjectMISC* scrapItem; // D0
				};
				static_assert(sizeof(BGSComponent) == 0xD8);
			}
		}
	}
}
