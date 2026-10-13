// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSImpactDataSet;
				class BGSMaterialType;
			}

			// 1.11.240.0: size 0x18, vtable RVA 0x30189E8, 21 virtual functions
			class BGSBlockBashData :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSBlockBashData() = default;

			public:
				Forms::BGSImpactDataSet* blockBashImpactDataSet; // 08
				Forms::BGSMaterialType* altBlockMaterialType; // 10
			};
			static_assert(sizeof(BGSBlockBashData) == 0x18);
		}
	}
}
