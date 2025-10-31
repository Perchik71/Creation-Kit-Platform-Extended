// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BGSLocalizedString.h>
#include <EditorAPI/Forms/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESFullName : public BaseFormComponent
				{
				private:
					BGSLocalizedString name;
				public:
					virtual ~TESFullName() = default;

					[[nodiscard]] inline std::uint32_t GetFullNameLength() const noexcept(true) { return name.length(); }
					[[nodiscard]] inline const char* GetFullName() const noexcept(true)
					{ 
						auto s = name.c_str();
						return s ? s : ""; 
					}
					inline operator const char*() const { return GetFullName(); }
				};	
				static_assert(sizeof(TESFullName) == 0x18);
			}
		}
	}
}