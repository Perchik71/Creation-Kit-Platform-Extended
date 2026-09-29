// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.EnumSet.h>
#include <EditorAPI/Forms/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSEquipSlot : public TESForm
				{
				public:
					enum class Flag
					{
						kNone = 0,
						kUseAllParents = 1 << 0,
						kParentsOptional = 1 << 1,
						kItemSlot = 1 << 2
					};
				private:
					// members
					BSTArray<BGSEquipSlot*>					parentSlots;
					TEnumSet<Flag, std::uint32_t>			flags;
				public:
					virtual ~BGSEquipSlot() = default;
				};
				static_assert(sizeof(BGSEquipSlot) == 0x48);
			}
		}
	}
}

