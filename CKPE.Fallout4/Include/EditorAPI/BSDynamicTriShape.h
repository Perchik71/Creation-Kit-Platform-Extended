// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSTriShape.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 170
			class BSDynamicTriShape : public BSTriShape
			{};
			static_assert(sizeof(BSDynamicTriShape) == 0x170);
		}
	}
}