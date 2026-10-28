// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				struct EffectShaderData
				{
					std::int32_t textureBlendModeSource; // 00 - REX::TEnumSet<_D3DBLEND, std::int32_t>
					std::int32_t textureBlendOperation; // 04 - REX::TEnumSet<_D3DBLENDOP, std::int32_t>
					std::int32_t textureZTestFunction; // 08 - REX::TEnumSet<_D3DCMPFUNC, std::int32_t>
					std::uint32_t fillColor1; // 0C
					float fillAlphaFadeInTime; // 10
					float fillAlphaFullTime; // 14
					float fillAlphaFadeOutTime; // 18
					float fillAlphaPersistentPercent; // 1C
					float fillAlphaPulseAmplitude; // 20
					float fillAlphaPulseFrequency; // 24
					float fillTextureUAnimSpeed; // 28
					float fillTextureVAnimSpeed; // 2C
					float edgeExponentValue; // 30
					std::uint32_t edgeColor; // 34
					float edgeAlphaFadeInTime; // 38
					float edgeAlphaFullTime; // 3C
					float edgeAlphaFadeOutTime; // 40
					float edgeAlphaPersistentPercent; // 44
					float edgeAlphaPulseAmplitude; // 48
					float edgeAlphaPulseFrequency; // 4C
					float fillAlphaFullPercent; // 50
					float edgeAlphaFullPercent; // 54
					std::int32_t textureBlendModeDest; // 58 - REX::TEnumSet<_D3DBLEND, std::int32_t>
					float alphaTestStartTime; // 5C
					float alphaTestEndTime; // 60
					float alphaTestStartValue; // 64
					float alphaTestEndValue; // 68
					BGSSoundDescriptorForm* ambientSound; // 70
					std::uint32_t fillColor2; // 78
					std::uint32_t fillColor3; // 7C
					float fillColorScaleA[3]; // 80
					float fillColorTimeA[3]; // 8C
					std::uint32_t flags; // 98
					float fillTextureUScale; // 9C
					float fillTextureVScale; // A0
					std::int8_t boneDepth; // A4
				};
				static_assert(sizeof(EffectShaderData) == 0xA8);

				// 1.11.240.0: size 0x188, vtable RVA 0x3088A70, 124 virtual functions
				class TESEffectShader :
					public TESForm,   // 000
					public TESModel   // 028
				{
				public:
					virtual ~TESEffectShader() = default;

					constexpr static std::uint8_t TYPE_ID = ftEffectShader;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESEffectShader[0];
				public:
					EffectShaderData data; // 60
					TESTexture textureShaderTexture; // 108
					TESTexture blockOutTexture; // 130
					TESTexture paletteTexture; // 158
					std::uint8_t shareableGeometry[0x8]; // 180 - NiPointer<BSGeometry>
				};
				static_assert(sizeof(TESEffectShader) == 0x188);
			}
		}
	}
}
