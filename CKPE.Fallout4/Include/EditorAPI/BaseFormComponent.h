// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <windows.h>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
			}

			class BaseFormComponent
			{
			public:
				virtual ~BaseFormComponent() = default;
				[[nodiscard]] virtual std::uint32_t GetFormComponentType() const noexcept(true);
				virtual void InitializeDataComponent();
				virtual void ClearDataComponent();
				virtual void InitComponent();
				virtual void CopyComponent(BaseFormComponent* Copy) noexcept(true);
				virtual void CopyComponent(BaseFormComponent* Copy, Forms::TESForm* Owner) noexcept(true);
				[[nodiscard]] virtual bool IsEmptyData() const noexcept(true);
				virtual void sub_40();
				virtual bool sub_48(Forms::TESForm* Owner, std::int64_t arg2) noexcept(true);
				virtual bool sub_50(std::int64_t arg) noexcept(true);
				virtual void sub_58();
				virtual void sub_60();
				virtual void sub_68();
				virtual bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam);
				[[nodiscard]] virtual bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true);
				virtual bool UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true);
				virtual void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true);
				virtual void DoUpdate(HWND hDialogBox) noexcept(true);
				virtual void sub_98(std::int64_t* arg);
				virtual void sub_A0();
			};
			static_assert(sizeof(BaseFormComponent) == 0x8);
		}
	}
}