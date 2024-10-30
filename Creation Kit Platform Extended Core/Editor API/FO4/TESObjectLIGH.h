// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"
#include "NiAPI/NiTypes.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x268
			class TESObjectLIGH : public TESForm
			{
			public:
				virtual ~TESObjectLIGH() = default;
			public:
				inline NiAPI::NiRGB GetSpecularColor() const { return m_SpecularColor; }
			public:
				READ_PROPERTY(GetSpecularColor) NiAPI::NiRGB SpecularColor;
			private:
				char pad28[0x1B8];
				NiAPI::NiRGB m_SpecularColor;
			};
			static_assert(sizeof(TESObjectLIGH) == 0x1E3);
		}
	}
}

#pragma pack(pop)