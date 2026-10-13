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
				// 1.11.240.0: size 0x68, vtable RVA 0x302DB90, 126 virtual functions
				class TESRegion :
					public TESForm   // 000
				{
				public:
					virtual ~TESRegion() = default;

					virtual void sub_3E0() { return; } // 124 - RVA 0x95D4B0
					virtual void sub_3E8() { return; } // 125 - RVA 0x95D630

				public:
					std::uint32_t unk28; // 28
					std::uint32_t unk2C; // 2C
					std::uint64_t unk30; // 30
					void* pointLists; // 38 - BSSimpleList<TESRegionPointList*>*
					std::uint64_t unk40; // 40
					std::uint64_t unk48; // 48
					std::uint64_t unk50; // 50
					std::uint32_t unk58; // 58
					std::uint8_t unk5C[0xC]; // 5C
				};
				static_assert(sizeof(TESRegion) == 0x68);
			}
		}
	}
}
