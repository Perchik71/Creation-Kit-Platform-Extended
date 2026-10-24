// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/I/ImageSpaceEffectOption.h>

namespace CKPE::Fallout4::EditorAPI
{
	class ImageSpaceEffectHDR : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectHDR) == 0xB0);

	class ImageSpaceEffectHDRCS : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectHDRCS) == 0xB0);

	class ImageSpaceEffectRadialBlur : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectRadialBlur) == 0xB0);

	class ImageSpaceEffectMap : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectMap) == 0xB0);

	class ImageSpaceEffectAmbientOcclusion : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectAmbientOcclusion) == 0xB0);

	class ImageSpaceEffectMotionBlur : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectMotionBlur) == 0xB0);

	class ImageSpaceEffectSunbeams : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectSunbeams) == 0xB0);

	class ImageSpaceEffectFullScreenColor : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectFullScreenColor) == 0xB0);

	class ImageSpaceEffectUpsampleDynamicResolution : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectUpsampleDynamicResolution) == 0xB0);

	class ImageSpaceEffectVLS : public ImageSpaceEffect {};
	static_assert(sizeof(ImageSpaceEffectVLS) == 0xB0);

	class ImageSpaceEffectRefraction : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x28];
	};
	static_assert(sizeof(ImageSpaceEffectRefraction) == 0xD8);

	class ImageSpaceEffectGetHit : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x58];
	};
	static_assert(sizeof(ImageSpaceEffectGetHit) == 0x108);

	class ImageSpaceEffectFullScreenBlur : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x78];
	};
	static_assert(sizeof(ImageSpaceEffectFullScreenBlur) == 0x128);

	class ImageSpaceEffectWaterDisplacement : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x20];
	};
	static_assert(sizeof(ImageSpaceEffectWaterDisplacement) == 0xD0);

	class ImageSpaceEffectNoise : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x2C8];
	};
	static_assert(sizeof(ImageSpaceEffectNoise) == 0x378);

	class ImageSpaceEffectPipboyScreen : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectPipboyScreen) == 0xC0);

	class ImageSpaceEffectHUDGlass : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x68];
	};
	static_assert(sizeof(ImageSpaceEffectHUDGlass) == 0x118);

	class ImageSpaceEffectVatsTarget : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectVatsTarget) == 0xC0);

	class ImageSpaceEffectModMenu : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectModMenu) == 0xC0);

	class ImageSpaceEffectScalableAmbientObscurance : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x8];
	};
	static_assert(sizeof(ImageSpaceEffectScalableAmbientObscurance) == 0xB8);

	class ImageSpaceEffectScalableAmbientObscuranceCS : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x78];
	};
	static_assert(sizeof(ImageSpaceEffectScalableAmbientObscuranceCS) == 0x128);

	class ImageSpaceEffectTemporalAA : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectTemporalAA) == 0xC0);

	class ImageSpaceEffectVLSLight : public ImageSpaceEffect
	{
		/* 0x0B0 */ std::byte _pad0B0[0x140];
	};
	static_assert(sizeof(ImageSpaceEffectVLSLight) == 0x1F0);

	class ImageSpaceEffectDepthOfField : public ImageSpaceEffectOption
	{
		/* 0x0C8 */ std::byte _pad0C8[0xA8];
	};
	static_assert(sizeof(ImageSpaceEffectDepthOfField) == 0x170);

	class ImageSpaceEffectDepthOfFieldSplitScreen : public ImageSpaceEffectOption
	{
		/* 0x0C8 */ std::byte _pad0C8[0xF0];
	};
	static_assert(sizeof(ImageSpaceEffectDepthOfFieldSplitScreen) == 0x1B8);

	class ImageSpaceEffectBokehDepthOfField : public ImageSpaceEffectOption {};
	static_assert(sizeof(ImageSpaceEffectBokehDepthOfField) == 0xC8);

	class ImageSpaceEffectRainSplash : public ImageSpaceEffectOption
	{
		/* 0x0C8 */ std::byte _pad0C8[0x38];
	};
	static_assert(sizeof(ImageSpaceEffectRainSplash) == 0x100);
}
