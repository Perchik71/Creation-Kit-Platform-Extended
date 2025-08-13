// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESObjectREFR.h>
#include <CKPE.Singleton.h>
#include <CKPE.ImageList.h>
#include <CKPE.Common.PatchBaseWindow.h>
#include <CKPE.SkyrimSE.D3D11ImagespaceAA.h>
#include <memory>
#include <dxgi.h>
#include <d3d11_2.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class RenderWindow : public Common::PatchBaseWindow
			{
				bool _BlockInputMessage{ false };
				std::unique_ptr<D3D11ImagespaceAA> _ImagespaceAA;

				RenderWindow(const RenderWindow&) = delete;
				RenderWindow& operator=(const RenderWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				RenderWindow();

				inline static ISingleton<RenderWindow> Singleton;
				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void DrawFrameEx(IDXGISwapChain* This, UINT SyncInterval, UINT Flags);
				static void setFlagLoadCell() noexcept(true);
				static void ImGuiDrawInfo() noexcept(true);

				inline D3D11ImagespaceAA* GetImagespaceAA() const noexcept(true) { return _ImagespaceAA.get(); }
				virtual bool HasAntiAliasing() const noexcept(true) { return _ImagespaceAA ? _ImagespaceAA->IsEnabled() : false; }
				virtual void SetAntiAliasingEnabled(bool v) noexcept(true) { if (_ImagespaceAA) _ImagespaceAA->SetEnabled(v); }

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}