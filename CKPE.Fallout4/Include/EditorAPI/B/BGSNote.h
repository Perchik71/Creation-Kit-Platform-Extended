// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x168, vtable RVA 0x3039EF8, 154 virtual functions
				class BGSNote :
					public TESBoundObject,          // 000
					public TESWeightForm,           // 090
					public TESValueForm,            // 0A0
					public TESModel,                // 0B0
					public TESFullName,             // 0E8
					public TESIcon,                 // 100
					public BGSPickupPutdownSounds   // 128
				{
				public:
					virtual ~BGSNote() = default;

				public:
					BSFixedString programFile; // 140
					std::uint64_t unk148; // 148
					std::int8_t type; // 150
					bool hasBeenRead; // 151
					std::uint8_t unk152[0xE]; // 152
					std::uint16_t unk160; // 160
					std::uint8_t unk162[0x6]; // 162
				};
				static_assert(sizeof(BGSNote) == 0x168);
			}
		}
	}
}
