// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundCategory;

				// 1.11.240.0: size 0xE8, vtable RVA 0x2FBA5A0, 124 virtual functions
				class BGSSoundCategorySnapshot :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSoundCategorySnapshot() = default;

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
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftSoundCategorySnapshot;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSoundCategorySnapshot[0];
				public:
					BSTHashMap<BGSSoundCategory*, float> categoryMult; // 28 - CNAM
					std::int8_t priority; // 60
					std::uint8_t unk61[0x7]; // 61
					std::uint8_t unk68[0x80]; // 68
				};
				static_assert(offsetof(BGSSoundCategorySnapshot, categoryMult) == 0x28);
				static_assert(offsetof(BGSSoundCategorySnapshot, priority) == 0x60);
				static_assert(sizeof(BGSSoundCategorySnapshot) == 0xE8);
			}
		}
	}
}
