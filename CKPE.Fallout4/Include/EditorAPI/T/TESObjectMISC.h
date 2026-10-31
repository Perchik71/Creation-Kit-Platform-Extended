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
				// 1.11.246.0: size 0x1E8, vtable RVA 0x309AA10, 156 virtual functions
				class TESObjectMISC :
					public TESBoundObject,             // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public TESIcon,                    // 0F8
					public TESValueForm,               // 120
					public TESWeightForm,              // 130
					public BGSDestructibleObjectForm,  // 140
					public BGSMessageIcon,             // 150
					public BGSPickupPutdownSounds,     // 180
					public BGSKeywordForm,             // 198
					public BGSFeaturedItemMessage      // 1B8
				{
				public:
					virtual ~TESObjectMISC() = default;

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

					constexpr static std::uint8_t TYPE_ID = ftMiscItem;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectMISC[0];

					virtual void SaveImpl() noexcept(true) { return; } // 154
					virtual void LoadImpl(TESFile* File, std::uint32_t ChunkID) noexcept(true) { return; } // 155

				public:
					BSTArray<void*> componentData; // 1C8 - BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>*
					std::uint64_t unk1E0; // 1E0
				};
				static_assert(sizeof(TESObjectMISC) == 0x1E8);
			}
		}
	}
}
