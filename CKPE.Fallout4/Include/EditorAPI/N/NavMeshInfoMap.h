// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/B/BSReadWriteLock.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x128, vtable RVA 0x3100FE0, 124 virtual functions
				class NavMeshInfoMap :
					public TESForm,           // 000
					public BSNavmeshInfoMap   // 028
				{
				public:
					virtual ~NavMeshInfoMap() = default;

				public:
					std::uint8_t unk38[0x8]; // 38
					BSTArray<void*> allPaths; // 40 - BSTArray<BSTArray<BSNavmeshInfo const*>*>, by layout (fills 0x38..0x88 exactly)
					std::uint8_t infoToIndexMap[0x30]; // 58 - BSTScatterTable<BSNavmeshInfo const*, ...>, by layout (part of the 0x38 byte PrecomputedNavmeshInfoPathMap base)
					bool updateAll; // 88
					std::uint8_t unk89[0x7]; // 89
					BSTArray<void*> staleNavmeshInfos; // 90 - BSTArray<BSNavmeshInfo*>, by position after updateAll
					// Two hash maps in the CK's 0x38 byte table layout (end of chain markers at C0 and F8, entries at D8 and 110), CommonLibF4's
					// infoMap and ckNavMeshInfoMap; the key/value types are CommonLibF4's.
					BSTHashMap<std::uint32_t, void*> infoMap; // A8 - NavMeshInfo*
					BSTHashMap<std::size_t, BSTArray<void*>*> ckNavMeshInfoMap; // E0 - values are BSTArray<BSNavmeshInfo*>*
					BSReadWriteLock mapLock; // 118
					bool init; // 120 - NVMI
					std::uint8_t unk121[0x7]; // 121
				};
				static_assert(sizeof(NavMeshInfoMap) == 0x128);
			}
		}
	}
}
