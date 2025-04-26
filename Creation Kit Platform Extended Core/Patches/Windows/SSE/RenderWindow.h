// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BaseWindow.h"
#include <Core/ImagespaceAA.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			class RenderWindow : public BaseWindow, public Classes::CUIBaseWindow
			{
			public:
				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				RenderWindow();
				
				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void DrawFrameEx(IDXGISwapChain* This, UINT SyncInterval, UINT Flags);
				static void ImGuiDrawInfo();
				static void setFlagLoadCell();

				inline ImagespaceAA* GetImagespaceAA() const noexcept(true) { return _ImagespaceAA.get(); }
				virtual bool HasAntiAliasing() const noexcept(true) { return _ImagespaceAA ? _ImagespaceAA->IsEnabled() : false; }
				virtual void SetAntiAliasingEnabled(bool v) noexcept(true) { if (_ImagespaceAA) _ImagespaceAA->SetEnabled(v); }
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				bool _BlockInputMessage;
				std::unique_ptr<ImagespaceAA> _ImagespaceAA;
			};

			extern RenderWindow* GlobalRenderWindowPtr;
		}
	}
}