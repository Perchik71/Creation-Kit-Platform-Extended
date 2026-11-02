// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/MagicItem.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x298, vtable RVA 0x3127C20
				class EnchantmentItem :
					public MagicItem	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftEnchantment;

					virtual ~EnchantmentItem() = default;
				public:
					std::uint8_t unk268[0x30];	// 268 - data, layout not resolved
				};
				static_assert(offsetof(EnchantmentItem, unk268) == 0x268);
				static_assert(sizeof(EnchantmentItem) == 0x298);
			}
		}
	}
}