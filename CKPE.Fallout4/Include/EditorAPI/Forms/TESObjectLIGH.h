// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include <EditorAPI/NiAPI/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x268
				class TESObjectLIGH : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLight;

					virtual ~TESObjectLIGH() = default;
				
					[[nodiscard]] inline NiAPI::NiRGB GetSpecularColor() const noexcept(true) { return m_SpecularColor; }
				
					CKPE_READ_PROPERTY(GetSpecularColor) NiAPI::NiRGB SpecularColor;
				private:
					char pad28[0x1B8];
					NiAPI::NiRGB m_SpecularColor;
				};
				static_assert(sizeof(TESObjectLIGH) == 0x1E8);
			}
		}
	}
}