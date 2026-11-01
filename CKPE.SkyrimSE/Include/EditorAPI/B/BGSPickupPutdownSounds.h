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
			// 1.7.99.0: size 0x18, vtable RVA 0x3078480. Both members are zeroed on construction; their form types are not resolved.
			class BGSPickupPutdownSounds : public BaseFormComponent
			{
			public:
				virtual ~BGSPickupPutdownSounds() = default;

				void* unk08;	// 08
				void* unk10;	// 10
			};
			static_assert(sizeof(BGSPickupPutdownSounds) == 0x18);
		}
	}
}