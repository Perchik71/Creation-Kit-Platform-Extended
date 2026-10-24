// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/N/NiTObjectArray.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSTriShape;
	class ImageSpaceManager;
	class ImageSpaceEffectParam;
	class ImageSpaceTexture;

	class ImageSpaceEffect
	{
	public:
		struct EffectDesc;
		struct EffectInput;

		ImageSpaceEffect() = delete;
		/* 00 */ virtual ~ImageSpaceEffect();
		/* 01 */ virtual void Render(BSTriShape* Geometry, ImageSpaceEffectParam* Param);
		/* 02 */ virtual void Dispatch(ImageSpaceEffectParam* Param, bool UseAsyncPipe,
			std::uint32_t Effect, EffectDesc* Description);
		/* 03 */ virtual void Setup(ImageSpaceManager* Manager, ImageSpaceEffectParam* Param);
		/* 04 */ virtual void Shutdown();
		/* 05 */ virtual void BorrowTextures(ImageSpaceEffectParam* Param);
		/* 06 */ virtual void ReturnTextures();
		/* 07 */ virtual void UpdateComputeShaderParam(std::uint32_t Effect);
		/* 08 */ virtual bool IsActive();
		/* 09 */ virtual bool UpdateParams(ImageSpaceEffectParam* Param);
		/* 0A */ virtual bool SetRenderStates(ImageSpaceEffectParam* Param);
		/* 0B */ virtual bool RestoreRenderStates(ImageSpaceEffectParam* Param);
		/* 0C */ virtual const char* GetName();

		/* 0x008 */ bool isActive;
		/* 0x009 */ bool paramsChanged;
		/* 0x00A */ std::byte _pad00A[6];
		/* 0x010 */ NiAPI::NiTObjectArray<ImageSpaceEffect*> effectList;
		/* 0x028 */ NiAPI::NiTObjectArray<ImageSpaceEffectParam*> effectParamList;
		/* 0x040 */ NiAPI::NiTObjectArray<ImageSpaceTexture*> textures;
		/* 0x058 */ NiAPI::NiTObjectArray<ImageSpaceTexture*> vsTextures;
		/* 0x070 */ NiAPI::NiTObjectArray<EffectInput*> effectInputs;
		/* 0x088 */ NiAPI::NiTObjectArray<std::int32_t*> effectOutput;
		/* 0x0A0 */ bool isComputeShader;
		/* 0x0A1 */ std::byte _pad0A1[3];
		/* 0x0A4 */ std::uint32_t outputCount;
		/* 0x0A8 */ bool useDynamicResolution;
		/* 0x0A9 */ std::byte _pad0A9[7];
	};
	static_assert(sizeof(ImageSpaceEffect) == 0xB0);
	static_assert(offsetof(ImageSpaceEffect, effectList) == 0x10);
	static_assert(offsetof(ImageSpaceEffect, effectInputs) == 0x70);
	static_assert(offsetof(ImageSpaceEffect, outputCount) == 0xA4);
}
