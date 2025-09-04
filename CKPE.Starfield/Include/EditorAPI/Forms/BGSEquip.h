// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSEquipSlot : public BaseFormComponent
				{
				public:
				};
				static_assert(sizeof(BGSEquipSlot) == 0x40);

				class BGSEquipType: public BaseFormComponent
				{
				public:
					[[nodiscard]] inline BGSEquipSlot* GetEquipSlot(const void* unk) const;
					inline void SetEquipSlot(BGSEquipSlot* slot);
				private:
					BGSEquipSlot* EquipSlot;
				};
				static_assert(sizeof(BGSEquipType) == 0x48);
			}
		}
	}
}