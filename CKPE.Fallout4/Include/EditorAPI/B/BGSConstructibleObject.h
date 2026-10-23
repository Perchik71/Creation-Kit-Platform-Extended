// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSTypedFormValuePair.h>
#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESDescription.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct ConstructibleObjectData
				{
					std::uint16_t numConstructed; // 00
					std::uint16_t workshopPriority; // 02
				};
				static_assert(sizeof(ConstructibleObjectData) == 0x4);

				// 1.11.240.0: size 0xB0, vtable RVA 0x307EED8, 124 virtual functions
				class BGSConstructibleObject :
					public TESForm,                 // 000
					public BGSPickupPutdownSounds,  // 028
					public TESDescription           // 040
				{
				public:
					virtual ~BGSConstructibleObject() = default;

				public:
					BGSTypedFormValuePair::Array* requiredItems; // 68
					std::uint8_t unk70[0x18]; // 70
					std::uint64_t unk88; // 88
					std::uint64_t unk90; // 90
					ConstructibleObjectData data; // 98
					std::uint32_t unk9C; // 9C
					std::uint64_t unkA0; // A0
					std::uint32_t unkA8; // A8
					std::uint32_t unkAC; // AC
				};
				static_assert(sizeof(BGSConstructibleObject) == 0xB0);
			}
		}
	}
}
