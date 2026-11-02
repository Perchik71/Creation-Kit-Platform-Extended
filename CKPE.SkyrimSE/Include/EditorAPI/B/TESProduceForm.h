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
			// 1.7.99.0: size 0x20, vtable RVA 0x3080988. Two pointers @08/@10 are zeroed on construction; the remainder is set by a helper (not resolved).
			class TESProduceForm : public BaseFormComponent
			{
			public:
				virtual ~TESProduceForm() = default;

				void* unk08;	// 08
				void* unk10;	// 10
				void* unk18;	// 18
			};
			static_assert(sizeof(TESProduceForm) == 0x20);
		}
	}
}