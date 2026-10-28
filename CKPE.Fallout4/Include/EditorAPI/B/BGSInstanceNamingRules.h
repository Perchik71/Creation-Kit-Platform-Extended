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
				// 1.11.240.0: size 0x128, vtable RVA 0x2FB9E60, 124 virtual functions
				class BGSInstanceNamingRules :
					public TESForm   // 000
				{
				public:
					virtual ~BGSInstanceNamingRules() = default;

					constexpr static std::uint8_t TYPE_ID = ftInstanceNamingRules;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSInstanceNamingRules[0];
				public:
					std::uint32_t unk28; // 28
					std::uint8_t unk2C[0xF4]; // 2C
					std::uint32_t unk120; // 120
					std::uint32_t unk124; // 124
				};
				static_assert(sizeof(BGSInstanceNamingRules) == 0x128);
			}
		}
	}
}
