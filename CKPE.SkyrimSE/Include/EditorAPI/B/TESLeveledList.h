// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x20, vtable RVA 0x307E330. Entry array @08, bytes @10/@11/@12 and a pointer @18 are zeroed on construction (roles not resolved).
			class TESLeveledList : public BaseFormComponent
			{
			public:
				virtual ~TESLeveledList() = default;

				void* unk08;			// 08
				std::uint8_t unk10;		// 10
				std::uint8_t unk11;		// 11
				std::uint8_t unk12;		// 12
				std::uint8_t pad13[5];	// 13
				void* unk18;			// 18
			};
			static_assert(offsetof(TESLeveledList, unk18) == 0x18);
			static_assert(sizeof(TESLeveledList) == 0x20);
		}
	}
}