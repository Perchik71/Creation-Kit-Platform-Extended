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
			class DestructibleObjectData;

			// 1.11.240.0: size 0x10, vtable RVA 0x3018D38, 21 virtual functions
			class BGSDestructibleObjectForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSDestructibleObjectForm() = default;

			public:
				DestructibleObjectData* data; // 08
			};
			static_assert(sizeof(BGSDestructibleObjectForm) == 0x10);
		}
	}
}
