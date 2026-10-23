// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class EffectSetting;

				// 1.11.246.0: size 0x0C (EFIT chunk)
				struct EffectItemData
				{
					float magnitude; // 00
					std::int32_t area; // 04
					std::int32_t duration; // 08
				};
				static_assert(sizeof(EffectItemData) == 0xC);

				// 1.11.246.0: size 0x50, allocated by MagicItem's EFID loader (constructor 0x140E694A0). CommonLibF4's EffectItem is 0x28 bytes:
				// the CK object is bigger because its condition list is 0x18 bytes and it ends with three editor-only members.
				class EffectItem
				{
				public:
					EffectItemData data; // 00 - EFIT
					std::uint32_t pad0C; // 0C
					EffectSetting* effectSetting; // 10 - EFID
					float rawCost; // 18
					std::uint32_t pad1C; // 1C
					std::uint8_t conditions[0x18]; // 20 - TESCondition (0x18 bytes in the CK), CTDA chunks
					std::uint64_t unk38; // 38 - 0 on construction
					std::uint64_t unk40; // 40 - 0 on construction
					std::uint8_t unk48; // 48 - 0 on construction
					std::uint8_t pad49[7]; // 49
				};
				static_assert(offsetof(EffectItem, effectSetting) == 0x10);
				static_assert(offsetof(EffectItem, conditions) == 0x20);
				static_assert(sizeof(EffectItem) == 0x50);
			}
		}
	}
}
