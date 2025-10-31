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
				class TESDescription : public BaseFormComponent
				{
				private:
					std::uint32_t fileOffset; 
					std::uint32_t chunkOffset;
					BGSLocalizedStringDL descriptionText;
				public:
					virtual ~TESDescription() = default;

					[[nodiscard]] inline std::uint32_t GetDescriptionTextLength() const noexcept(true) { return descriptionText.length(); }
					[[nodiscard]] inline const char* GetDescriptionText() const noexcept(true)
					{ 
						auto s = descriptionText.c_str();
						return s ? s : ""; 
					}
					inline operator const char*() const { return GetDescriptionText(); }
				};	
				static_assert(sizeof(TESDescription) == 0x28);
			}
		}
	}
}