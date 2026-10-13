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
				class TESRace;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x3021C80, 22 virtual functions
			class TESRaceForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESRaceForm() = default;

				virtual bool SwitchRace(Forms::TESRace* pRace) noexcept(true) { return false; } // 21 - RVA 0x8F4EE0, *(_QWORD *)(a1 + 8) = a2;; return 1;

			public:
				Forms::TESRace* formRace; // 08
			};
			static_assert(sizeof(TESRaceForm) == 0x10);
		}
	}
}
