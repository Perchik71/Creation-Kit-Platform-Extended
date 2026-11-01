// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x20, vtable RVA 0x30768F0. Bytes @08/@09, pointer @10 and a 32 bit value @18 are zeroed on construction (roles not resolved).
			class BGSIdleCollection : public BaseFormComponent
			{
			public:
				virtual ~BGSIdleCollection() = default;

				std::uint8_t unk08;		// 08
				std::uint8_t unk09;		// 09
				std::uint8_t pad0A[6];	// 0A
				void* unk10;			// 10
				std::uint32_t unk18;	// 18
				std::uint32_t pad1C;	// 1C
			};
			static_assert(sizeof(BGSIdleCollection) == 0x20);
		}
	}
}