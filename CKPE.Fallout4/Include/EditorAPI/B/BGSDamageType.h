// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class ActorValueInfo;

				class MagicItem;

				// 1.11.240.0: size 0x38, vtable RVA 0x2FB91D0, 124 virtual functions
				class BGSDamageType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSDamageType() = default;

				public:
					ActorValueInfo* resistance; // 28
					MagicItem* spell; // 30
				};
				static_assert(sizeof(BGSDamageType) == 0x38);
			}
		}
	}
}
