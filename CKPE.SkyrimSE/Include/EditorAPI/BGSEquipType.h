// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BaseFormComponent.h>
#include <EditorAPI/Forms/BGSEquipSlot.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BGSEquipType: public BaseFormComponent
			{
				// members
				Forms::BGSEquipSlot* data;
			public:
				enum class EQUIPPED_ITEM_TYPE
				{
					kSpell = 24,
					kShield = 25,
					kTorch = 26,

					kTotal
				};

				virtual ~BGSEquipType() = default;

				// add
				[[nodiscard]] inline virtual Forms::BGSEquipSlot*	GetEquipSlot() const noexcept(true) { return data; }
				inline virtual void									SetEquipSlot(Forms::BGSEquipSlot* a_slot) noexcept(true) { data = a_slot; }

				CKPE_PROPERTY(GetEquipSlot, SetEquipSlot) Forms::BGSEquipSlot* EquipSlot;
			};
			static_assert(sizeof(BGSEquipType) == 0x10);
		}
	}
}