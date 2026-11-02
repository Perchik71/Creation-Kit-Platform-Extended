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
			// 1.7.99.0: size 0x18, vtable RVA 0x307ABF0. Pointer @08 and a 16 bit field @12 are zeroed on construction (roles not resolved).
			class TESEnchantableForm : public BaseFormComponent
			{
			public:
				virtual ~TESEnchantableForm() = default;

				void* unk08;			// 08
				std::uint8_t unk10[0x8];	// 10
			};
			static_assert(sizeof(TESEnchantableForm) == 0x18);
		}
	}
}