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
				class BGSInstanceNamingRules;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x2FA70A0, 21 virtual functions
			class BGSInstanceNamingRulesForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSInstanceNamingRulesForm() = default;
				/* 08 */ [[nodiscard]] std::uint32_t GetFormComponentType() const noexcept(true) override;
				/* 10 */ void InitializeDataComponent() override;

			public:
				Forms::BGSInstanceNamingRules* instanceNamingRules; // 08
			};
			static_assert(sizeof(BGSInstanceNamingRulesForm) == 0x10);
		}
	}
}
