// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x50, vtable RVA 0x30790F0. Level (1) @10 and speed multiplier (100) @16 are verified from the constructor defaults; the other fields are zeroed and their roles are not resolved.
			class TESActorBaseData : public BaseFormComponent
			{
			public:
				virtual ~TESActorBaseData() = default;

				std::uint32_t unk08;		// 08 - 0 on construction
				std::uint16_t unk0C;		// 0C - 0 on construction
				std::uint16_t unk0E;		// 0E - 0 on construction
				std::uint16_t level;		// 10 - 1 on construction
				std::uint16_t unk12;		// 12 - 0 on construction
				std::uint16_t unk14;		// 14 - 0 on construction
				std::uint16_t speedMult;	// 16 - 100 on construction
				std::uint16_t unk18;		// 18
				std::uint16_t unk1A;		// 1A - 0 on construction
				std::uint16_t unk1C;		// 1C - 0 on construction
				std::uint16_t unk1E;		// 1E - 0 on construction
				void* unk20;				// 20 - 0 on construction
				void* unk28;				// 28 - 0 on construction
				void* unk30;				// 30 - 0 on construction
				std::uint8_t unk38[0x18];	// 38
			};
			static_assert(offsetof(TESActorBaseData, level) == 0x10);
			static_assert(offsetof(TESActorBaseData, speedMult) == 0x16);
			static_assert(sizeof(TESActorBaseData) == 0x50);
		}
	}
}