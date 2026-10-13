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
				// 1.11.240.0: size 0x40, vtable RVA 0x3082BF8, 124 virtual functions
				class BGSOutfit :
					public TESForm   // 000
				{
				public:
					virtual ~BGSOutfit() = default;

				public:
					BSTArray<void*> outfitItems; // 28 - BSTArray<TESForm*>
				};
				static_assert(sizeof(BGSOutfit) == 0x40);
			}
		}
	}
}
