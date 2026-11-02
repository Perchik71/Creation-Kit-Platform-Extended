// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x8, vtable RVA 0x3095FF8. Holds no data.
			class BGSPreloadable : public BaseFormComponent
			{
			public:
				virtual ~BGSPreloadable() = default;


			};
			static_assert(sizeof(BGSPreloadable) == 0x8);
		}
	}
}