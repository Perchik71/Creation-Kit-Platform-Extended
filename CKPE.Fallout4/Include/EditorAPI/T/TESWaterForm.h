// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESImageSpace;

				class BGSSoundDescriptorForm;

				class SpellItem;

				class BGSMaterialType;

				struct WaterShaderData
				{
					float maxDepth; // 00
					std::uint32_t shallowWaterColor; // 04
					std::uint32_t deepWaterColor; // 08
					float shallowFogColorRange; // 0C
					float deepFogColorRange; // 10
					float shallowAlpha; // 14
					float deepAlpha; // 18
					float shallowFogAlphaRange; // 1C
					float deepFogAlphaRange; // 20
					std::uint32_t underwaterFogColor; // 24
					float underwaterFogAmount; // 28
					float underwaterFogDistNear; // 2C
					float underwaterFogDistFar; // 30
					float normalMagnitude; // 34
					float normalFalloffShallow; // 38
					float normalFalloffDeep; // 3C
					float reflectionAmount; // 40
					float fresnelAmount; // 44
					float surfaceEffectFalloff; // 48
					float displacementForce; // 4C
					float displacementVelocity; // 50
					float displacementFalloff; // 54
					float displacementDampener; // 58
					float displacementSize; // 5C
					std::uint32_t reflectionWaterColor; // 60
					float sunSpecularPower; // 64
					float sunSpecularMagnitude; // 68
					float sunSparklePower; // 6C
					float sunSparkleMagnitude; // 70
					float lightRadius; // 74
					float lightBrightness; // 78
					float shininess; // 7C
					float noiseWindDirections[3]; // 80
					float noiseWindSpeeds[3]; // 8C
					float amplitude[3]; // 98
					float uvScales[3]; // A4
					float noiseFalloff[3]; // B0
					float siltAmount; // BC
					std::uint32_t lightSiltColor; // C0
					std::uint32_t darkSiltColor; // C4
					bool ssrEnabled; // C8
				};
				static_assert(sizeof(WaterShaderData) == 0xCC);

				// 1.11.240.0: size 0x290, vtable RVA 0x30B8570, 124 virtual functions
				class TESWaterForm :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~TESWaterForm() = default;

				public:
					bool needUpdate; // 40
					std::uint8_t unk41[0x3]; // 41
					std::uint8_t unk44[0x8]; // 44
					std::uint8_t unk4C[0x8]; // 4C
					std::uint8_t unk54[0x8]; // 54
					std::uint8_t unk5C[0x8]; // 5C
					std::uint8_t unk64[0x8]; // 64
					std::uint8_t unk6C[0xC]; // 6C
					TESTexture unk78; // 78
					TESTexture unkA0; // A0
					TESTexture unkC8; // C8
					std::uint16_t unkF0; // F0
					std::uint8_t unkF2[0x6]; // F2
					BGSMaterialType* materialType; // F8
					BGSSoundDescriptorForm* waterSound; // 100
					WaterShaderData data; // 108 - DATA
					std::uint8_t unk1D4[0x4]; // 1D4
					std::uint64_t unk1D8; // 1D8
					std::uint8_t unk1E0; // 1E0
					std::uint8_t unk1E1[0x7]; // 1E1
					TESWaterForm* waterWeatherControl[3]; // 1E8
					std::uint64_t unk200; // 200
					std::uint32_t unk208; // 208
					std::uint32_t unk20C; // 20C
					std::uint32_t unk210; // 210
					std::uint32_t unk214; // 214
					std::uint32_t unk218; // 218
					std::uint32_t unk21C; // 21C
					std::uint32_t unk220; // 220
					std::uint32_t unk224; // 224
					SpellItem* consumeSpell; // 228
					SpellItem* contactSpell; // 230
					std::uint64_t unk238; // 238
					std::uint64_t unk240; // 240
					std::uint64_t unk248; // 248
					std::uint64_t unk250; // 250
					std::uint64_t unk258; // 258
					std::uint64_t unk260; // 260
					std::uint8_t unk268; // 268
					std::uint8_t unk269[0x7]; // 269
					TESImageSpace* imageSpace; // 270
					std::uint8_t linearVelocity[0xC]; // 278 - NiPoint3
					std::uint8_t angularVelocity[0xC]; // 284 - NiPoint3
				};
				static_assert(sizeof(TESWaterForm) == 0x290);
			}
		}
	}
}
