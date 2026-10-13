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
				class BGSTerminal;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x2FA9560, 21 virtual functions
			class BGSNativeTerminalForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSNativeTerminalForm() = default;

			public:
				Forms::BGSTerminal* terminal; // 08
			};
			static_assert(sizeof(BGSNativeTerminalForm) == 0x10);
		}
	}
}
