// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFormIDListView.h>
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
				// 1.11.240.0: size 0x98, vtable RVA 0x3032230, 126 virtual functions
				class BGSDialogueView :
					public TESFormIDListView   // 000
				{
				public:
					virtual ~BGSDialogueView() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSDialogueView[0];
				public:
					BSTArray<void*> unk28; // 28 - element type not identified yet
					BSTArray<void*> unk40; // 40 - element type not identified yet
					std::uint32_t unk58; // 58
					std::uint32_t unk5C; // 5C
					std::uint64_t unk60; // 60
					std::uint64_t unk68; // 68
					std::uint32_t unk70; // 70
					std::uint32_t unk74; // 74
					std::uint16_t unk78; // 78
					std::uint8_t unk7A[0x6]; // 7A
					std::uint64_t unk80; // 80
					std::uint64_t unk88; // 88
					std::uint64_t unk90; // 90
				};
				static_assert(sizeof(BGSDialogueView) == 0x98);
			}
		}
	}
}
