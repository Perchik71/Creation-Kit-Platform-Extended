// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESIcon : public TESTexture
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESIcon;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESIcon[0];

				virtual ~TESIcon() = default;

				// override
				[[nodiscard]] const char* GetCaptionForOpenDialog() const noexcept(true) override { return "Add Icon File"; }
			};
			static_assert(sizeof(TESIcon) == 0x28);
		}
	}
}