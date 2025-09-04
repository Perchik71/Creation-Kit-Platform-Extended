// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "TESFullName.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword : public TESForm, public TESFullName
				{
				private:
					std::uint64_t unk168;
					BSFixedString unk170;
					char pad178[0x48];
				};
				static_assert(sizeof(BGSKeyword) == 0x1C0);
			}
		}
	}
}