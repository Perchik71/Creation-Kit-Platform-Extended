// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BGSKeyword.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSLocationRefType : public BGSKeyword
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLocationRef;

					virtual ~BGSLocationRefType() = default;
				};
				static_assert(sizeof(BGSLocationRefType) == 0x30);
			}
		}
	}
}
