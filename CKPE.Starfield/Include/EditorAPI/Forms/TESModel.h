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
				class TESModel : public BaseFormComponent
				{
				public:
					BSFixedString Model;  // 08
				private:
					char pad48_5[0x18];
				};
				static_assert(sizeof(TESModel) == 0x60);
			}
		}
	}
}