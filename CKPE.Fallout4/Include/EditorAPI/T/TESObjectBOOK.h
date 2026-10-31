// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSFeaturedItemMessage.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectSTAT;

				// 1.11.240.0: size 0x248, vtable RVA 0x3095FA8, 154 virtual functions
				class TESObjectBOOK :
					public TESBoundObject,             // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public TESIcon,                    // 0F8
					public TESValueForm,               // 120
					public TESWeightForm,              // 130
					public TESDescription,             // 140
					public BGSDestructibleObjectForm,  // 168
					public BGSMessageIcon,             // 178
					public BGSPickupPutdownSounds,     // 1A8
					public BGSKeywordForm,             // 1C0
					public BGSFeaturedItemMessage      // 1E0
				{
				public:
					virtual ~TESObjectBOOK() = default;

					/* 0A8 */ void InitializeData() override;
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
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftBook;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectBOOK[0];
				public:
					std::uint8_t data[0x18]; // 1F0 - OBJ_BOOK
					TESObjectSTAT* inventoryModel; // 208
					std::uint64_t unk210; // 210
					std::uint64_t unk218; // 218
					TESDescription unk220; // 220
				};
				static_assert(sizeof(TESObjectBOOK) == 0x248);
			}
		}
	}
}
