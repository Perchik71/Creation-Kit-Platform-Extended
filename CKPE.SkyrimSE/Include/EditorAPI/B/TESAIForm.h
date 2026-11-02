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
			// 1.7.99.0: size 0x30, vtable RVA 0x3079980. Bytes @08-@0E are set on construction (@09 = 4, @0B = 3); an object at @20 is built by a helper constructor.
			class TESAIForm : public BaseFormComponent
			{
			public:
				virtual ~TESAIForm() = default;

				std::uint8_t unk08[8];		// 08
				std::uint8_t unk10[0x10];	// 10
				void* unk20[2];				// 20
			};
			static_assert(sizeof(TESAIForm) == 0x30);
		}
	}
}