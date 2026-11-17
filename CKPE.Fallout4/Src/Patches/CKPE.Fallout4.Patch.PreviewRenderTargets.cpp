// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

				using RenderFrame = void(*)(EditorAPI::TESPreviewControl*);
				using BeginFrame = void(*)(Graphics::Renderer*, std::uint32_t);
				using EndFrame = void(*)(Graphics::Renderer*);

				RenderFrame originalRenderFrame;
				BeginFrame originalBeginFrame;
				EndFrame originalEndFrame;

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

				void InvalidateCachedStates(Graphics::RendererThreadState* state) noexcept(true)
				{
					for (auto& shadowState : state->shadowStates)
						shadowState.InvalidateCachedState();
				}

				void SetStateExtents(SIZE backBuffer, SIZE screen) noexcept(true)
				{
					auto state = Graphics::State::GetSingleton();
					state->backBufferWidth = backBuffer.cx;
					state->backBufferHeight = backBuffer.cy;
					state->screenWidth = screen.cx;
					state->screenHeight = screen.cy;
				}

				SIZE GetTargetsExtent(const Graphics::RenderTargetManager* manager) noexcept(true)
				{
					const auto& main = manager->renderTargetData[
						static_cast<std::size_t>(Graphics::RENDER_TARGET::kMAIN)];
					return { static_cast<LONG>(main.width), static_cast<LONG>(main.height) };
				}

				void EnsureTargets(SIZE extent) noexcept(true)
				{
					auto manager = Graphics::RenderTargetManager::GetSingleton();
					const auto current = GetTargetsExtent(manager);
					if ((current.cx == extent.cx) && (current.cy == extent.cy))
					{
						SetStateExtents(extent, extent);
						return;
					}

					const auto context = Graphics::GetRendererThreadContext();
					const auto threadState = context->GetState();
					auto& framebuffer = Graphics::GetRendererData()->renderTargets[0];
					const auto window = Graphics::GetCurrentRendererWindow();

					context->deviceContext->ClearState();
					context->deviceContext->Flush();
					InvalidateCachedStates(threadState);
					framebuffer.rtView = nullptr;
					framebuffer.srView = nullptr;
					framebuffer.uaView = nullptr;

					manager->ReleaseRenderTargets();
					context->deviceContext->Flush();
					SetStateExtents(extent, extent);

					manager->creating = true;
					Graphics::RenderTargetManager::CreateRenderTargets();
					manager->creating = false;

					framebuffer.rtView = window->swapChainRenderTarget.rtView;
					framebuffer.srView = window->swapChainRenderTarget.srView;
					framebuffer.uaView = window->swapChainRenderTarget.uaView;

					Graphics::Renderer::GetSingleton()->UpdateViewport(
						&threadState->shadowStates[0], 0, 0, false);
					InvalidateCachedStates(threadState);
				}

				void PreviewBeginFrame(Graphics::Renderer* renderer,
					std::uint32_t windowIndex) noexcept(true)
				{
					originalBeginFrame(renderer, windowIndex);
					if (!activePreview)
						return;

					auto& frame = *activePreview;
					const auto state = Graphics::State::GetSingleton();
					frame.previousExtent =
						GetTargetsExtent(Graphics::RenderTargetManager::GetSingleton());
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

					PreviewFrame frame(extent);
					originalRenderFrame(control);
				}

				// Call sites and vtables reach their bodies through E9 thunks.
				bool IsThunkTo(std::uintptr_t thunk, const Common::ID& body) noexcept(true)
				{
					return thunk && (*reinterpret_cast<const std::uint8_t*>(thunk) == 0xE9) &&
						(Detours::BranchTarget(thunk) == body.Address());
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
				using EditorAPI::TESRenderControl;
				namespace IDs = EditorAPI::IDs;

				const Relocation vtable(EditorAPI::TESPreviewControl::VTABLE);
				const Relocation begin(IDs::TESRenderControl::RenderFrame, 0xF3);
				const Relocation end(IDs::TESRenderControl::RenderFrame, 0x1FB);
				const auto frameThunk = reinterpret_cast<const std::uintptr_t*>(
					vtable.Address())[TESRenderControl::RENDER_VFUNC_INDEX];
				const auto beginThunk = Detours::BranchTarget(begin.Address());
				const auto endThunk = Detours::BranchTarget(end.Address());
				if (!begin.IsBranchTo(beginThunk) || !end.IsBranchTo(endThunk) ||
					!IsThunkTo(frameThunk, IDs::TESRenderControl::RenderFrame) ||
					!IsThunkTo(beginThunk, IDs::BSGraphics::Renderer::BeginFrame) ||
					!IsThunkTo(endThunk, IDs::BSGraphics::Renderer::EndFrame))
					return false;

				originalRenderFrame = reinterpret_cast<RenderFrame>(frameThunk);
				originalBeginFrame = reinterpret_cast<BeginFrame>(beginThunk);
				originalEndFrame = reinterpret_cast<EndFrame>(endThunk);
				if (!begin.RedirectBranch(beginThunk, &PreviewBeginFrame) ||
					!end.RedirectBranch(endThunk, &PreviewEndFrame))
					return false;
				vtable.WriteVFunc(TESRenderControl::RENDER_VFUNC_INDEX, &RenderPreview);
				return true;
			}
		}
	}
}
