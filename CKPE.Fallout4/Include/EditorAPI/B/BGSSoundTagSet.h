// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				// 1.11.240.0: size 0xE0, vtable RVA 0x2FBBA38, 124 virtual functions
				class BGSSoundTagSet :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSoundTagSet() = default;

				public:
					BSTHashMap<BSFixedString, BGSSoundDescriptorForm*> soundTags; // 28 - TNAM
					std::uint8_t unk60[0x80]; // 60
				};
				static_assert(offsetof(BGSSoundTagSet, soundTags) == 0x28);
				static_assert(sizeof(BGSSoundTagSet) == 0xE0);
			}
		}
	}
}
