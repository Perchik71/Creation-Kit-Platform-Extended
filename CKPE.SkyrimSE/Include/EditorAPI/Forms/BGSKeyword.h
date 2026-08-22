// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "../BGSLocalizedString.h"
#include "TESForm.h"
#include <EditorAPI/NiAPI/NiTypes.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword : public TESForm
				{
					NiAPI::NiRGB _Color;
				public:
					constexpr static std::uint8_t TYPE_ID = ftKeyword;

					virtual ~BGSKeyword() = default;

					[[nodiscard]] inline NiAPI::NiRGB GetColor() const noexcept(true) { return _Color; }
					inline void SetColor(const NiAPI::NiRGB& color) noexcept(true) { _Color = color; }
					inline void SetColor(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept(true) { _Color = { r, g, b}; }

					CKPE_PROPERTY(GetColor, SetColor) NiAPI::NiRGB Color;
				};
				static_assert(sizeof(BGSKeyword) == 0x30);
			}
		}
	}
}
