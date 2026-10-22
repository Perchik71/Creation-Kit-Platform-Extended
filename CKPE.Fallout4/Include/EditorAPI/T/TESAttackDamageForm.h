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
			// 1.11.240.0: size 0x10, vtable RVA 0x301C448, 21 virtual functions
			class TESAttackDamageForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESAttackDamageForm() = default;

			public:
				std::uint16_t attackDamage; // 08 - edited through dialog item 1015
				std::uint8_t pad0A[0x6]; // 0A
			};
			static_assert(sizeof(TESAttackDamageForm) == 0x10);
		}
	}
}
