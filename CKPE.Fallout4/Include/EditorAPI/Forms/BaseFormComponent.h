// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 21 vtbl funcs (more purecall: no implementation)
				class BaseFormComponent
				{
				public:
					virtual ~BaseFormComponent() = default;
				};
				static_assert(sizeof(BaseFormComponent) == 0x8);
			}
		}
	}
}