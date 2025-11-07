// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/TESFullName.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uint32_t TESFullName::GetFullNameLength() const noexcept(true)
			{ 
				return name.length(); 
			}
			
			const char* TESFullName::GetFullName() const noexcept(true)
			{
				auto s = name.c_str();
				return s ? s : "";
			}
		}
	}
}