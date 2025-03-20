// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BaseWindow.h"
#include "Editor API/FO4/TESF4.h"
#include <Core/ImagespaceAA.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
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
				static void UpdateDrawInfo(char* Dest, UINT Size, const char* FormatStr, ...);
				static void ImGuiDrawInfo();

				virtual bool HasAntiAliasing() const noexcept(true) { return _ImagespaceAA->IsEnabled(); }
				virtual void SetAntiAliasingEnabled(bool v) noexcept(true) { _ImagespaceAA->SetEnabled(v); }
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			public:
				struct Area
				{
					SIZE WindowSize;
					SIZE WindowSize2;	// I don't know why, but they are the same

					inline Area& operator=(const Area& a)
					{
						WindowSize = a.WindowSize;
						WindowSize2 = a.WindowSize2;

						return *this;
					}
				};

				inline static Area* _TempDrawArea;
			private:
				std::unique_ptr<ImagespaceAA> _ImagespaceAA;
			};

			extern RenderWindow* GlobalRenderWindowPtr;
		}
	}
}