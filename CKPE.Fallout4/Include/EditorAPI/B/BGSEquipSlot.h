// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class ActorValueInfo;

				// 1.11.240.0: size 0x50, vtable RVA 0x3035D48, 124 virtual functions
				class BGSEquipSlot :
					public TESForm   // 000
				{
				public:
					virtual ~BGSEquipSlot() = default;

					constexpr static std::uint8_t TYPE_ID = ftEquipSlot;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSEquipSlot[0];
				public:
					BSTArray<void*> parentSlots; // 28 - BSTArray<BGSEquipSlot*>
					std::uint32_t flags; // 40
					std::uint32_t unk44; // 44
					ActorValueInfo* conditionActorValue; // 48
				};
				static_assert(sizeof(BGSEquipSlot) == 0x50);
			}
		}
	}
}
