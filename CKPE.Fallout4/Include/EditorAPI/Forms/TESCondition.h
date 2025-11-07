// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiFlags.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/Forms/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct TESCondition
				{
					void* head;
				};
				static_assert(sizeof(TESCondition) == 0x8);
			}
		}
	}
}