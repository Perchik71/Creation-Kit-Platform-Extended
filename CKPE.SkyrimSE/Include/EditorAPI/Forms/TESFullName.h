// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BGSLocalizedString.h>
#include <EditorAPI/Forms/BaseFormComponent.h>

#pragma pack(push, 1)

namespace CKPE
{
	namespace SkyrimSE
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

					inline std::uint32_t length() const noexcept(true) { return name.length(); }
					inline const char* c_str() const noexcept(true) 
					{ 
						auto s = name.c_str();
						return s ? s : ""; 
					}
					inline operator const char*() const { return c_str(); }

					// 0x78 vtbl get_name
				};	
				static_assert(sizeof(TESFullName) == 0x18);
			}
		}
	}
}

#pragma pack(pop)