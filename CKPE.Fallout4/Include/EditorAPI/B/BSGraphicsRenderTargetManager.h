// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <span>
#include <EditorAPI/B/BSGraphicsTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				// CK 1.10.982+ layout; fields that move on 1.10.162 are accessor-only.
				struct RenderTargetManager
				{
					/* 0x000 */ RenderTargetProperties renderTargetData[100];
				private:
					/* 0xC80 */ DepthStencilTargetProperties depthStencilTargetData[12];
					/* 0xDD0 */ CubeMapRenderTargetProperties cubeMapRenderTargetData[1];
					/* 0xDF4 */ std::int32_t renderTargetID[100];
					/* 0xF84 */ std::int32_t depthStencilTargetID[12];
					/* 0xFB4 */ std::int32_t cubeMapRenderTargetID[1];
					/* 0xFB8 */ bool creating;
					/* 0xFB9 */ std::byte _padFB9[3];
				public:
					[[nodiscard]] static RenderTargetManager* GetSingleton() noexcept(true);
					// Empty on 1.10.162, whose depth-stencil properties are 0x18 bytes.
					[[nodiscard]] std::span<const DepthStencilTargetProperties>
						GetDepthStencilTargetData() const noexcept(true);
					[[nodiscard]] std::span<const CubeMapRenderTargetProperties, 1>
						GetCubeMapRenderTargetData() const noexcept(true);
					[[nodiscard]] std::span<const std::int32_t, 100>
						GetRenderTargetIDs() const noexcept(true);
					[[nodiscard]] std::span<const std::int32_t, 12>
						GetDepthStencilTargetIDs() const noexcept(true);
					[[nodiscard]] std::span<const std::int32_t, 1>
						GetCubeMapRenderTargetIDs() const noexcept(true);
					[[nodiscard]] bool IsCreating() const noexcept(true);
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
					// Logical IDs must be translated through GetRenderTargetIDs after acquire.
					void AcquireRenderTarget(RENDER_TARGET target) noexcept(true);
					void ReleaseRenderTarget(RENDER_TARGET target) noexcept(true);
					// Asserts unless the target is acquired.
					void SaveRenderTarget(RENDER_TARGET target, const char* fileName,
						std::int32_t format, bool flag) noexcept(true);
					// Releases every shared target except the swap-chain framebuffer.
					void ReleaseRenderTargets() noexcept(true);
					// Sized from State's back-buffer extent.
					void CreateRenderTargets() noexcept(true);

					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(RenderTargetManager, depthStencilTargetData) == 0xC80 &&
							offsetof(RenderTargetManager, cubeMapRenderTargetData) == 0xDD0 &&
							offsetof(RenderTargetManager, renderTargetID) == 0xDF4 &&
							offsetof(RenderTargetManager, depthStencilTargetID) == 0xF84 &&
							offsetof(RenderTargetManager, cubeMapRenderTargetID) == 0xFB4 &&
							offsetof(RenderTargetManager, creating) == 0xFB8;
					}
				};
				static_assert(sizeof(RenderTargetManager) == 0xFBC);
				static_assert(RenderTargetManager::VerifyLayout());
			}
		}
	}
}
