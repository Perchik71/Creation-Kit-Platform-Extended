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
			// 1.11.240.0: size 0x10, vtable RVA 0x3018898, 22 virtual functions
			class BGSBipedObjectForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSBipedObjectForm() = default;

				virtual bool FillsBipedSlot(std::int32_t nBipedSlot) const noexcept(true) { return false; } // 21 - RVA 0x88BA90, if ( a2 >= 32 ); return 0;; v3 = *(_DWORD *)(a1 + 8);; return _bittest(&v3, a2);

			public:
				std::uint32_t bipedModelData; // 08 - BIPED_MODEL, biped slot mask
				std::uint32_t unk0C; // 0C
			};
			static_assert(sizeof(BGSBipedObjectForm) == 0x10);
		}
	}
}
