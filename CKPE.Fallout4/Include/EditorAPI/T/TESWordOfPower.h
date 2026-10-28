// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x50, vtable RVA 0x307D338, 124 virtual functions
				class TESWordOfPower :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~TESWordOfPower() = default;

					constexpr static std::uint8_t TYPE_ID = ftWordOfPower;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESWordOfPower[0];
				public:
					BGSLocalizedString translation; // 40
				};
				static_assert(sizeof(TESWordOfPower) == 0x50);
			}
		}
	}
}
