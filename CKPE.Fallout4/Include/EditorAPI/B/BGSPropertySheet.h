// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSTypedFormValuePair.h>
#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x10, vtable RVA 0x2FA9870, 21 virtual functions
			class BGSPropertySheet :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSPropertySheet() = default;

			public:
				BGSTypedFormValuePair::Array* properties; // 08 - heap allocated 24 byte array
			};
			static_assert(sizeof(BGSPropertySheet) == 0x10);
		}
	}
}
