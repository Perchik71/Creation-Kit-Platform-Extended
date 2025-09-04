// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"
#include "../BGSLocalizedString.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESFullName : public BaseFormComponent
				{
					[[nodiscard]] inline std::uint32_t GetFullNameLengthOrig() const noexcept(true)
					{
						return vtbl_call<std::uint32_t>(0xF0, this);
					}

					[[nodiscard]] inline const char* GetFullNameOrig() const noexcept(true)
					{
						return vtbl_call<const char*>(0xF8, this);
					}

					[[nodiscard]] inline const BGSLocalizedString& GetFullNameLocalized() const noexcept(true)
					{
						return vtbl_call<BGSLocalizedString&>(0x100, this);
					}

					BGSLocalizedString FullName;
				};
				static_assert(sizeof(TESFullName) == 0x50);
			}
		}
	}
}