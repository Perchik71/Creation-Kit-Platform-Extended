// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiFlags.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/T/TESForm.h>

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