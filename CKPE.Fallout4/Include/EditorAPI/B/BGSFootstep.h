// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSImpactDataSet;

				// 1.11.240.0: size 0x38, vtable RVA 0x305A448, 124 virtual functions
				class BGSFootstep :
					public TESForm   // 000
				{
				public:
					virtual ~BGSFootstep() = default;

					constexpr static std::uint8_t TYPE_ID = ftFootstep;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSFootstep[0];
				public:
					BSFixedString tagString; // 28
					BGSImpactDataSet* impactSet; // 30
				};
				static_assert(sizeof(BGSFootstep) == 0x38);
			}
		}
	}
}
