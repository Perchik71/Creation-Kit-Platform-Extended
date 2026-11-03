// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <intrin.h>
#include <utility>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/IDs.h>
#include <EditorAPI/B/BSGraphicsRenderer.h>
#include <EditorAPI/B/BSGraphicsRenderTargetManager.h>
#include <EditorAPI/B/BSGraphicsState.h>
#include <EditorAPI/T/TESPreviewControl.h>
#include <Patches/CKPE.Fallout4.Patch.PreviewRenderTargets.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			namespace
			{
				namespace Graphics = EditorAPI::BSGraphics;
				namespace GraphicsIDs = EditorAPI::IDs::BSGraphics;

				using RenderFrame = void(*)(EditorAPI::TESPreviewControl*);
				using BeginFrame = void(*)(Graphics::Renderer*, std::uint32_t);
				using EndFrame = void(*)(Graphics::Renderer*);
				using CreateTargets = void(*)();

				RenderFrame originalRenderFrame;
				BeginFrame originalBeginFrame;
				EndFrame originalEndFrame;
				SIZE allocatedExtent{};

				struct PreviewFrame;
				thread_local PreviewFrame* activePreview = nullptr;

				struct PreviewFrame
				{
					SIZE extent;
					SIZE previousExtent{};
					SIZE previousBackBuffer{};
					SIZE previousScreen{};
					bool began = false;
					PreviewFrame* previous;

					explicit PreviewFrame(SIZE size) noexcept(true) :
						extent(size), previous(std::exchange(activePreview, this))
					{}

					~PreviewFrame() noexcept(true)
					{
						activePreview = previous;
					}
				};

				std::byte* GetRenderThreadSlot(std::size_t offset) noexcept(true)
				{
					const auto index = *Common::Relocation<std::uint32_t*>(
						EditorAPI::IDs::Functions::data_8017B70);
					const auto tls = reinterpret_cast<std::uintptr_t*>(__readgsqword(0x58));
					return *reinterpret_cast<std::byte**>(tls[index] + offset);
				}

				std::byte* GetRenderThreadState() noexcept(true)
				{
					auto state = GetRenderThreadSlot(0x39B0);
					if (!state)
						state = *Common::Relocation<std::byte**>(
							GraphicsIDs::Renderer::DefaultThreadState);
					return state;
				}

				void InvalidateCachedState(std::byte* state) noexcept(true)
				{
					const Common::Relocation<void(*)(std::byte*)> invalidate(
						GraphicsIDs::Renderer::InvalidateCachedState);
					invalidate(state + 0x1B70);
					invalidate(state + 0x2480);
				}

				void SetStateExtents(SIZE backBuffer, SIZE screen) noexcept(true)
				{
					auto state = Graphics::State::GetSingleton();
					state->backBufferWidth = backBuffer.cx;
					state->backBufferHeight = backBuffer.cy;
					state->screenWidth = screen.cx;
					state->screenHeight = screen.cy;
				}

				void EnsureTargets(SIZE extent) noexcept(true)
				{
					if ((allocatedExtent.cx == extent.cx) &&
						(allocatedExtent.cy == extent.cy))
					{
						SetStateExtents(extent, extent);
						return;
					}

					auto manager = Graphics::RenderTargetManager::GetSingleton();
					auto data = Graphics::GetRendererData();
					auto threadState = GetRenderThreadState();
					auto creator = GetRenderThreadSlot(0x39A8);
					auto& framebuffer = data->renderTargets[0];
					const auto window = Graphics::GetCurrentRendererWindow();

					CKPE::vtbl_call<void>(0x370, creator);
					CKPE::vtbl_call<void>(0x378, creator);
					InvalidateCachedState(threadState);
					framebuffer.rtView = nullptr;
					framebuffer.srView = nullptr;
					framebuffer.uaView = nullptr;

					const Common::Relocation<void(*)(Graphics::RenderTargetManager*)>
						release(GraphicsIDs::RenderTargetManager::ReleaseRenderTargets);
					release(manager);
					CKPE::vtbl_call<void>(0x378, creator);
					SetStateExtents(extent, extent);

					const auto callback = *Common::Relocation<CreateTargets*>(
						GraphicsIDs::RenderTargetManager::CreationCallback);
					manager->creating = true;
					callback();
					manager->creating = false;
					allocatedExtent = extent;

					framebuffer.rtView = window->swapChainRenderTarget.rtView;
					framebuffer.srView = window->swapChainRenderTarget.srView;
					framebuffer.uaView = window->swapChainRenderTarget.uaView;

					const Common::Relocation<void(*)(Graphics::Renderer*,
						Graphics::RendererShadowState*, std::uint32_t, std::uint32_t, bool)>
						updateViewport(GraphicsIDs::Renderer::UpdateViewport);
					updateViewport(Graphics::Renderer::GetSingleton(),
						reinterpret_cast<Graphics::RendererShadowState*>(
							threadState + 0x1B70), 0, 0, false);
					InvalidateCachedState(threadState);
				}

				void PreviewBeginFrame(Graphics::Renderer* renderer,
					std::uint32_t windowIndex) noexcept(true)
				{
					originalBeginFrame(renderer, windowIndex);
					if (!activePreview)
						return;

					auto& frame = *activePreview;
					const auto state = Graphics::State::GetSingleton();
					const auto manager = Graphics::RenderTargetManager::GetSingleton();
					const auto& main = manager->renderTargetData[
						static_cast<std::size_t>(Graphics::RENDER_TARGET::kMAIN)];
					// Stock resizes refresh descriptors, not our cached allocation.
					allocatedExtent = { static_cast<LONG>(main.width),
						static_cast<LONG>(main.height) };
					frame.previousExtent = allocatedExtent;
					frame.previousBackBuffer = {
						static_cast<LONG>(state->backBufferWidth),
						static_cast<LONG>(state->backBufferHeight) };
					frame.previousScreen = { static_cast<LONG>(state->screenWidth),
						static_cast<LONG>(state->screenHeight) };
					frame.began = true;
					EnsureTargets(frame.extent);
				}

				void PreviewEndFrame(Graphics::Renderer* renderer) noexcept(true)
				{
					originalEndFrame(renderer);
					if (!activePreview || !activePreview->began)
						return;

					const auto& frame = *activePreview;
					EnsureTargets(frame.previousExtent);
					SetStateExtents(frame.previousBackBuffer, frame.previousScreen);
				}

				void RenderPreview(EditorAPI::TESPreviewControl* control) noexcept(true)
				{
					RECT rect{};
					if (!GetClientRect(control->GetDisplayWindow(), &rect))
						return;
					const SIZE extent{ rect.right - rect.left, rect.bottom - rect.top };
					// Smaller clients would produce zero-sized quarter targets.
					if ((extent.cx < 4) || (extent.cy < 4))
						return;
					if (!*Common::Relocation<CreateTargets*>(
						GraphicsIDs::RenderTargetManager::CreationCallback))
						return;

					PreviewFrame frame(extent);
					originalRenderFrame(control);
				}

				bool IsExpectedCall(const Common::Relocation<>& site,
					std::uintptr_t target) noexcept(true)
				{
					return (*reinterpret_cast<const std::uint8_t*>(site.Address()) == 0xE8) &&
						(Detours::BranchTarget(site.Address()) == target);
				}
			}

			PreviewRenderTargets::PreviewRenderTargets() : Common::Patch()
			{
				SetName("Preview Render Targets");
			}

			bool PreviewRenderTargets::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* PreviewRenderTargets::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool PreviewRenderTargets::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> PreviewRenderTargets::GetDependencies() const noexcept(true)
			{
				return { "D3D11 Patch" };
			}

			bool PreviewRenderTargets::DoQuery() const noexcept(true)
			{
				switch (VersionLists::GetEditorVersion())
				{
				case VersionLists::EDITOR_FALLOUT_C4_1_10_982_3:
				case VersionLists::EDITOR_FALLOUT_C4_1_11_137_0:
				case VersionLists::EDITOR_FALLOUT_C4_1_11_246_0:
					return true;
				default:
					return false;
				}
			}

			bool PreviewRenderTargets::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				const Relocation vtable(EditorAPI::TESPreviewControl::VTABLE);
				const Relocation begin(EditorAPI::IDs::TESRenderControl::RenderFrame, 0xF3);
				const Relocation end(EditorAPI::IDs::TESRenderControl::RenderFrame, 0x1FB);
				const auto frameTarget = Offset{ 0, 0x26EE0, 0x26797, 0x2662F }.Address();
				const auto beginTarget = Offset{ 0, 0x84F63, 0x85738, 0x858C3 }.Address();
				const auto endTarget = Offset{ 0, 0x6C0F8, 0x6C4EF, 0x6C5B7 }.Address();
				if ((*reinterpret_cast<const std::uintptr_t*>(vtable.Address() + 0x30) !=
					frameTarget) || !IsExpectedCall(begin, beginTarget) ||
					!IsExpectedCall(end, endTarget))
					return false;

				originalRenderFrame = reinterpret_cast<RenderFrame>(frameTarget);
				originalBeginFrame = reinterpret_cast<BeginFrame>(beginTarget);
				originalEndFrame = reinterpret_cast<EndFrame>(endTarget);
				if (!begin.RedirectBranch(beginTarget, &PreviewBeginFrame))
					return false;
				if (!end.RedirectBranch(endTarget, &PreviewEndFrame))
				{
					begin.WriteCall(originalBeginFrame);
					return false;
				}
				vtable.WriteVFunc(6, &RenderPreview);
				return true;
			}
		}
	}
}
