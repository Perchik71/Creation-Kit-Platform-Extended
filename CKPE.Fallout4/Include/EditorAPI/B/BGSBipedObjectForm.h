// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x10, vtable RVA 0x3018898, 22 virtual functions
			class BGSBipedObjectForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSBipedObjectForm() = default;
				/* 10 */ void InitializeDataComponent() override;
				/* 18 */ void ClearDataComponent() override;
				/* 20 */ void InitComponent() override;
				/* 38 */ [[nodiscard]] bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true) override;
				/* 60 */ void BuildComponentCheckInDependencyMessage(const Forms::CheckInFormList* FormSet, BSString& Message) const noexcept(true) override;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 78 */ [[nodiscard]] bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;
				/* 88 */ void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 90 */ void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 98 */ void CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

				virtual bool FillsBipedSlot(std::int32_t nBipedSlot) const noexcept(true) { return false; } // 21 - RVA 0x88BA90, if ( a2 >= 32 ); return 0;; v3 = *(_DWORD *)(a1 + 8);; return _bittest(&v3, a2);

			public:
				std::uint32_t bipedModelData; // 08 - BIPED_MODEL, biped slot mask
				std::uint32_t pad0C; // 0C
			};
			static_assert(sizeof(BGSBipedObjectForm) == 0x10);
		}
	}
}
