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
				class TESObjectARMO;
			}

			// 1.11.240.0: size 0x18, vtable RVA 0x301B630, 21 virtual functions
			class BGSSkinForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSSkinForm() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSkinForm[0];
			public:
				Forms::TESObjectARMO* formSkin; // 08
				std::uint32_t skinControlID; // 10 - dialog control ID (IDC_SKIN, 1087 on construction)
				std::uint32_t pad14; // 14
			};
			static_assert(sizeof(BGSSkinForm) == 0x18);
		}
	}
}
