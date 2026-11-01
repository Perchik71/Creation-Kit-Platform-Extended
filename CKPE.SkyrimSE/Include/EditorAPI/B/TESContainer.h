// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x18, vtable RVA 0x307A370. Entry array @08, count @10 and a byte @14 are zeroed on construction (entry type not resolved).
			class TESContainer : public BaseFormComponent
			{
			public:
				virtual ~TESContainer() = default;

				void* unk08;			// 08
				std::uint32_t unk10;		// 10
				std::uint8_t unk14;		// 14
				std::uint8_t pad15[3];	// 15
			};
			static_assert(offsetof(TESContainer, unk10) == 0x10);
			static_assert(sizeof(TESContainer) == 0x18);
		}
	}
}