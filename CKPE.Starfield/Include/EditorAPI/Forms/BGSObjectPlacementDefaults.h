// Copyright © 2023-2025 aka perchik71. All rights reserved.
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
				class BGSObjectPlacementDefaults : public BaseFormComponent
				{
					char pad48_1[0x18];
				public:
				};
				static_assert(sizeof(BGSObjectPlacementDefaults) == 0x58);
			}
		}
	}
}