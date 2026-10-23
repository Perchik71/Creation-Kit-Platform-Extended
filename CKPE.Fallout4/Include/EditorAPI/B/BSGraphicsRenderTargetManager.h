// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSGraphicsTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				// Only the verified prefix of the CK 1.10.982.3+ object is modeled.
				struct RenderTargetManager
				{
					/* 0x000 */ RenderTargetProperties renderTargetData[100];
					/* 0xC80 */ DepthStencilTargetProperties depthStencilTargetData[12];
					/* 0xDD0 */ CubeMapRenderTargetProperties cubeMapRenderTargetData[1];
					/* 0xDF4 */ std::int32_t renderTargetID[100];
					/* 0xF84 */ std::int32_t depthStencilTargetID[12];
					/* 0xFB4 */ std::int32_t cubeMapRenderTargetID[1];
					/* 0xFB8 */ bool creating;
					/* 0xFB9 */ std::byte _padFB9[3];

					// Returns null on 1.10.162 rather than exposing the wrong layout.
					[[nodiscard]] static RenderTargetManager* GetSingleton() noexcept(true);
					// Creation requires the engine's render-target creation phase.
					void CreateRenderTarget(RENDER_TARGET target,
						const RenderTargetProperties* properties,
						RenderTargetCreationMode mode) noexcept(true);
					void CreateDepthStencilTarget(DEPTH_STENCIL_TARGET target,
						const DepthStencilTargetProperties* properties,
						RenderTargetCreationMode mode) noexcept(true);
					void CreateCubeMapRenderTarget(CUBEMAP_RENDER_TARGET target,
						const CubeMapRenderTargetProperties* properties,
						RenderTargetCreationMode mode) noexcept(true);
					// Logical IDs must be translated through renderTargetID after acquire.
					void AcquireRenderTarget(RENDER_TARGET target) noexcept(true);
					void ReleaseRenderTarget(RENDER_TARGET target) noexcept(true);
				};
				static_assert(sizeof(RenderTargetManager) == 0xFBC);
				static_assert(offsetof(RenderTargetManager, depthStencilTargetData) == 0xC80);
				static_assert(offsetof(RenderTargetManager, cubeMapRenderTargetData) == 0xDD0);
				static_assert(offsetof(RenderTargetManager, renderTargetID) == 0xDF4);
				static_assert(offsetof(RenderTargetManager, depthStencilTargetID) == 0xF84);
				static_assert(offsetof(RenderTargetManager, cubeMapRenderTargetID) == 0xFB4);
				static_assert(offsetof(RenderTargetManager, creating) == 0xFB8);
			}
		}
	}
}
