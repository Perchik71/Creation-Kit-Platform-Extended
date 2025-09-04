// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BGSTransform.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSPreviewTransform : public BaseFormComponent
				{
				public:					
					BGSTransform* Transforms[8];
				};
				static_assert(sizeof(BGSPreviewTransform) == 0x80);
			}
		}
	}
}