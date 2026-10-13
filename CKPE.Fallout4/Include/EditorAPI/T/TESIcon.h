// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x28, vtable RVA 0x30203A8, 27 virtual functions
			class TESIcon :
				public TESTexture   // 000
			{
			public:
				virtual ~TESIcon() = default;
			};
			static_assert(sizeof(TESIcon) == 0x28);
		}
	}
}
