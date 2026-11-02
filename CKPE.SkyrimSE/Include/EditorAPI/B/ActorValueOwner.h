// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x08, vtable RVA 0x30AEAB8. Interface with a vtable only; the virtuals are not resolved.
			struct ActorValueOwner
			{
				constexpr static Common::ID RTTI = IDs::RTTI::ActorValueOwner;
				constexpr static Common::ID VTABLE = IDs::VTABLE::ActorValueOwner[0];
				void* vftable;	// 00
			};
			static_assert(sizeof(ActorValueOwner) == 0x08);
		}
	}
}