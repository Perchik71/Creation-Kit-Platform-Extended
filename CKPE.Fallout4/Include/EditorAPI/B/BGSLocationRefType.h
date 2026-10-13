// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSKeyword.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x60, vtable RVA 0x30A5538, 124 virtual functions
				class BGSLocationRefType :
					public BGSKeyword   // 000
				{
				public:
					virtual ~BGSLocationRefType() = default;
				};
				static_assert(sizeof(BGSLocationRefType) == 0x60);
			}
		}
	}
}
