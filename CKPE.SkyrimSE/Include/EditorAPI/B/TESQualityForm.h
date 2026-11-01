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
			// 1.7.99.0: size 0x10, vtable RVA 0x3080CB8. A 32 bit value @08 is zeroed on construction (quality; layout not resolved).
			class TESQualityForm : public BaseFormComponent
			{
			public:
				virtual ~TESQualityForm() = default;

				std::uint32_t unk08;	// 08
				std::uint32_t pad0C;	// 0C
			};
			static_assert(sizeof(TESQualityForm) == 0x10);
		}
	}
}