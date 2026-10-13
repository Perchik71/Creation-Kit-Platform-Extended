// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFormIDListView.h>
#include <EditorAPI/N/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct ImageSpaceModifierData
				{
					bool animatable; // 00
					float duration; // 04
					std::uint32_t keySize[21][2]; // 08
					std::uint32_t tintColorKeySize; // B0
					std::uint32_t blurKeySize; // B4
					std::uint32_t doubleKeySize; // B8
					std::uint32_t radialBlurStrengthKeySize; // BC
					std::uint32_t radialBlurRampupKeySize; // C0
					std::uint32_t radialBlurStartKeySize; // C4
					bool useTargetForRadialBlur; // C8
					NiAPI::NiPoint2 radialBlurCenter; // CC
					std::uint32_t depthOfFieldStrengthKeySize; // D4
					std::uint32_t depthOfFieldDistanceKeySize; // D8
					std::uint32_t depthOfFieldRangeKeySize; // DC
					bool useTargetForDepthOfField; // E0
					std::int8_t depthOfFieldMode; // E1
					std::uint32_t radialBlurRampDownKeySize; // E4
					std::uint32_t radialBlurDownStartKeySize; // E8
					std::uint32_t fadeColorKeySize; // EC
					std::uint32_t motionBlurStrengthKeySize; // F0
					std::uint32_t depthOfFieldVignetteRadiusKeySize; // F4
					std::uint32_t depthOfFieldVignetteStrengthKeySize; // F8
				};
				static_assert(sizeof(ImageSpaceModifierData) == 0xFC);

				// 1.11.240.0: size 0xDC8, vtable RVA 0x30A9C88, 126 virtual functions
				class TESImageSpaceModifier :
					public TESFormIDListView   // 000
				{
				public:
					virtual ~TESImageSpaceModifier() = default;

				public:
					ImageSpaceModifierData data; // 28
					std::uint32_t unk124; // 124
					std::uint8_t interpolator[21][2][0x30]; // 128 - NiPointer<NiFloatInterpolator>[21][2] in the game (0x30-byte objects in the CK)
					std::uint8_t blurInterpolator[0x30]; // 908
					std::uint8_t doubleInterpolator[0x30]; // 938
					std::uint8_t tintColorInterpolator[0x38]; // 968 - NiColorInterpolator in the game
					std::uint8_t fadeColorInterpolator[0x38]; // 9A0 - NiColorInterpolator in the game
					std::uint8_t radialBlurStrengthInterpolator[0x30]; // 9D8
					std::uint8_t radialBlurRampupInterpolator[0x30]; // A08
					std::uint8_t radialBlurStartInterpolator[0x30]; // A38
					std::uint8_t radialBlurRampDownInterpolator[0x30]; // A68
					std::uint8_t radialBlurDownStartInterpolator[0x30]; // A98
					std::uint8_t depthOfFieldStrengthInterpolator[0x30]; // AC8
					std::uint8_t depthOfFieldDistanceInterpolator[0x30]; // AF8
					std::uint8_t depthOfFieldRangeInterpolator[0x30]; // B28
					std::uint8_t depthOfFieldVignetteRadiusInterpolator[0x30]; // B58
					std::uint8_t depthOfFieldVignetteStrengthInterpolator[0x30]; // B88
					std::uint8_t motionBlurStrengthInterpolator[0x30]; // BB8
					std::uint8_t unkBE8[0x1C8]; // BE8
					std::uint32_t unkDB0; // DB0
					std::uint32_t unkDB4; // DB4
					std::uint64_t unkDB8; // DB8
					std::uint32_t unkDC0; // DC0
					std::uint32_t unkDC4; // DC4
				};
				static_assert(sizeof(TESImageSpaceModifier) == 0xDC8);
			}
		}
	}
}
