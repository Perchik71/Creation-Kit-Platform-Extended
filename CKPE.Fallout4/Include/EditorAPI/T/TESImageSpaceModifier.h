// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFormIDListView.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiInterpolator.h>

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
				static_assert(offsetof(ImageSpaceModifierData, duration) == 0x4);
				static_assert(offsetof(ImageSpaceModifierData, radialBlurCenter) == 0xCC);
				static_assert(offsetof(ImageSpaceModifierData, depthOfFieldVignetteStrengthKeySize) == 0xF8);

				// Unlike the game, the CK embeds its interpolators.
				class TESImageSpaceModifier :
					public TESFormIDListView   // 000
				{
				public:
					virtual ~TESImageSpaceModifier() = default;

					constexpr static std::uint8_t TYPE_ID = ftImageSpaceModifier;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESImageSpaceModifier[0];
				public:
					ImageSpaceModifierData data; // 28
					std::uint32_t unk124; // 124
					NiAPI::NiFloatInterpolator interpolator[21][2]; // 128 - embedded in the CK (NiPointer<NiFloatInterpolator>[21][2] in the game)
					NiAPI::NiFloatInterpolator blurInterpolator; // 908
					NiAPI::NiFloatInterpolator doubleInterpolator; // 938
					NiAPI::NiColorInterpolator tintColorInterpolator; // 968 - embedded in the CK
					NiAPI::NiColorInterpolator fadeColorInterpolator; // 9A0 - embedded in the CK
					NiAPI::NiFloatInterpolator radialBlurStrengthInterpolator; // 9D8
					NiAPI::NiFloatInterpolator radialBlurRampupInterpolator; // A08
					NiAPI::NiFloatInterpolator radialBlurStartInterpolator; // A38
					NiAPI::NiFloatInterpolator radialBlurRampDownInterpolator; // A68
					NiAPI::NiFloatInterpolator radialBlurDownStartInterpolator; // A98
					NiAPI::NiFloatInterpolator depthOfFieldStrengthInterpolator; // AC8
					NiAPI::NiFloatInterpolator depthOfFieldDistanceInterpolator; // AF8
					NiAPI::NiFloatInterpolator depthOfFieldRangeInterpolator; // B28
					NiAPI::NiFloatInterpolator depthOfFieldVignetteRadiusInterpolator; // B58
					NiAPI::NiFloatInterpolator depthOfFieldVignetteStrengthInterpolator; // B88
					NiAPI::NiFloatInterpolator motionBlurStrengthInterpolator; // BB8
					std::uint8_t unkBE8[0x1C8]; // BE8
					std::uint32_t unkDB0; // DB0
					std::uint32_t unkDB4; // DB4
					std::uint64_t unkDB8; // DB8 - a string set to "ImageSpaceModifier" by the constructor
					std::uint32_t unkDC0; // DC0
					std::uint32_t unkDC4; // DC4
				};
				static_assert(offsetof(TESImageSpaceModifier, data) == 0x28);
				static_assert(offsetof(TESImageSpaceModifier, interpolator) == 0x128);
				static_assert(offsetof(TESImageSpaceModifier, motionBlurStrengthInterpolator) == 0xBB8);
				static_assert(sizeof(TESImageSpaceModifier) == 0xDC8);
			}
		}
	}
}
