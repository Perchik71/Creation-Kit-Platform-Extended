// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x58, vtable RVA 0x305C430, 124 virtual functions
				class BGSListForm :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~BGSListForm() = default;

				public:
					BSTArray<TESForm*> arrayOfForms; // 40
				};
				static_assert(sizeof(BGSListForm) == 0x58);
			}
		}
	}
}
