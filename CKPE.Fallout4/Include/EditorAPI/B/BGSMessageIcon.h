// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/T/TESIcon.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x30, vtable RVA 0x3019F98, 21 virtual functions
			class BGSMessageIcon :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSMessageIcon() = default;
				/* 10 */ void InitializeDataComponent() override;
				/* 18 */ void ClearDataComponent() override;
				/* 38 */ [[nodiscard]] bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true) override;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 78 */ [[nodiscard]] bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;
				/* 88 */ void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 90 */ void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 98 */ void CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMessageIcon[0];
			public:
				TESIcon icon; // 08
			};
			static_assert(sizeof(BGSMessageIcon) == 0x30);
		}
	}
}
