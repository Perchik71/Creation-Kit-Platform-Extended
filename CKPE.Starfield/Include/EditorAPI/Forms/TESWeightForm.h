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
				class TESWeightForm : public BaseFormComponent
				{
				public:
					std::int32_t Weight;
				};
				static_assert(sizeof(TESWeightForm) == 0x48);
			}
		}
	}
}