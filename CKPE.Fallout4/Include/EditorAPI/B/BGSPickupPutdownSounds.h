// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;
			}

			// 1.11.240.0: size 0x18, vtable RVA 0x301B3B0, 21 virtual functions
			class BGSPickupPutdownSounds :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSPickupPutdownSounds() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPickupPutdownSounds[0];
			public:
				Forms::BGSSoundDescriptorForm* pickupSound; // 08
				Forms::BGSSoundDescriptorForm* putdownSound; // 10
			};
			static_assert(sizeof(BGSPickupPutdownSounds) == 0x18);
		}
	}
}
