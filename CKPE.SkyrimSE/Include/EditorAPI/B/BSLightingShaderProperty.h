// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderProperty.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// Layout confirmed in the constructor/destructor (operator delete size 0x160) and in
			// BSLightingShader::SetupGeometry of the Creation Kit 1.5.73.0, 1.6.438.0 and 1.7.99.0 (the 1.6.1130.0 and
			// 1.6.1378.1 executables contain the same instruction sequence as 1.6.438.0).
			class BSLightingShaderProperty : public BSShaderProperty
			{
			private:
				char _pad88[0x68];					// 88 (texture sets, material colours, ...)
			public:
				// Emissive colour (per-geometry "PropertyColor" pixel shader constant = emissiveColor * emissiveMultiple).
				// The pointer references a private 12 byte colour only when BSSP_FLAG_OWN_EMIT (0x16) is set, otherwise it
				// can point to a shared default, so never write through it; swap the pointer instead.
				NiAPI::NiColor* pEmissiveColor;		// F0
				float fEmissiveMultiple;			// F8

				[[nodiscard]] inline NiAPI::NiColor* GetEmissiveColor() const noexcept(true) { return pEmissiveColor; }
				[[nodiscard]] inline float GetEmissiveMultiple() const noexcept(true) { return fEmissiveMultiple; }
			private:
				char _padFC[0x64];					// FC
			};
			static_assert(sizeof(BSLightingShaderProperty) == 0x160);
			static_assert(offsetof(BSLightingShaderProperty, pEmissiveColor) == 0xF0);
			static_assert(offsetof(BSLightingShaderProperty, fEmissiveMultiple) == 0xF8);
		}
	}
}
