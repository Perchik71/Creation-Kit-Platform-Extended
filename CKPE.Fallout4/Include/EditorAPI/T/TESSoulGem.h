// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectMISC.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x1F8, vtable RVA 0x30A19B8, 156 virtual functions
				class TESSoulGem :
					public TESObjectMISC   // 000
				{
				public:
					virtual ~TESSoulGem() = default;

					constexpr static std::uint8_t TYPE_ID = ftSoulGem;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESSoulGem[0];
				public:
					TESSoulGem* linkedSoulGem; // 1E8
					std::int8_t currentSoul; // 1F0
					std::int8_t soulCapacity; // 1F1
					std::uint8_t unk1F2[0x6]; // 1F2
				};
				static_assert(sizeof(TESSoulGem) == 0x1F8);
			}
		}
	}
}
