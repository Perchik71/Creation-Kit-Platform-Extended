// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSTypedFormValuePair.h>
#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/T/TESCondition.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;

				struct ConstructibleObjectData
				{
					std::uint16_t numConstructed; // 00
					std::uint16_t workshopPriority; // 02
				};
				static_assert(sizeof(ConstructibleObjectData) == 0x4);

				// 1.11.240.0: size 0xB0, vtable RVA 0x307EED8, 124 virtual functions
				class BGSConstructibleObject :
					public TESForm,                 // 000
					public BGSPickupPutdownSounds,  // 028
					public TESDescription           // 040
				{
				public:
					virtual ~BGSConstructibleObject() = default;

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
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftConstructibleObject;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSConstructibleObject[0];
				public:
					BGSTypedFormValuePair::Array* requiredItems; // 68
					TESCondition conditions; // 70
					TESForm* createdItem; // 88
					BGSKeyword* benchKeyword; // 90
					ConstructibleObjectData data; // 98
					std::uint32_t pad9C; // 9C
					BGSKeyword** filterKeywords; // A0 - the game's recipe filter keyword array (pointer and count)
					std::uint32_t filterKeywordCount; // A8
					std::uint32_t padAC; // AC
				};
				static_assert(sizeof(BGSConstructibleObject) == 0xB0);
				static_assert(offsetof(BGSConstructibleObject, conditions) == 0x70);
				static_assert(offsetof(BGSConstructibleObject, createdItem) == 0x88);
				static_assert(offsetof(BGSConstructibleObject, benchKeyword) == 0x90);
				static_assert(offsetof(BGSConstructibleObject, filterKeywords) == 0xA0);
			}
		}
	}
}
