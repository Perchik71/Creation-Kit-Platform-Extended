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
			// 1.7.99.0: size 0x28, vtable RVA 0x3081328. Byte @18 is set to 11 on construction. The size is taken from the first TESFaction member at 0x68 (no other class embeds it); the layout is not resolved.
			class TESReactionForm : public BaseFormComponent
			{
			public:
				virtual ~TESReactionForm() = default;

				std::uint8_t unk08[0x10];	// 08 - helper-constructed object
				std::uint8_t unk18;			// 18 - 11 on construction
				std::uint8_t pad19[7];		// 19
				std::uint8_t unk20[8];		// 20
			};
			static_assert(sizeof(TESReactionForm) == 0x28);
		}
	}
}