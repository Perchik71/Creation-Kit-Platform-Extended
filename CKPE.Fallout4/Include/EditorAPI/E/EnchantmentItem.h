// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/M/MagicItem.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x300, vtable RVA 0x30BF688, 188 virtual functions
				class EnchantmentItem :
					public MagicItem   // 000
				{
				public:
					virtual ~EnchantmentItem() = default;

				public:
					std::uint32_t unk2D0; // 2D0
					std::uint32_t unk2D4; // 2D4
					std::uint32_t unk2D8; // 2D8
					std::uint32_t unk2DC; // 2DC
					std::uint32_t unk2E0; // 2E0
					std::uint32_t unk2E4; // 2E4
					std::uint32_t unk2E8; // 2E8
					std::uint32_t unk2EC; // 2EC
					std::uint64_t unk2F0; // 2F0
					std::uint64_t unk2F8; // 2F8
				};
				static_assert(sizeof(EnchantmentItem) == 0x300);
			}
		}
	}
}
