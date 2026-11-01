// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESObjectACTI.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x1E0, vtable RVA 0x30F4508
				class TESFurniture :
					public TESObjectACTI
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftFurniture;

					virtual ~TESFurniture() = default;


				public:
					std::uint8_t unk138[0xA8];	// 138 - data, layout not resolved
				};
				static_assert(offsetof(TESFurniture, unk138) == 0x138);
				static_assert(sizeof(TESFurniture) == 0x1E0);
			}
		}
	}
}