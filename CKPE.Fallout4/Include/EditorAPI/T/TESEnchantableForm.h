// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class EnchantmentItem;
			}

			// 1.11.240.0: size 0x18, vtable RVA 0x301D390, 22 virtual functions
			class TESEnchantableForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESEnchantableForm() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESEnchantableForm[0];

				virtual std::uint16_t GetCastingType() const noexcept(true) { return 0; } // 21 - RVA 0x8C03F0, return *(unsigned __int16 *)(a1 + 16);
			public:
				Forms::EnchantmentItem* formEnchanting; // 08
				std::uint16_t castingType; // 10 - MagicSystem::CastingType
				std::uint16_t amountOfEnchantment; // 12
				std::uint32_t pad14; // 14
			};
			static_assert(sizeof(TESEnchantableForm) == 0x18);
		}
	}
}
