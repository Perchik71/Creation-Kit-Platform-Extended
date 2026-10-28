// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESDescription.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x70, vtable RVA 0x30667C8, 124 virtual functions
				class TESLoadScreen :
					public TESForm,         // 000
					public TESDescription   // 028
				{
				public:
					virtual ~TESLoadScreen() = default;

					constexpr static std::uint8_t TYPE_ID = ftLoadScreen;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESLoadScreen[0];
				public:
					std::uint8_t unk50[0x18]; // 50
					std::uint64_t unk68; // 68
				};
				static_assert(sizeof(TESLoadScreen) == 0x70);
			}
		}
	}
}
