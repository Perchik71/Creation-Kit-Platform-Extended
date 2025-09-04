// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BGSPreviewTransform.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSnapTemplate: public TESForm, public BGSPreviewTransform
				{
				public:
				};
				static_assert(sizeof(BGSSnapTemplate) == 0x198);
			}
		}
	}
}