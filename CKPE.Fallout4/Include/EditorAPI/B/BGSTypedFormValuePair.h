// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTHashMap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
			}

			// Form/value pairs kept in a heap allocated array (damage types, actor value modifiers, required items, property lists,
			// keyword chances); the element is {form, value} padded to 0x10 bytes.
			namespace BGSTypedFormValuePair
			{
				union SharedVal
				{
					std::uint32_t i;
					float f;
				};
				static_assert(sizeof(SharedVal) == 0x4);

				using Array = BSTArray<BSTTuple<Forms::TESForm*, SharedVal>>;
			}
		}
	}
}
