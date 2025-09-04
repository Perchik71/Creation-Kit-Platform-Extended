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
				class TESValueForm : public BaseFormComponent
				{
				public:
					std::int32_t Value;
				};
				static_assert(sizeof(TESValueForm) == 0x48);
			}
		}
	}
}