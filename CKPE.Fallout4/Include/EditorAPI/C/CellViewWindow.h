// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <cstdint>
#include <CKPE.Common.Relocation.h>

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

			// Cell View window list-view helpers.
			namespace CellViewWindow
			{
				// Inserts a form into the Cell View cell list (LVM_INSERTITEM).
				// CK 1.11.246.0 0x14063F780.
				inline void InsertForm(HWND ListView, Forms::TESForm* Form, bool UseImage, std::int32_t ItemIndex) noexcept(true)
				{
					using func_t = decltype(&InsertForm);
					static Common::Relocation<func_t> func{ Common::ID{ 641951, 1463745 } };
					return func(ListView, Form, UseImage, ItemIndex);
				}

				// Inserts an object (reference) into the Cell View object list, 1.10.162.0 only (returns a status there).
				inline std::int32_t InsertObject(HWND ListView, void* Form) noexcept(true)
				{
					using func_t = decltype(&InsertObject);
					static Common::Relocation<func_t> func{ Common::ID{ 643205 } };
					return func(ListView, Form);
				}

				// Inserts an object (reference) into the Cell View object list, 1.10.982.3 and later.
				// CK 1.11.246.0 0x1406713B0.
				inline void InsertObject_ver2(HWND ListView, Forms::TESForm* Form) noexcept(true)
				{
					using func_t = decltype(&InsertObject_ver2);
					static Common::Relocation<func_t> func{ Common::ID{ 1458119 } };
					return func(ListView, Form);
				}
			}
		}
	}
}
