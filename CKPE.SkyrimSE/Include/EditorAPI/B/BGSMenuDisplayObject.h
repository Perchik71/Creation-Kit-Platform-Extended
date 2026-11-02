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
			// 1.7.99.0: size 0x10, vtable RVA 0x3076E90. Object pointer @08, zeroed on construction (form type not resolved).
			class BGSMenuDisplayObject : public BaseFormComponent
			{
			public:
				virtual ~BGSMenuDisplayObject() = default;

				void* unk08;	// 08
			};
			static_assert(sizeof(BGSMenuDisplayObject) == 0x10);
		}
	}
}