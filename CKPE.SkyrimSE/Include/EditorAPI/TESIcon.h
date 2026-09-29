// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/TESTexture.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESIcon : public TESTexture
			{
			public:
				virtual ~TESIcon() = default;

				// override
				[[nodiscard]] const char* GetCaptionForOpenDialog() const noexcept(true) override { return "Add Icon File"; }
			};
			static_assert(sizeof(TESIcon) == 0x28);
		}
	}
}