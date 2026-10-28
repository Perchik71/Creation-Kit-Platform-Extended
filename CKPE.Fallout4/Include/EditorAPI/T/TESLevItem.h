// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESLeveledList.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xC8, vtable RVA 0x308E308, 154 virtual functions
				class TESLevItem :
					public TESBoundObject,  // 000
					public TESLeveledList   // 090
				{
				public:
					virtual ~TESLevItem() = default;

					constexpr static std::uint8_t TYPE_ID = ftLeveledItem;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESLevItem[0];
				public:
					BGSLocalizedString unkB8; // B8
				};
				static_assert(sizeof(TESLevItem) == 0xC8);
			}
		}
	}
}
