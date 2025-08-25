// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "..\BSFixedString.h"
#include "..\BGSLocalizedString.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 0x00000000086DD5F8, 0x0000000000000000 offset, 34 functions
				class TESFormRefCount
				{
				public:
					virtual ~TESFormRefCount();
				};
				static_assert(sizeof(TESFormRefCount) == 0x8);

				// 0x00000000084B1430, 0x0000000000000000 offset, 30 functions (0x48 size)
				class BaseFormComponent : public TESFormRefCount
				{
					char pad08[0x40];
				public:
					virtual ~BaseFormComponent();
				};
				static_assert(sizeof(BaseFormComponent) == 0x48);

				class BGSFormDialogInterface
				{
				public:
				};
			}
		}
	}
}
