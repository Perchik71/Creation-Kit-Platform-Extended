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
			// 1.7.99.0: size 0x28, vtable RVA 0x307A8F0 (DESC). Members other than the construction defaults are not resolved:
			// the text object at 10 is built by a helper constructor.
			class TESDescription : public BaseFormComponent
			{
			public:
				virtual ~TESDescription() = default;

				std::uint32_t unk08;		// 08 - 1138 on construction
				std::uint32_t pad0C;		// 0C
				void* unk10[2];				// 10 - helper-constructed 16 byte text object
				void* unk20;				// 20
			};
			static_assert(offsetof(TESDescription, unk08) == 0x08);
			static_assert(sizeof(TESDescription) == 0x28);
		}
	}
}