// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESContainer.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSForcedLocRefType.h>
#include <EditorAPI/B/BGSPropertySheet.h>
#include <EditorAPI/B/BGSNativeTerminalForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;
				class BGSListForm;

				struct CONT_DATA
				{
					std::int8_t contFlags; // 00
				};
				static_assert(sizeof(CONT_DATA) == 0x1);

				// 1.11.240.0: size 0x220, vtable RVA 0x3097668, 154 virtual functions
				class TESObjectCONT :
					public TESBoundAnimObject,         // 000
					public TESContainer,               // 090
					public TESFullName,                // 108
					public BGSModelMaterialSwap,       // 120
					public TESWeightForm,              // 170
					public BGSDestructibleObjectForm,  // 180
					public BGSOpenCloseForm,           // 190
					public BGSKeywordForm,             // 198
					public BGSForcedLocRefType,        // 1B8
					public BGSPropertySheet,           // 1C8
					public BGSNativeTerminalForm,      // 1D8
					public TESMagicCasterForm,         // 1E8
					public TESMagicTargetForm          // 1E9
				{
				public:
					virtual ~TESObjectCONT() = default;

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
					/* 498 */ void CollectInventoryItems(TESContainer* Inventory, std::uint16_t Level, const ContainerItemExtra* ExtraData) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftContainer;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectCONT[0];
				public:
					CONT_DATA data; // 1E9
					std::uint8_t unk1EA[0x6]; // 1EA
					BGSSoundDescriptorForm* openSound; // 1F0
					BGSSoundDescriptorForm* closeSound; // 1F8
					BGSSoundDescriptorForm* takeAllSound; // 200
					BGSListForm* containsOnlyList; // 208
					std::uint8_t unk210[0x10]; // 210
				};
				static_assert(sizeof(TESObjectCONT) == 0x220);
			}
		}
	}
}
