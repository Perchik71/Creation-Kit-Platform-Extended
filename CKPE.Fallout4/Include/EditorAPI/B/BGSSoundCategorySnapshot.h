// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
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
				class BGSSoundCategory;

				// 1.11.240.0: size 0xE8, vtable RVA 0x2FBA5A0, 124 virtual functions
				class BGSSoundCategorySnapshot :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSoundCategorySnapshot() = default;

					constexpr static std::uint8_t TYPE_ID = ftSoundCategorySnapshot;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSoundCategorySnapshot[0];
				public:
					BSTHashMap<BGSSoundCategory*, float> categoryMult; // 28 - CNAM
					std::int8_t priority; // 60
					std::uint8_t unk61[0x7]; // 61
					std::uint8_t unk68[0x80]; // 68
				};
				static_assert(offsetof(BGSSoundCategorySnapshot, categoryMult) == 0x28);
				static_assert(offsetof(BGSSoundCategorySnapshot, priority) == 0x60);
				static_assert(sizeof(BGSSoundCategorySnapshot) == 0xE8);
			}
		}
	}
}
