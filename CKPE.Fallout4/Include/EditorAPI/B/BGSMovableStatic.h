// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectSTAT.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				struct MOVABLE_STATIC_DATA
				{
					std::int8_t flags; // 00
				};
				static_assert(sizeof(MOVABLE_STATIC_DATA) == 0x1);

				// 1.11.240.0: size 0x198, vtable RVA 0x3081EB0, 154 virtual functions
				class BGSMovableStatic :
					public TESObjectSTAT,              // 000
					public BGSDestructibleObjectForm,  // 148
					public BGSKeywordForm              // 158
				{
				public:
					virtual ~BGSMovableStatic() = default;

				public:
					BGSSoundDescriptorForm* soundLoop; // 178
					MOVABLE_STATIC_DATA data; // 180
					std::uint8_t unk181[0x7]; // 181
					std::uint8_t unk188[0x10]; // 188
				};
				static_assert(sizeof(BGSMovableStatic) == 0x198);
			}
		}
	}
}
