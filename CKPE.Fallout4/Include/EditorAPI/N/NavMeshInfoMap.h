// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
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
					std::uint8_t infoToIndexMap[0x30]; // 58 - BSTScatterTable<BSNavmeshInfo const*, ...>, by layout
					bool updateAll; // 88
					std::uint8_t unk89[0x7]; // 89
					BSTArray<void*> staleNavmeshInfos; // 90 - BSTArray<BSNavmeshInfo*>, by position after updateAll
					std::uint8_t unkA8[0xC]; // A8
					std::uint8_t unkB4[0x8]; // B4
					std::uint32_t unkBC; // BC
					std::uint64_t unkC0; // C0
					std::uint32_t unkC8; // C8
					std::uint8_t unkCC[0xC]; // CC
					std::uint64_t unkD8; // D8
					std::uint8_t unkE0[0xC]; // E0
					std::uint8_t unkEC[0x8]; // EC
					std::uint32_t unkF4; // F4
					std::uint64_t unkF8; // F8
					std::uint32_t unk100; // 100
					std::uint8_t unk104[0xC]; // 104
					std::uint64_t unk110; // 110
					std::uint64_t unk118; // 118
					bool init; // 120 - NVMI
					std::uint8_t unk121[0x7]; // 121
				};
				static_assert(sizeof(NavMeshInfoMap) == 0x128);
			}
		}
	}
}
