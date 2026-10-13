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
				// 1.11.240.0: size 0x58, vtable RVA 0x30A62D8, 124 virtual functions
				class BGSReferenceGroup :
					public TESForm   // 000
				{
				public:
					virtual ~BGSReferenceGroup() = default;

				public:
					std::uint64_t unk28; // 28
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					std::uint32_t unk38; // 38
					std::uint32_t unk3C; // 3C
					std::uint64_t unk40; // 40
					std::uint64_t unk48; // 48
					std::uint32_t unk50; // 50
					std::uint32_t unk54; // 54
				};
				static_assert(sizeof(BGSReferenceGroup) == 0x58);
			}
		}
	}
}
