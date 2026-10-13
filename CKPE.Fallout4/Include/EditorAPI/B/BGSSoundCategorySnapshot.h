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
				// 1.11.240.0: size 0xE8, vtable RVA 0x2FBA5A0, 124 virtual functions
				class BGSSoundCategorySnapshot :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSoundCategorySnapshot() = default;

				public:
					std::uint8_t unk28[0xC]; // 28
					std::uint8_t unk34[0x8]; // 34
					std::uint32_t unk3C; // 3C
					std::uint64_t unk40; // 40
					std::uint32_t unk48; // 48
					std::uint8_t unk4C[0xC]; // 4C
					std::uint64_t unk58; // 58
					std::int8_t priority; // 60
					std::uint8_t unk61[0x7]; // 61
					std::uint8_t unk68[0x80]; // 68
				};
				static_assert(sizeof(BGSSoundCategorySnapshot) == 0xE8);
			}
		}
	}
}
