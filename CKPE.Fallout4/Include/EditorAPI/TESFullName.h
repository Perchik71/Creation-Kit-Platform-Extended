// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BGSLocalizedString.h>
#include <EditorAPI/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESFullName : public BaseFormComponent
			{
			private:
				BGSLocalizedString name;
			public:
				virtual ~TESFullName() = default;

				[[nodiscard]] virtual std::uint32_t GetFullNameLength() const noexcept(true);
				[[nodiscard]] virtual const char* GetFullName() const noexcept(true);

				inline operator const char*() const noexcept(true) { return GetFullName(); }
			};	
			static_assert(sizeof(TESFullName) == 0x18);
		}
	}
}