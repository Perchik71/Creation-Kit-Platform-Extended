// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x78, vtable RVA 0x30D2BF0
				class Script :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftScript;

					virtual ~Script() = default;
				public:
					std::uint8_t unk28[0x50];	// 28 - data, layout not resolved
				};
				static_assert(offsetof(Script, unk28) == 0x28);
				static_assert(sizeof(Script) == 0x78);
			}
		}
	}
}