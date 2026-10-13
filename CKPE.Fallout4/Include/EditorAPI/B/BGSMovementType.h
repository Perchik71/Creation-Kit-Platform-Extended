// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xA8, vtable RVA 0x30395F8, 124 virtual functions
				class BGSMovementType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSMovementType() = default;

				public:
					BSFixedString unk28; // 28
					std::uint8_t unk30[0x70]; // 30
					float floatHeight; // A0
					std::uint32_t unkA4; // A4
				};
				static_assert(sizeof(BGSMovementType) == 0xA8);
			}
		}
	}
}
