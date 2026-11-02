// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

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
				constexpr static Common::ID RTTI = IDs::RTTI::BGSPickupPutdownSounds;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPickupPutdownSounds[0];

				virtual ~BGSPickupPutdownSounds() = default;

				void* unk08;	// 08
				void* unk10;	// 10
			};
			static_assert(sizeof(BGSPickupPutdownSounds) == 0x18);
		}
	}
}