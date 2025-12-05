// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			void BaseFormComponent::InitializeDataComponent()
			{ 
				this_call<void>(0x8, this);
			}

			void BaseFormComponent::ClearDataComponent()
			{ 
				this_call<void>(0x10, this);
			}

			void BaseFormComponent::CopyComponent(BaseFormComponent* Copy) noexcept(true)
			{ 
				this_call<void>(0x18, this, Copy);
			}

			bool BaseFormComponent::IsEmptyData() const noexcept(true)
			{ 
				return this_call<bool>(0x20, this);
			}

			void BaseFormComponent::nullsub_28() const noexcept(true) { return; }
			bool BaseFormComponent::nullsub_30() const noexcept(true) { return true; }
			void BaseFormComponent::nullsub_38() const noexcept(true) { return; }

			bool BaseFormComponent::Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{ 
				return this_call<bool>(0x40, this, hDialogBox, uMsg, wParam, lParam);
			}

			bool BaseFormComponent::IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true)
			{ 
				return this_call<bool>(0x48, this, hDialogBox);
			}

			bool BaseFormComponent::UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true)
			{ 
				return this_call<bool>(0x50, this, hDialogBox);
			}

			void BaseFormComponent::UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true)
			{ 
				this_call<void>(0x58, this, hDialogBox);
			}

			void BaseFormComponent::nullsub_60() const noexcept(true) { return; }
			void BaseFormComponent::nullsub_68() const noexcept(true) { return; } // 20
		}
	}
}