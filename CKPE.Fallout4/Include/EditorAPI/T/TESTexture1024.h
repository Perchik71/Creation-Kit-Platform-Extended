// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x28, vtable RVA 0x30B8FB8, 27 virtual functions
			class TESTexture1024 :
				public TESTexture   // 000
			{
			public:
				virtual ~TESTexture1024() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESTexture1024[0];
			};
			static_assert(sizeof(TESTexture1024) == 0x28);
		}
	}
}
