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
				class BGSListForm;
			}

			// 1.11.240.0: size 0x38, vtable RVA 0x301A2D8, 21 virtual functions
			class BGSOverridePackCollection :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSOverridePackCollection() = default;

			public:
				Forms::BGSListForm* spectatorOverRidePackList; // 08
				Forms::BGSListForm* observeCorpseOverRidePackList; // 10
				Forms::BGSListForm* guardWarnOverRidePackList; // 18
				Forms::BGSListForm* enterCombatOverRidePackList; // 20
				Forms::BGSListForm* followerCommandPackList; // 28
				Forms::BGSListForm* elevatorOverRidePackList; // 30
			};
			static_assert(sizeof(BGSOverridePackCollection) == 0x38);
		}
	}
}
