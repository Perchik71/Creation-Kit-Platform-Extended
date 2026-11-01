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
			// 1.7.99.0: size 0x18, vtable RVA 0x3078700. Pointer @08 is zeroed and a 32 bit value @10 is set to 1087 on construction.
			class BGSSkinForm : public BaseFormComponent
			{
			public:
				virtual ~BGSSkinForm() = default;

				void* unk08;			// 08
				std::uint32_t unk10;		// 10
				std::uint32_t pad14;	// 14
			};
			static_assert(sizeof(BGSSkinForm) == 0x18);
		}
	}
}