// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESFormIDListView.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x98, vtable RVA 0x3097B60
				class BGSDialogueView :
					public TESFormIDListView	// 00
				{
				public:
					virtual ~BGSDialogueView() = default;
				public:
					std::uint8_t unk28[0x70];	// 28 - data, layout not resolved
				};
				static_assert(offsetof(BGSDialogueView, unk28) == 0x28);
				static_assert(sizeof(BGSDialogueView) == 0x98);
			}
		}
	}
}