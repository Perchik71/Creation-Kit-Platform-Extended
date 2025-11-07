// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/TESDescription.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uint32_t TESDescription::GetDescriptionTextLength() const noexcept(true)
			{ 
				return descriptionText.length();
			}

			const char* TESDescription::GetDescriptionText() const noexcept(true)
			{
				auto s = descriptionText.c_str();
				return s ? s : "";
			}
		}
	}
}