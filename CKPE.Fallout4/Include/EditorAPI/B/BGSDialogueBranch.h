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
				class TESQuest;
				class TESTopic;

				// 1.11.240.0: size 0x68, vtable RVA 0x3031BB0, 124 virtual functions
				class BGSDialogueBranch :
					public TESForm   // 000
				{
				public:
					virtual ~BGSDialogueBranch() = default;

				public:
					std::uint32_t flags; // 28
					std::uint32_t unk2C; // 2C
					TESQuest* quest; // 30
					TESTopic* startingTopic; // 38
					std::int32_t type; // 40 - REX::TEnumSet<DIALOGUE_TYPE, std::int32_t>
					std::uint8_t unk44[0x4]; // 44
					std::uint32_t unk48; // 48
					std::uint32_t unk4C; // 4C
					BSTArray<void*> unk50; // 50 - element type not identified yet
				};
				static_assert(sizeof(BGSDialogueBranch) == 0x68);
			}
		}
	}
}
