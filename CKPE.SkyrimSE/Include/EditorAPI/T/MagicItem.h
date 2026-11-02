// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x268, vtable RVA 0x3129780. Base of spells, potions, ingredients, scrolls and enchantments.
				class MagicItem :
					public TESBoundObject,		// 00
					public TESFullName,			// 50
					public BGSKeywordForm		// 68
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::MagicItem;
					constexpr static Common::ID VTABLE = IDs::VTABLE::MagicItem[0];

					virtual ~MagicItem() = default;
				public:
					std::uint8_t efid[0x1E8];	// 80 - EFID
				};
				static_assert(offsetof(MagicItem, efid) == 0x80);
				static_assert(sizeof(MagicItem) == 0x268);
			}
		}
	}
}