// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESModel.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSModelMaterialSwap : public TESModel
				{
					char pad60[0x8];
				public:
					[[nodiscard]] inline const BGSModelMaterialSwap* GetAsModelMaterialSwap() const noexcept(true)
					{
						return this;
					}
				};
				static_assert(sizeof(BGSModelMaterialSwap) == 0x68);
			}
		}
	}
}