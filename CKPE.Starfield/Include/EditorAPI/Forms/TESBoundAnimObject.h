// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESBoundObject.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESBoundAnimObject : public TESBoundObject
				{
				public:
				};
				static_assert(sizeof(TESBoundObject) == 0x2F0);
			}
		}
	}
}