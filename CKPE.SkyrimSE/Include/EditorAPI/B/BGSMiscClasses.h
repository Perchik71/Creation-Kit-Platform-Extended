// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/N/NiRefObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// Miscellaneous CK classes (1.7.99.0). Sizes from the allocations of the constructors.

			// vtable RVA 0x3042240, 2 virtual functions
			class BGSAttackData : public NiAPI::NiRefObject
			{
			public:
				std::uint8_t unk10[0x40];	// 10 - layout not resolved
			};
			static_assert(sizeof(BGSAttackData) == 0x50);

			// vtable RVA 0x3175008, 2 virtual functions
			class BGSLOSData : public NiAPI::NiRefObject
			{
			public:
				void* unk10;			// 10 - 0 on construction
				void* unk18;			// 18 - 0 on construction
				std::uint32_t unk20;	// 20 - 0 on construction
				std::uint16_t unk24;	// 24 - 0 on construction
				std::uint16_t pad26;	// 26
			};
			static_assert(offsetof(BGSLOSData, unk10) == 0x10);
			static_assert(sizeof(BGSLOSData) == 0x28);
		}
	}
}