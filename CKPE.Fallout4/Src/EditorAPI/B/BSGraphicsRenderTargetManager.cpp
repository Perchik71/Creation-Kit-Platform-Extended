// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/B/BSGraphicsRenderTargetManager.h>
#include <EditorAPI/IDs.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				constexpr Common::Offset CubeMapRenderTargetDataOffset{ 0xDA0, 0xDD0 };
				constexpr Common::Offset RenderTargetIDOffset{ 0xDC4, 0xDF4 };
				constexpr Common::Offset DepthStencilTargetIDOffset{ 0xF54, 0xF84 };
				constexpr Common::Offset CubeMapRenderTargetIDOffset{ 0xF84, 0xFB4 };
				constexpr Common::Offset CreatingOffset{ 0xF88, 0xFB8 };

				RenderTargetManager* RenderTargetManager::GetSingleton() noexcept(true)
				{
					static Common::Relocation<RenderTargetManager*> instance{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::Instance };
					return instance.Get();
				}

				std::span<const DepthStencilTargetProperties>
					RenderTargetManager::GetDepthStencilTargetData() const noexcept(true)
				{
					if (VersionLists::GetEditorVersion() <
						VersionLists::EDITOR_FALLOUT_C4_1_10_982_3)
						return {};
					return depthStencilTargetData;
				}

				std::span<const CubeMapRenderTargetProperties, 1>
					RenderTargetManager::GetCubeMapRenderTargetData() const noexcept(true)
				{
					return Common::RelocateMember<decltype(cubeMapRenderTargetData)>(
						this, CubeMapRenderTargetDataOffset);
				}

				std::span<const std::int32_t, 100>
					RenderTargetManager::GetRenderTargetIDs() const noexcept(true)
				{
					return Common::RelocateMember<decltype(renderTargetID)>(
						this, RenderTargetIDOffset);
				}

				std::span<const std::int32_t, 12>
					RenderTargetManager::GetDepthStencilTargetIDs() const noexcept(true)
				{
					return Common::RelocateMember<decltype(depthStencilTargetID)>(
						this, DepthStencilTargetIDOffset);
				}

				std::span<const std::int32_t, 1>
					RenderTargetManager::GetCubeMapRenderTargetIDs() const noexcept(true)
				{
					return Common::RelocateMember<decltype(cubeMapRenderTargetID)>(
						this, CubeMapRenderTargetIDOffset);
				}

				bool RenderTargetManager::IsCreating() const noexcept(true)
				{
					return Common::RelocateMember<bool>(this, CreatingOffset);
				}

				void RenderTargetManager::CreateRenderTarget(RENDER_TARGET target,
					const RenderTargetProperties* properties,
					RenderTargetCreationMode mode) noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*, RENDER_TARGET,
						const RenderTargetProperties*, RenderTargetCreationMode);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::CreateRenderTarget };
					function(this, target, properties, mode);
				}

				void RenderTargetManager::CreateDepthStencilTarget(
					DEPTH_STENCIL_TARGET target,
					const DepthStencilTargetProperties* properties,
					RenderTargetCreationMode mode) noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*, DEPTH_STENCIL_TARGET,
						const DepthStencilTargetProperties*, RenderTargetCreationMode);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::CreateDepthStencilTarget };
					function(this, target, properties, mode);
				}

				void RenderTargetManager::CreateCubeMapRenderTarget(
					CUBEMAP_RENDER_TARGET target,
					const CubeMapRenderTargetProperties* properties,
					RenderTargetCreationMode mode) noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*, CUBEMAP_RENDER_TARGET,
						const CubeMapRenderTargetProperties*, RenderTargetCreationMode);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::CreateCubeMapRenderTarget };
					function(this, target, properties, mode);
				}

				void RenderTargetManager::AcquireRenderTarget(
					RENDER_TARGET target) noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*, RENDER_TARGET);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::AcquireRenderTarget };
					function(this, target);
				}

				void RenderTargetManager::ReleaseRenderTarget(
					RENDER_TARGET target) noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*, RENDER_TARGET);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::ReleaseRenderTarget };
					function(this, target);
				}

				void RenderTargetManager::SaveRenderTarget(RENDER_TARGET target,
					const char* fileName, std::int32_t format, bool flag) noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*, RENDER_TARGET, const char*,
						std::int32_t, bool);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::Functions::BSGraphics__RenderTargetManager__SaveRenderTarget };
					function(this, target, fileName, format, flag);
				}

				void RenderTargetManager::ReleaseRenderTargets() noexcept(true)
				{
					using Function = void(*)(RenderTargetManager*);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::ReleaseRenderTargets };
					function(this);
				}

				void RenderTargetManager::CreateRenderTargets() noexcept(true)
				{
					using Function = void(*)();
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RenderTargetManager::CreateRenderTargets };
					auto& creating = Common::RelocateMember<bool>(this, CreatingOffset);
					creating = true;
					function();
					creating = false;
				}
			}
		}
	}
}
