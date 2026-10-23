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
			class BSD3DResourceCreator;

			namespace BSGraphics
			{
				struct RendererShadowState;

				struct RendererWindow
				{
					/* 0x000 */ HWND hwnd;
					/* 0x008 */ std::int32_t windowX;
					/* 0x00C */ std::int32_t windowY;
					/* 0x010 */ std::int32_t windowWidth;
					/* 0x014 */ std::int32_t windowHeight;
					/* 0x018 */ IDXGISwapChain* swapChain;
					/* 0x020 */ RenderTarget swapChainRenderTarget;
				};
				static_assert(sizeof(RendererWindow) == 0x50);
				static_assert(offsetof(RendererWindow, swapChain) == 0x18);
				static_assert(offsetof(RendererWindow, swapChainRenderTarget) == 0x20);

				struct RendererData
				{
					/* 0x0000 */ RendererShadowState* shadowState;
					/* 0x0008 */ BSD3DResourceCreator* resourceCreator;
					/* 0x0010 */ std::uint32_t adapter;
					/* 0x0014 */ DXGI_RATIONAL desiredRefreshRate;
					/* 0x001C */ DXGI_RATIONAL actualRefreshRate;
					/* 0x0024 */ DXGI_MODE_SCALING scaleMode;
					/* 0x0028 */ DXGI_MODE_SCANLINE_ORDER scanlineMode;
					/* 0x002C */ std::int32_t fullScreen;
					/* 0x0030 */ bool appFullScreen;
					/* 0x0031 */ bool borderlessWindow;
					/* 0x0032 */ bool vsync;
					/* 0x0033 */ std::byte _pad0033[0xD];
					/* 0x0040 */ std::uint32_t presentInterval;
					/* 0x0044 */ std::byte _pad0044[4];
					/* 0x0048 */ ID3D11Device* device;
					/* 0x0050 */ ID3D11DeviceContext* context;
					/* 0x0058 */ RendererWindow renderWindows[32];
					// Resource arrays use physical IDs, not logical target enums.
					/* 0x0A58 */ RenderTarget renderTargets[101];
					/* 0x1D48 */ DepthStencilTarget depthStencilTargets[13];
					/* 0x2500 */ CubeMapRenderTarget cubeMapRenderTargets[2];
					/* 0x2580 */ std::byte _pad2580[0x38];
					/* 0x25B8 */ const char* className;
					/* 0x25C0 */ HINSTANCE instance;
					/* 0x25C8 */ bool nvapiEnabled;
					/* 0x25C9 */ std::byte _pad25C9[7];
				};
				static_assert(sizeof(RendererData) == 0x25D0);
				static_assert(offsetof(RendererData, device) == 0x48);
				static_assert(offsetof(RendererData, context) == 0x50);
				static_assert(offsetof(RendererData, renderWindows) == 0x58);
				static_assert(offsetof(RendererData, renderTargets) == 0xA58);
				static_assert(offsetof(RendererData, depthStencilTargets) == 0x1D48);
				static_assert(offsetof(RendererData, cubeMapRenderTargets) == 0x2500);
				static_assert(offsetof(RendererData, className) == 0x25B8);

				struct AdapterInfo
				{
					/* 0x000 */ char description[64];
					/* 0x040 */ std::uint32_t deviceID;
					/* 0x044 */ std::uint32_t vendorID;
					/* 0x048 */ std::uint32_t subSystemID;
					/* 0x04C */ std::uint32_t revision;
					/* 0x050 */ std::uint32_t dedicatedVideoMemoryMB;
					/* 0x054 */ std::uint32_t dedicatedSystemMemoryMB;
				};
				static_assert(sizeof(AdapterInfo) == 0x58);
				static_assert(offsetof(AdapterInfo, deviceID) == 0x40);
				static_assert(offsetof(AdapterInfo, dedicatedVideoMemoryMB) == 0x50);

				// Common prefix; CK 1.11.137+ appends AdapterInfo at 0x25E0.
				struct Renderer
				{
					/* 0x0000 */ std::byte _pad0000[0x10];
					/* 0x0010 */ RendererData data;

					[[nodiscard]] static Renderer* GetSingleton() noexcept(true);
					// Adapter information was appended in CK 1.11.137.
					[[nodiscard]] const AdapterInfo* GetAdapterInfo() const noexcept(true);
				};
				static_assert(sizeof(Renderer) == 0x25E0);
				static_assert(offsetof(Renderer, data) == 0x10);

				[[nodiscard]] RendererData* GetRendererData() noexcept(true);
				[[nodiscard]] RendererWindow* GetCurrentRendererWindow() noexcept(true);
			}
		}
	}
}
