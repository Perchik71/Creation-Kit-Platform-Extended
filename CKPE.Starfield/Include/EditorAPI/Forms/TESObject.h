// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "TESCellUseList.h"
#include "BGSSnapTemplateComponent.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObject : public TESForm
				{
					char pad118[0x8];
					BGSSnapTemplateComponent _SnapTemplate;
					TESCellUseList _CellUseList;
				public:
				};
				static_assert(sizeof(TESObject) == 0x1A8);
			}
		}
	}
}