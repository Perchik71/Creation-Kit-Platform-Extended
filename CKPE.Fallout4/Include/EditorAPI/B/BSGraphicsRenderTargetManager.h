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
				// Only renderTargetData shares one layout across every supported build.
				struct RenderTargetManager
				{
					/* 0x000 */ RenderTargetProperties renderTargetData[100];

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
					void AcquireRenderTarget(RENDER_TARGET target) noexcept(true);
					void ReleaseRenderTarget(RENDER_TARGET target) noexcept(true);
					// Asserts unless the target is acquired.
					void SaveRenderTarget(RENDER_TARGET target, const char* fileName,
						std::int32_t format, bool flag) noexcept(true);
					// Releases every shared target except the swap-chain framebuffer.
					void ReleaseRenderTargets() noexcept(true);
					// Sized from State's back-buffer extent.
					void CreateRenderTargets() noexcept(true);
				};
			}
		}
	}
}
