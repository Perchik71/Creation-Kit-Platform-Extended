// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSImpactDataSet;

				// 1.11.240.0: size 0x90, vtable RVA 0x305D4D8, 124 virtual functions
				class BGSMaterialType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSMaterialType() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftMaterialType;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMaterialType[0];
				public:
					TESModel breakableFXModel; // 28
					BGSMaterialType* parentType; // 60
					BGSImpactDataSet* havokImpactDataSet; // 68
					BSFixedString materialName; // 70
					std::uint32_t materialID; // 78
					NiAPI::NiColor materialColor; // 7C
					float buoyancy; // 88
					std::uint32_t flags; // 8C
				};
				static_assert(sizeof(BGSMaterialType) == 0x90);
			}
		}
	}
}
