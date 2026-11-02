// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/I/ImageSpaceEffectOption.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class ImageSpaceEffectHDR : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectHDR;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectHDR[0];
	};
	static_assert(sizeof(ImageSpaceEffectHDR) == 0xB0);

	class ImageSpaceEffectHDRCS : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectHDRCS;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectHDRCS[0];
	};
	static_assert(sizeof(ImageSpaceEffectHDRCS) == 0xB0);

	class ImageSpaceEffectRadialBlur : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectRadialBlur;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectRadialBlur[0];
	};
	static_assert(sizeof(ImageSpaceEffectRadialBlur) == 0xB0);

	class ImageSpaceEffectMap : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectMap;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectMap[0];
	};
	static_assert(sizeof(ImageSpaceEffectMap) == 0xB0);

	class ImageSpaceEffectAmbientOcclusion : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectAmbientOcclusion;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectAmbientOcclusion[0];
	};
	static_assert(sizeof(ImageSpaceEffectAmbientOcclusion) == 0xB0);

	class ImageSpaceEffectMotionBlur : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectMotionBlur;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectMotionBlur[0];
	};
	static_assert(sizeof(ImageSpaceEffectMotionBlur) == 0xB0);

	class ImageSpaceEffectSunbeams : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectSunbeams;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectSunbeams[0];
	};
	static_assert(sizeof(ImageSpaceEffectSunbeams) == 0xB0);

	class ImageSpaceEffectFullScreenColor : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectFullScreenColor;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectFullScreenColor[0];
	};
	static_assert(sizeof(ImageSpaceEffectFullScreenColor) == 0xB0);

	class ImageSpaceEffectUpsampleDynamicResolution : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectUpsampleDynamicResolution;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectUpsampleDynamicResolution[0];
	};
	static_assert(sizeof(ImageSpaceEffectUpsampleDynamicResolution) == 0xB0);

	class ImageSpaceEffectVLS : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectVLS;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectVLS[0];
	};
	static_assert(sizeof(ImageSpaceEffectVLS) == 0xB0);

	class ImageSpaceEffectRefraction : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectRefraction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectRefraction[0];

		/* 0x0B0 */ std::byte _pad0B0[0x28];
	};
	static_assert(sizeof(ImageSpaceEffectRefraction) == 0xD8);

	class ImageSpaceEffectGetHit : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectGetHit;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectGetHit[0];

		/* 0x0B0 */ std::byte _pad0B0[0x58];
	};
	static_assert(sizeof(ImageSpaceEffectGetHit) == 0x108);

	class ImageSpaceEffectFullScreenBlur : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectFullScreenBlur;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectFullScreenBlur[0];

		/* 0x0B0 */ std::byte _pad0B0[0x78];
	};
	static_assert(sizeof(ImageSpaceEffectFullScreenBlur) == 0x128);

	class ImageSpaceEffectWaterDisplacement : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectWaterDisplacement;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectWaterDisplacement[0];

		/* 0x0B0 */ std::byte _pad0B0[0x20];
	};
	static_assert(sizeof(ImageSpaceEffectWaterDisplacement) == 0xD0);

	class ImageSpaceEffectNoise : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectNoise;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectNoise[0];

		/* 0x0B0 */ std::byte _pad0B0[0x2C8];
	};
	static_assert(sizeof(ImageSpaceEffectNoise) == 0x378);

	class ImageSpaceEffectPipboyScreen : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectPipboyScreen;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectPipboyScreen[0];

		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectPipboyScreen) == 0xC0);

	class ImageSpaceEffectHUDGlass : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectHUDGlass;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectHUDGlass[0];

		/* 0x0B0 */ std::byte _pad0B0[0x68];
	};
	static_assert(sizeof(ImageSpaceEffectHUDGlass) == 0x118);

	class ImageSpaceEffectVatsTarget : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectVatsTarget;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectVatsTarget[0];

		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectVatsTarget) == 0xC0);

	class ImageSpaceEffectModMenu : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectModMenu;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectModMenu[0];

		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectModMenu) == 0xC0);

	class ImageSpaceEffectScalableAmbientObscurance : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectScalableAmbientObscurance;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectScalableAmbientObscurance[0];

		/* 0x0B0 */ std::byte _pad0B0[0x8];
	};
	static_assert(sizeof(ImageSpaceEffectScalableAmbientObscurance) == 0xB8);

	class ImageSpaceEffectScalableAmbientObscuranceCS : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectScalableAmbientObscuranceCS;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectScalableAmbientObscuranceCS[0];

		/* 0x0B0 */ std::byte _pad0B0[0x78];
	};
	static_assert(sizeof(ImageSpaceEffectScalableAmbientObscuranceCS) == 0x128);

	class ImageSpaceEffectTemporalAA : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectTemporalAA;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectTemporalAA[0];

		/* 0x0B0 */ std::byte _pad0B0[0x10];
	};
	static_assert(sizeof(ImageSpaceEffectTemporalAA) == 0xC0);

	class ImageSpaceEffectVLSLight : public ImageSpaceEffect
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectVLSLight;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectVLSLight[0];

		/* 0x0B0 */ std::byte _pad0B0[0x140];
	};
	static_assert(sizeof(ImageSpaceEffectVLSLight) == 0x1F0);

	class ImageSpaceEffectDepthOfField : public ImageSpaceEffectOption
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectDepthOfField;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectDepthOfField[0];

		/* 0x0C8 */ std::byte _pad0C8[0xA8];
	};
	static_assert(sizeof(ImageSpaceEffectDepthOfField) == 0x170);

	class ImageSpaceEffectDepthOfFieldSplitScreen : public ImageSpaceEffectOption
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectDepthOfFieldSplitScreen;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectDepthOfFieldSplitScreen[0];

		/* 0x0C8 */ std::byte _pad0C8[0xF0];
	};
	static_assert(sizeof(ImageSpaceEffectDepthOfFieldSplitScreen) == 0x1B8);

	class ImageSpaceEffectBokehDepthOfField : public ImageSpaceEffectOption
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectBokehDepthOfField;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectBokehDepthOfField[0];
	};
	static_assert(sizeof(ImageSpaceEffectBokehDepthOfField) == 0xC8);

	class ImageSpaceEffectRainSplash : public ImageSpaceEffectOption
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ImageSpaceEffectRainSplash;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ImageSpaceEffectRainSplash[0];

		/* 0x0C8 */ std::byte _pad0C8[0x38];
	};
	static_assert(sizeof(ImageSpaceEffectRainSplash) == 0x100);
}
