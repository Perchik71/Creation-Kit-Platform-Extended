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
			// 1.7.99.0: size 0x10, vtable RVA 0x3075980. A 32 bit value @08 is zeroed on construction (slot data; layout not resolved).
			class BGSBipedObjectForm : public BaseFormComponent
			{
			public:
				virtual ~BGSBipedObjectForm() = default;

				std::uint32_t unk08;		// 08
				std::uint32_t pad0C;	// 0C
			};
			static_assert(sizeof(BGSBipedObjectForm) == 0x10);
		}
	}
}