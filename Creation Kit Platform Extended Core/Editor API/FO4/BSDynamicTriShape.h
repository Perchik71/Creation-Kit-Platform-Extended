// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BSTriShape.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// 170
			class BSDynamicTriShape : public BSTriShape
			{};
			static_assert(sizeof(BSDynamicTriShape) == 0x170);
		}
	}
}