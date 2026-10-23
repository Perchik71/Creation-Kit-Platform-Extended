// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderMaterial.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSEffectShaderMaterial : public BSShaderMaterial
			{
			public:
				float FalloffStartAngle;								// 38
				float FalloffStopAngle;								// 3C
				float FalloffStartOpacity;							// 40
				float FalloffStopOpacity;							// 44
				NiAPI::NiColorA BaseColor;							// 48
				NiAPI::NiPointer<NiAPI::NiTexture> BaseTexture;		// 58
				NiAPI::NiPointer<NiAPI::NiTexture> GrayscaleTexture;	// 60
				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentTexture;	// 68
				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentMaskTexture;	// 70
				NiAPI::NiPointer<NiAPI::NiTexture> NormalTexture;		// 78
				float SoftFalloffDepth;								// 80
				float BaseColorScale;								// 84
				BSFixedString SourceTexturePath;						// 88
				BSFixedString GrayscaleTexturePath;					// 90
				BSFixedString EnvironmentTexturePath;				// 98
				BSFixedString EnvironmentMaskTexturePath;			// A0
				BSFixedString NormalTexturePath;						// A8
				float EnvironmentMapScale;							// B0
				std::uint8_t TextureClampMode;						// B4
				std::uint8_t LightingInfluence;						// B5
				std::uint8_t EnvironmentMinLOD;						// B6
				std::uint8_t UnkB7;									// B7
			};
			static_assert(sizeof(BSEffectShaderMaterial) == 0xB8);
			static_assert(offsetof(BSEffectShaderMaterial, BaseColor) == 0x48);
			static_assert(offsetof(BSEffectShaderMaterial, BaseTexture) == 0x58);
			static_assert(offsetof(BSEffectShaderMaterial, SoftFalloffDepth) == 0x80);
			static_assert(offsetof(BSEffectShaderMaterial, SourceTexturePath) == 0x88);
			static_assert(offsetof(BSEffectShaderMaterial, TextureClampMode) == 0xB4);
		}
	}
}
