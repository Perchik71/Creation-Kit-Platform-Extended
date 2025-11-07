// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uint32_t BaseFormComponent::GetFormComponentType() const noexcept(true)
			{ 
				return this_call<std::uint32_t>(0x8, this);
			}

			void BaseFormComponent::InitializeDataComponent()
			{ 
				this_call<void>(0x10, this);
			}

			void BaseFormComponent::ClearDataComponent()
			{ 
				this_call<void>(0x18, this);
			}

			void BaseFormComponent::InitComponent()
			{ 
				this_call<void>(0x20, this);
			}

			void BaseFormComponent::CopyComponent(BaseFormComponent* Copy) noexcept(true)
			{ 
				this_call<void>(0x28, this, Copy);
			}

			void BaseFormComponent::CopyComponent(BaseFormComponent* Copy, Forms::TESForm* Owner) noexcept(true)
			{ 
				this_call<void>(0x30, this, Copy, Owner);
			}

			bool BaseFormComponent::IsEmptyData() const noexcept(true)
			{ 
				return this_call<bool>(0x38, this);
			}

			void BaseFormComponent::sub_40() { return; } // 08

			bool BaseFormComponent::sub_48(Forms::TESForm* arg1, std::int64_t arg2) noexcept(true)
			{ 
				return this_call<bool>(0x48, this, arg1, arg2);
			}

			bool BaseFormComponent::sub_50(std::int64_t arg) noexcept(true)
			{
				return this_call<bool>(0x50, this, arg);
			}

			void BaseFormComponent::sub_58() { return; } // 11
			void BaseFormComponent::sub_60() { return; } // 12
			void BaseFormComponent::sub_68() { return; } // 13

			bool BaseFormComponent::Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{ 
				return this_call<bool>(0x70, this, hDialogBox, uMsg, wParam, lParam);
			}

			bool BaseFormComponent::IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true)
			{ 
				return this_call<bool>(0x78, this, hDialogBox);
			}

			bool BaseFormComponent::UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true)
			{ 
				return this_call<bool>(0x80, this, hDialogBox);
			}

			void BaseFormComponent::UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true)
			{ 
				this_call<void>(0x88, this, hDialogBox);
			}

			void BaseFormComponent::DoUpdate(HWND hDialogBox) noexcept(true)
			{ 
				this_call<void>(0x90, this, hDialogBox);
			}

			void BaseFormComponent::sub_98(std::int64_t* arg) 
			{
				this_call<void>(0x98, this, arg);
			}

			void BaseFormComponent::sub_A0() { return; } // 20
		}
	}
}