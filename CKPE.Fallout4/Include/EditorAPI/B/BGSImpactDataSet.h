// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSPreloadable.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMaterialType;
				class BGSImpactData;
				// 1.11.240.0: size 0x68, vtable RVA 0x305BB50, 124 virtual functions
				class BGSImpactDataSet :
					public TESForm,         // 000
					public BGSPreloadable   // 028
				{
				public:
					virtual ~BGSImpactDataSet() = default;

					/* 0B0 */ void ClearData() override;
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

					constexpr static std::uint8_t TYPE_ID = ftImpactDataSet;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSImpactDataSet[0];
				public:
					// 0x38 byte table (capacity 3C, good 44, modification count 50, entries 60); ClearData walks 24 byte entries {material, impact data, next}
					BSTHashMap<const BGSMaterialType*, BGSImpactData*> impactMap; // 30
				};
				static_assert(sizeof(BGSImpactDataSet) == 0x68);
			}
		}
	}
}
