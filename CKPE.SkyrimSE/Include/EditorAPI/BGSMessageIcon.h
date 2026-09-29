// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/TESIcon.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BGSMessageIcon : public BaseFormComponent
			{
				TESIcon icon;
			public:
				virtual ~BGSMessageIcon() = default;

				[[nodiscard]] inline TESIcon& GetIcon() noexcept(true) { return icon; }
			};
			static_assert(sizeof(BGSMessageIcon) == 0x30);
		}
	}
}