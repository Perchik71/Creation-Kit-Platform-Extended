// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSDestructibleObjectForm : public BaseFormComponent
				{
					char pad40_1[0x8];
				public:
				};
				static_assert(sizeof(BGSDestructibleObjectForm) == 0x48);
			}
		}
	}
}