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
			// 1.7.99.0: vtable RVA 0x30CBD80. Interface: only the vtable pointer, the virtuals are not resolved.
			struct BSIMusicType
			{
				constexpr static Common::ID RTTI = IDs::RTTI::BSIMusicType;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSIMusicType[0];
				void* vftable;	// 00
			};
			static_assert(sizeof(BSIMusicType) == 0x08);
		}
	}
}