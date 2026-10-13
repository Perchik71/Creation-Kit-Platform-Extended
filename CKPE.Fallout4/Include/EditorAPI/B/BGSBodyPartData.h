// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSPreloadable.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xA0, vtable RVA 0x3030C60, 124 virtual functions
				class BGSBodyPartData :
					public TESForm,         // 000
					public TESModel,        // 028
					public BGSPreloadable   // 060
				{
				public:
					virtual ~BGSBodyPartData() = default;

				public:
					std::uint8_t unk68[0x10]; // 68
					std::uint64_t unk78; // 78
					std::uint64_t unk80; // 80
					std::uint64_t unk88; // 88
					std::uint64_t unk90; // 90
					std::uint64_t unk98; // 98
				};
				static_assert(sizeof(BGSBodyPartData) == 0xA0);
			}
		}
	}
}
