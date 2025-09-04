// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BGSSnapBehavior.h"
#include "BGSSnapTemplate.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSPropertySheet : public BaseFormComponent
				{
					char pad40_1[0x8];
				public:
				};
				static_assert(sizeof(BGSPropertySheet) == 0x48);
			}
		}
	}
}