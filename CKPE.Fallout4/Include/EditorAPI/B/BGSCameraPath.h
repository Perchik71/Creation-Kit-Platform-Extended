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
				struct PATH_DATA
				{
					std::int8_t flags; // 00
				};
				static_assert(sizeof(PATH_DATA) == 0x1);

				// 1.11.240.0: size 0x70, vtable RVA 0x3054830, 124 virtual functions
				class BGSCameraPath :
					public TESForm   // 000
				{
				public:
					virtual ~BGSCameraPath() = default;

				public:
					std::uint8_t unk28[0x18]; // 28
					std::uint64_t unk40; // 40
					std::uint64_t unk48; // 48
					PATH_DATA data; // 50
					std::uint8_t unk51[0x7]; // 51
					std::uint64_t unk58; // 58
					std::uint64_t unk60; // 60
					std::uint64_t unk68; // 68
				};
				static_assert(sizeof(BGSCameraPath) == 0x70);
			}
		}
	}
}
