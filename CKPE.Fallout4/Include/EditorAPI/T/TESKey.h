// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectMISC.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x1E8, vtable RVA 0x308C828, 156 virtual functions
				class TESKey :
					public TESObjectMISC   // 000
				{
				public:
					virtual ~TESKey() = default;

					constexpr static std::uint8_t TYPE_ID = ftKey;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESKey[0];
				};
				static_assert(sizeof(TESKey) == 0x1E8);
			}
		}
	}
}
