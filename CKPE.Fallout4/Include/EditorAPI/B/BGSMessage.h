// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMenuIcon;
				class TESQuest;

				// 1.11.240.0: size 0xA8, vtable RVA 0x305E4E0, 124 virtual functions
				class BGSMessage :
					public TESForm,         // 000
					public TESFullName,     // 028
					public TESDescription   // 040
				{
				public:
					virtual ~BGSMessage() = default;

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

					constexpr static std::uint8_t TYPE_ID = ftMessage;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMessage[0];
				public:
					BGSMenuIcon* icon; // 68
					TESQuest* ownerQuest; // 70
					BSSimpleList<void*> buttonList; // 78 - BSSimpleList<MESSAGEBOX_BUTTON*>
					BSFixedString swfFile; // 88 - BSFixedStringCS
					BGSLocalizedString shortName; // 90
					std::uint32_t flags; // A0
					std::uint32_t displayTime; // A4
				};
				static_assert(sizeof(BGSMessage) == 0xA8);
			}
		}
	}
}
