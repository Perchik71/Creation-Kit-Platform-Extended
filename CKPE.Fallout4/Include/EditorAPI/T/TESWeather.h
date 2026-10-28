// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BGSLightingTemplate.h>
#include <EditorAPI/T/TESTexture1024.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSGodRays;
				class BGSLensFlare;
				class BGSReferenceEffect;
				class BGSShaderParticleGeometryData;
				class TESImageSpace;

				class MagicItem;
				class TESObjectSTAT;
				class BGSSoundDescriptorForm;

				// 1.11.246.0: size 0x10
				struct WeatherSpellData
				{
					MagicItem* spell; // 00
					float threshold; // 08
					std::uint32_t pad0C; // 0C
				};
				static_assert(sizeof(WeatherSpellData) == 0x10);

				// 1.11.246.0: size 0x10, an element of the sound list
				struct WeatherSound
				{
					BGSSoundDescriptorForm* soundForm; // 00
					std::int32_t type; // 08 - REX::TEnumSet<SoundType, std::int32_t>
					std::uint32_t pad0C; // 0C
				};
				static_assert(sizeof(WeatherSound) == 0x10);

				// 1.11.240.0: size 0x1308, vtable RVA 0x30B90E0, 124 virtual functions
				class TESWeather :
					public TESForm   // 000
				{
				public:
					virtual ~TESWeather() = default;

					constexpr static std::uint8_t TYPE_ID = ftWeather;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESWeather[0];
				public:
					std::int32_t context; // 28 - REX::TEnumSet<SpellContext, std::int32_t>
					std::uint8_t unk2C[0x4]; // 2C
					TESTexture1024 cloudTexture[32]; // 30
					std::int8_t cloudLayerSpeedY[32]; // 530
					std::int8_t cloudLayerSpeedX[32]; // 550
					std::uint32_t cloudColorData[32][8]; // 570
					float cloudAlpha[32][8]; // 970
					std::uint32_t cloudLayerDisabledBits; // D70
					std::int8_t weatherData[20]; // D74
					float fogData[18]; // D88
					std::uint32_t colorData[19][8]; // DD0
					float fogColorScales[4][8]; // 1030
					WeatherSpellData weatherSpellData[2]; // 10B0
					BSSimpleList<WeatherSound*> soundList; // 10D0
					BSTArray<TESObjectSTAT*> skyStatics; // 10E0
					std::uint32_t numCloudLayers; // 10F8
					std::uint8_t unk10FC[0x4]; // 10FC
					TESImageSpace* imageSpace[8]; // 1100
					BGSGodRays* godRays[8]; // 1140
					BGSDirectionalAmbientLightingColors directionalAmbientLightingColors[8]; // 1180
					TESModel aurora; // 1280
					BGSLensFlare* sunGlareLensFlare; // 12B8
					float volatilityMult; // 12C0
					float visibilityMult; // 12C4
					BGSShaderParticleGeometryData* precipitationData; // 12C8
					BGSReferenceEffect* referenceEffect; // 12D0
					std::uint32_t unk12D8; // 12D8
					std::uint32_t unk12DC; // 12DC
					std::uint64_t unk12E0; // 12E0
					std::uint32_t unk12E8; // 12E8
					std::uint32_t unk12EC; // 12EC
					BSTArray<void*> unk12F0; // 12F0 - element type not identified yet (CK only)
				};
				static_assert(sizeof(TESWeather) == 0x1308);
			}
		}
	}
}
