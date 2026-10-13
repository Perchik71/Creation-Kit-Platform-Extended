// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/T/TESProduceForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x208, vtable RVA 0x3089580, 154 virtual functions
				class TESFlora :
					public TESObjectACTI,   // 000
					public TESProduceForm   // 1E8
				{
				public:
					virtual ~TESFlora() = default;
				};
				static_assert(sizeof(TESFlora) == 0x208);
			}
		}
	}
}
