// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESForm.h"
#include "NiAPI/NiTypes.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			// 0x920 - It hurts me so much on one form, and most of the memory is empty.
			class TESObjectLIGH : public TESForm
			{
			private:
				char pad[0x6EE];
				NiAPI::NiRGB m_SpecularColor;
			public:
				inline NiAPI::NiRGB GetSpecularColor() const { return m_SpecularColor; }
				inline void SetSpecularColor(const NiAPI::NiRGB& Color) { m_SpecularColor = Color; }
			};
		}
	}
}

#pragma pack(pop)