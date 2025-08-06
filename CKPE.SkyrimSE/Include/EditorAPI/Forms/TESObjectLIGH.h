// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include "TESForm.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x1A8
				class TESObjectLIGH : public TESForm
				{
				public:
					virtual ~TESObjectLIGH() = default;
				public:
					inline NiAPI::NiRGB GetSpecularColor() const noexcept(true) { return m_SpecularColor; }
				public:
					CKPE_READ_PROPERTY(GetSpecularColor) NiAPI::NiRGB SpecularColor;
				private:
					char pad28[0x138];
					NiAPI::NiRGB m_SpecularColor;
				};
				static_assert(sizeof(TESObjectLIGH) == 0x163);
			}
		}
	}
}