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
				class BGSEquipSlot;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x3019480, 23 virtual functions
			class BGSEquipType :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSEquipType() = default;

				virtual Forms::BGSEquipSlot* GetEquipSlot(const void* pInstanceData) const noexcept(true) { return nullptr; } // 21 - RVA 0x893670, return (const char *)*((_QWORD *)this + 1);
				virtual void SetEquipSlot(Forms::BGSEquipSlot* pSlot) noexcept(true) { return; } // 22 - RVA 0x893680, *(_QWORD *)(a1 + 8) = a2;

			public:
				Forms::BGSEquipSlot* equipSlot; // 08
			};
			static_assert(sizeof(BGSEquipType) == 0x10);
		}
	}
}
