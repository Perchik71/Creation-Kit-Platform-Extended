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
				// 1.11.240.0: size 0x50, vtable RVA 0x302F698, 124 virtual functions
				class BGSAssociationType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSAssociationType() = default;

				public:
					BSFixedString associationLabel[2][2]; // 28 - BSFixedStringCS
					std::uint32_t flags; // 48 - REX::TEnumSet<Flags, std::uint32_t>
					std::uint32_t unk4C; // 4C
				};
				static_assert(sizeof(BGSAssociationType) == 0x50);
			}
		}
	}
}
