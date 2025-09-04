// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"
#include "../Interfaces/IKeywordFormBase.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeywordForm : public BaseFormComponent
				{
				public:
					Interfaces::IKeywordFormBase KeywordFormIntf;
				private:
					char pad40_22[0x78];
				};
				static_assert(sizeof(BGSKeywordForm) == 0xC0);
			}
		}
	}
}