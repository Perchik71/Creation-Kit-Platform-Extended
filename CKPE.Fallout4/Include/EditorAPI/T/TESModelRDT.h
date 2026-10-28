// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x38, vtable RVA 0x301C5D8, 27 virtual functions
			class TESModelRDT :
				public TESModel   // 000
			{
			public:
				virtual ~TESModelRDT() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESModelTri[0];
			};
			static_assert(sizeof(TESModelRDT) == 0x38);
		}
	}
}
