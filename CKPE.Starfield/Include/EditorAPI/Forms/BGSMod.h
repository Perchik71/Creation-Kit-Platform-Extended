// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"
#include "../BSFixedString.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				namespace BGSMod
				{
					namespace Template
					{
						class Items : public BaseFormComponent
						{
							char pad[0x10];
							BSFixedString _empty_str;	// ??
						public:
						};
						static_assert(sizeof(Items) == 0x58);
					}
				}
			}
		}
	}
}