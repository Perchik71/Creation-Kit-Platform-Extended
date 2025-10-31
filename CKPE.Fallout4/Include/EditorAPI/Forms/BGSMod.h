// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESFullName.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				namespace BGSMod
				{
					class Container
					{
					private:
						char pad0[0x8];
					};
					static_assert(sizeof(Container) == 0x8);

					namespace Template
					{
						class Items : public TESFullName, public Container
						{
						private:
						};
						static_assert(sizeof(Items) == 0x20);
					}
				}
			}
		}
	}
}
