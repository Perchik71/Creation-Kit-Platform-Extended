// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x70, vtable RVA 0x30424C0, 124 virtual functions
				class TESEyes :
					public TESForm,      // 000
					public TESFullName,  // 028
					public TESTexture    // 040
				{
				public:
					virtual ~TESEyes() = default;

					constexpr static std::uint8_t TYPE_ID = ftEyes;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESEyes[0];
				public:
					std::int8_t flags; // 68
					std::uint8_t unk69[0x7]; // 69
				};
				static_assert(sizeof(TESEyes) == 0x70);
			}
		}
	}
}
