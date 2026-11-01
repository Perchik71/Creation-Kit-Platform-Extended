// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: vtable RVA 0x30CD0F0. Interface: only the vtable pointer, the virtuals are not resolved.
			struct BSISoundCategory
			{
				void* vftable;	// 00
			};
			static_assert(sizeof(BSISoundCategory) == 0x08);
		}
	}
}