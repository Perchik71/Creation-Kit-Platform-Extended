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
				// 1.11.240.0: size 0x80, vtable RVA 0x3083310, 124 virtual functions
				class BGSPackIn :
					public TESForm   // 000
				{
				public:
					virtual ~BGSPackIn() = default;

					constexpr static std::uint8_t TYPE_ID = ftPackIn;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPackIn[0];
				public:
					std::uint64_t unk28; // 28
					std::uint64_t unk30; // 30
					std::uint32_t unk38; // 38
					std::uint32_t unk3C; // 3C
					std::uint32_t unk40; // 40
					std::uint8_t unk44[0x10]; // 44
					std::uint8_t unk54[0x8]; // 54
					std::uint32_t unk5C; // 5C
					std::uint64_t unk60; // 60
					std::uint32_t unk68; // 68
					std::uint8_t unk6C[0xC]; // 6C
					std::uint64_t unk78; // 78
				};
				static_assert(sizeof(BGSPackIn) == 0x80);
			}
		}
	}
}
