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
				class BGSSoundDescriptorForm;
				class TESBoundObject;
			}

			// 1.11.240.0: size 0x20, vtable RVA 0x30219D8, 21 virtual functions
			class TESProduceForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESProduceForm() = default;

			public:
				Forms::BGSSoundDescriptorForm* harvestSound; // 08
				Forms::TESBoundObject* produceItem; // 10
				std::int8_t produceChance[4]; // 18
				std::uint32_t pad1C; // 1C
			};
			static_assert(sizeof(TESProduceForm) == 0x20);
		}
	}
}
