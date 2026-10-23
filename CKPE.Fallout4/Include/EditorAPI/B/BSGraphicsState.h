// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <immintrin.h>
#include <EditorAPI/B/BSGraphicsTypes.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/N/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				struct alignas(0x10) ViewData
				{
					/* 0x000 */ NiAPI::NiRect<float> viewPort;
					/* 0x010 */ NiAPI::NiPoint2 viewDepthRange;
					/* 0x018 */ std::byte _pad018[8];
					/* 0x020 */ __m128 viewUp;
					/* 0x030 */ __m128 viewRight;
					/* 0x040 */ __m128 viewDir;
					/* 0x050 */ __m128 viewMat[4];
					/* 0x090 */ __m128 projMat[4];
					/* 0x0D0 */ __m128 viewProjMat[4];
					/* 0x110 */ __m128 viewProjUnjittered[4];
					/* 0x150 */ __m128 currentViewProjUnjittered[4];
					/* 0x190 */ __m128 previousViewProjUnjittered[4];
					/* 0x1D0 */ std::byte _pad1D0[0x40];
				};
				static_assert(sizeof(ViewData) == 0x210);
				static_assert(offsetof(ViewData, viewMat) == 0x50);
				static_assert(offsetof(ViewData, viewProjUnjittered) == 0x110);
				static_assert(offsetof(ViewData, previousViewProjUnjittered) == 0x190);

				struct alignas(0x10) CameraStateData
				{
					/* 0x000 */ ViewData camViewData;
					/* 0x210 */ NiAPI::NiPoint3 posAdjust;
					/* 0x21C */ NiAPI::NiPoint3 currentPosAdjust;
					/* 0x228 */ NiAPI::NiPoint3 previousPosAdjust;
					/* 0x234 */ std::byte _pad234[4];
					/* 0x238 */ const NiAPI::NiCamera* referenceCamera;
					/* 0x240 */ bool useJitter;
					/* 0x241 */ std::byte _pad241[0xF];
				};
				static_assert(sizeof(CameraStateData) == 0x250);
				static_assert(offsetof(CameraStateData, referenceCamera) == 0x238);

				struct alignas(0x10) RendererShadowState
				{
					/* 0x000 */ std::uint32_t stateUpdateFlags;
					/* 0x004 */ std::byte _pad004[0x8C];
					/* 0x090 */ D3D11_VIEWPORT viewPort;
					/* 0x0A8 */ std::byte _pad0A8[0x608];
					/* 0x6B0 */ NiAPI::NiPoint3A posAdjust;
					/* 0x6C0 */ NiAPI::NiPoint3A previousPosAdjust;
					/* 0x6D0 */ ViewData cameraData;
					/* 0x8E0 */ std::byte _pad8E0[0x30];
				};
				static_assert(sizeof(RendererShadowState) == 0x910);
				static_assert(offsetof(RendererShadowState, viewPort) == 0x90);
				static_assert(offsetof(RendererShadowState, cameraData) == 0x6D0);

				// CK 1.10.982.3+ layout; 1.10.162 shifts the temporal-AA fields.
				struct alignas(0x10) State
				{
					/* 0x000 */ std::uint32_t currentFrame;
					/* 0x004 */ float offsetX;
					/* 0x008 */ float offsetY;
					/* 0x00C */ std::uint32_t currentFrameOffset;
					/* 0x010 */ std::uint32_t previousFrameOffset;
					/* 0x014 */ std::byte _pad014[0x64];
					/* 0x078 */ std::uint32_t backBufferWidth;
					/* 0x07C */ std::uint32_t backBufferHeight;
					/* 0x080 */ std::uint32_t screenWidth;
					/* 0x084 */ std::uint32_t screenHeight;
					/* 0x088 */ std::byte _pad088[0x10];
					/* 0x098 */ std::uint32_t frameCount;
					/* 0x09C */ std::uint32_t frameID;
					/* 0x0A0 */ bool insideFrame;
					/* 0x0A1 */ std::byte _pad0A1[0xB];
					/* 0x0AC */ std::uint32_t taaState;
					/* 0x0B0 */ std::uint32_t taaDisableCounter;
					/* 0x0B4 */ std::byte _pad0B4[0x8C];
					/* 0x140 */ BSTArray<CameraStateData> cameraDataCache;
					/* 0x158 */ std::byte _pad158[8];
					/* 0x160 */ CameraStateData cameraState;
					/* 0x3B0 */ std::byte _pad3B0[0x10];

					// Returns null on 1.10.162 rather than exposing the wrong layout.
					[[nodiscard]] static State* GetSingleton() noexcept(true);
					void UpdateTemporalData() noexcept(true);
					void SetCamera(const NiAPI::NiCamera* camera, bool useJitter,
						float minDepth, float maxDepth) noexcept(true);
				};
				static_assert(sizeof(State) == 0x3C0);
				static_assert(offsetof(State, frameCount) == 0x98);
				static_assert(offsetof(State, taaState) == 0xAC);
				static_assert(offsetof(State, cameraDataCache) == 0x140);
				static_assert(offsetof(State, cameraState) == 0x160);
			}
		}
	}
}
