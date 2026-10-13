// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSPreloadable.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x68, vtable RVA 0x305BB50, 124 virtual functions
				class BGSImpactDataSet :
					public TESForm,         // 000
					public BGSPreloadable   // 028
				{
				public:
					virtual ~BGSImpactDataSet() = default;

				public:
					std::uint8_t unk30[0xC]; // 30
					std::uint8_t unk3C[0x8]; // 3C
					std::uint32_t unk44; // 44
					std::uint64_t unk48; // 48
					std::uint32_t unk50; // 50
					std::uint8_t unk54[0xC]; // 54
					std::uint64_t unk60; // 60
				};
				static_assert(sizeof(BGSImpactDataSet) == 0x68);
			}
		}
	}
}
