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
			// 1.7.99.0: size 0x08, vtable RVA 0x30AEAB8. Interface with a vtable only; the virtuals are not resolved.
			struct ActorValueOwner
			{
				void* vftable;	// 00
			};
			static_assert(sizeof(ActorValueOwner) == 0x08);
		}
	}
}