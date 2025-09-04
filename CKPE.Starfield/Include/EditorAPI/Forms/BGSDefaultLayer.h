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
				class BGSDefaultLayer : public BaseFormComponent
				{
					char pad40[0x8];
					std::uint32_t unk_id;
				public:
				};
				static_assert(sizeof(BGSDefaultLayer) == 0x50);
			}
		}
	}
}