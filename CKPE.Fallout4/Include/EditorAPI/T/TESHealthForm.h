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
			// 1.11.240.0: size 0x10, vtable RVA 0x3020260, 21 virtual functions
			class TESHealthForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESHealthForm() = default;

			public:
				std::uint32_t health; // 08 - edited through dialog item 1024
				std::uint32_t pad0C; // 0C
			};
			static_assert(sizeof(TESHealthForm) == 0x10);
		}
	}
}
