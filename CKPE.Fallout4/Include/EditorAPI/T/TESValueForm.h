// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESValueForm : public BaseFormComponent
			{
				// members
				std::uint32_t value;
			public:
				virtual ~TESValueForm() = default;
				/* 10 */ void InitializeDataComponent() override;
				/* 18 */ void ClearDataComponent() override;
				/* 38 */ [[nodiscard]] bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true) override;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 78 */ [[nodiscard]] bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;
				/* 88 */ void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 90 */ void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true) override;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESValueForm[0];

				[[nodiscard]] inline std::uint32_t GetValue() const noexcept(true) { return value; }
				inline void SetValue(std::uint32_t a_value) noexcept(true) { value = a_value; }

				CKPE_PROPERTY(GetValue, SetValue) std::uint32_t Value;
			};
			static_assert(sizeof(TESValueForm) == 0x10);
		}
	}
}