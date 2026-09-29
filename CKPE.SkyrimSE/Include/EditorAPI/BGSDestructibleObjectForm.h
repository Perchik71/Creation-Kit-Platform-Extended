// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BGSDestructibleObjectForm : public BaseFormComponent
			{
				// members
				void* data;
			public:
				virtual ~BGSDestructibleObjectForm() = default;
			};
			static_assert(sizeof(BGSDestructibleObjectForm) == 0x10);
		}
	}
}