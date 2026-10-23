// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSPreloadable.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMaterialType;
				class BGSImpactData;
				// 1.11.240.0: size 0x68, vtable RVA 0x305BB50, 124 virtual functions
				class BGSImpactDataSet :
					public TESForm,         // 000
					public BGSPreloadable   // 028
				{
				public:
					virtual ~BGSImpactDataSet() = default;

				public:
					// 0x38 byte table (capacity 3C, good 44, modification count 50, entries 60); ClearData walks 24 byte entries {material, impact data, next}
					BSTHashMap<const BGSMaterialType*, BGSImpactData*> impactMap; // 30
				};
				static_assert(sizeof(BGSImpactDataSet) == 0x68);
			}
		}
	}
}
