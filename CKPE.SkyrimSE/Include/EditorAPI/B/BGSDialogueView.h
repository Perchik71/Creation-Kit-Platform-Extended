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
					std::uint8_t pad28[0x30];	// 28
					std::uint32_t enam;	// 58 - ENAM
					std::uint8_t pad5C[0x4];	// 5C
					void* qnam;	// 60 - QNAM
					std::uint8_t pad68[0x10];	// 68
					std::uint8_t dnam;	// 78 - DNAM
					std::uint8_t pad79[0x1F];	// 79
				};
				static_assert(offsetof(BGSDialogueView, pad28) == 0x28);
				static_assert(offsetof(BGSDialogueView, enam) == 0x58);
				static_assert(offsetof(BGSDialogueView, qnam) == 0x60);
				static_assert(offsetof(BGSDialogueView, dnam) == 0x78);
				static_assert(sizeof(BGSDialogueView) == 0x98);
			}
		}
	}
}