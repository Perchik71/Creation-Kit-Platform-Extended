// Copyright © 2025-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/Forms/TESForm.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				TESForm::FormType TESForm::GetFormType_orig() const noexcept(true)
				{
					return vtbl_call<FormType>(0x1A8, this);
				}

				std::uint32_t TESForm::GetEditIDLength() const noexcept(true)
				{
					return vtbl_call<std::uint32_t>(0x308, this);
				}

				const char* TESForm::GetEditorID_orig() const noexcept(true)
				{
					return vtbl_call<const char*>(0x310, this);
				}

				const char* TESForm::GetFullName_orig() const noexcept(true)
				{
					return vtbl_call<const char*>(0x2E0, this);
				}

				void TESForm::DebugInfo(char* buffer, std::uint32_t dwSize) const noexcept(true)
				{
					vtbl_call<void>(0x1B0, this, buffer, dwSize);
				}

				void TESForm::MarkAsDeleted(bool state) const noexcept(true)
				{
					vtbl_call<void>(0x230, this, state);
				}

				void TESForm::MarkAsChanged(bool state) const noexcept(true)
				{
					vtbl_call<void>(0x238, this, state);
				}

				void TESForm::ShowEditWindow(void* hParentWindow) const noexcept(true)
				{
					vtbl_call<void>(0x420, this, hParentWindow, 0, 1);
				}
			}
		}
	}
}