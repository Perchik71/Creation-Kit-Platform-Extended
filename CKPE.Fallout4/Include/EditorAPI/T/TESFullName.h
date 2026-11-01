// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESFullName : public BaseFormComponent
			{
			private:
				BGSLocalizedString name;
			public:
				virtual ~TESFullName() = default;
				/* 10 */ void InitializeDataComponent() override;
				/* 18 */ void ClearDataComponent() override;
				/* 38 */ [[nodiscard]] bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true) override;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 78 */ [[nodiscard]] bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;
				/* 88 */ void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 90 */ void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true) override;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESFullName[0];

				[[nodiscard]] virtual std::uint32_t GetFullNameLength() const noexcept(true);
				[[nodiscard]] virtual const char* GetFullName() const noexcept(true);

				inline operator const char*() const noexcept(true) { return GetFullName(); }
			};	
			static_assert(sizeof(TESFullName) == 0x18);
		}
	}
}