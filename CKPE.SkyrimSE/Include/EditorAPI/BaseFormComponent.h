// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <windows.h>
#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BaseFormComponent
			{
			public:
				virtual ~BaseFormComponent() = default;

				virtual void InitializeDataComponent();
				virtual void ClearDataComponent();
				virtual void CopyComponent(BaseFormComponent* Copy) noexcept(true);
				[[nodiscard]] virtual bool IsEmptyData() const noexcept(true);
				virtual void nullsub_28() const noexcept(true);
				[[nodiscard]] virtual bool nullsub_30() const noexcept(true);
				virtual void nullsub_38() const noexcept(true);
				virtual bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam);
				[[nodiscard]] virtual bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true);
				virtual bool UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true);
				virtual void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true);
				virtual void nullsub_60() const noexcept(true);
				virtual void nullsub_68() const noexcept(true);
			};
			static_assert(sizeof(BaseFormComponent) == 0x8);
		}
	}
}