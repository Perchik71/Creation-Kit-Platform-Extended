// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x110, vtable RVA 0x3084F98, 154 virtual functions
				class BGSStaticCollection :
					public TESBoundObject,        // 000
					public TESFullName,           // 090
					public BGSModelMaterialSwap   // 0A8
				{
				public:
					virtual ~BGSStaticCollection() = default;

				public:
					BSTHashMap38<TESBoundObject*, void*>* dataMap; // F8 - table layout assumed to be the CK 0x38 byte one like every other CK table; value is BSSimpleList<BGSStaticCollection::REF_COL_DATA>*
					std::uint64_t unk100; // 100
					std::uint32_t unk108; // 108
					std::uint32_t unk10C; // 10C
				};
				static_assert(sizeof(BGSStaticCollection) == 0x110);
			}
		}
	}
}
