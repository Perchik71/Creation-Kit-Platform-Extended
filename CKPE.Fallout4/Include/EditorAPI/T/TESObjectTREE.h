// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESProduceForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BaseTreeData;

				struct OBJ_TREE
				{
					float trunkFlexibility; // 00
					float branchFlexibility; // 04
					float trunkAmplitude; // 08
					float frontAmplitude; // 0C
					float backAmplitude; // 10
					float sideAmplitude; // 14
					float frontFrequency; // 18
					float backFrequency; // 1C
					float sideFrequency; // 20
					float leafFlexibility; // 24
					float leafAmplitude; // 28
					float leafFrequency; // 2C
				};
				static_assert(sizeof(OBJ_TREE) == 0x30);

				// 1.11.240.0: size 0x158, vtable RVA 0x309B7D8, 154 virtual functions
				class TESObjectTREE :
					public TESBoundObject,        // 000
					public BGSModelMaterialSwap,  // 090
					public TESFullName,           // 0E0
					public TESProduceForm         // 0F8
				{
				public:
					virtual ~TESObjectTREE() = default;

				public:
					OBJ_TREE data; // 118
					BaseTreeData* baseData; // 148
					std::int32_t type; // 150 - REX::TEnumSet<etTreeType, std::int32_t>
					std::uint32_t unk154; // 154
				};
				static_assert(sizeof(TESObjectTREE) == 0x158);
			}
		}
	}
}
