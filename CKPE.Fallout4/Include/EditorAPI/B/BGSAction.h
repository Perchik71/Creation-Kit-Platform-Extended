// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSKeyword.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x68, vtable RVA 0x302F038, 124 virtual functions
				class BGSAction :
					public BGSKeyword   // 000
				{
				public:
					virtual ~BGSAction() = default;

					constexpr static std::uint8_t TYPE_ID = ftAction;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSAction[0];
				public:
					std::uint32_t index; // 60
					std::uint32_t unk64; // 64
				};
				static_assert(sizeof(BGSAction) == 0x68);
			}
		}
	}
}
