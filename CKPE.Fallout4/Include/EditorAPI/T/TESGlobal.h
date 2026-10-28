// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x30, vtable RVA 0x30661B8, 124 virtual functions
				class TESGlobal :
					public TESForm   // 000
				{
				public:
					virtual ~TESGlobal() = default;

					constexpr static std::uint8_t TYPE_ID = ftGlobal;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESGlobal[0];
				public:
					float value; // 28
					std::uint32_t unk2C; // 2C
				};
				static_assert(sizeof(TESGlobal) == 0x30);
			}
		}
	}
}
