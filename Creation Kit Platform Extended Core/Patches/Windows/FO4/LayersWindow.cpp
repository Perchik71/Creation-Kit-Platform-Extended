// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "LayersWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			LayersWindow* GlobalLayersWindowPtr = nullptr;

			bool LayersWindow::HasOption() const
			{
				return false;
			}

			bool LayersWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* LayersWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* LayersWindow::GetName() const
			{
				return "Layers Window";
			}

			bool LayersWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> LayersWindow::GetDependencies() const
			{
				return {};
			}

			bool LayersWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool LayersWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), 
							(uintptr_t)&HKWndProc);

					// Layers Window enable doublebuffered treeview control
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					// Layers dialog fix resize
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&MoveWindowBody);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&MoveWindowHeader);
					
					return true;
				}

				return false;
			}

			bool LayersWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			LayersWindow::LayersWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalLayersWindowPtr);
				GlobalLayersWindowPtr = this;
			}

			LRESULT CALLBACK LayersWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						GlobalLayersWindowPtr->m_hWnd = Hwnd;
						GlobalLayersWindowPtr->PushbuttonNewLayer = GetDlgItem(Hwnd, 1124);
						GlobalLayersWindowPtr->PushbuttonAddSelection = GetDlgItem(Hwnd, 3682);
						GlobalLayersWindowPtr->EdittextFilter = GetDlgItem(Hwnd, 2581);
						GlobalLayersWindowPtr->PushbuttonPlus = GetDlgItem(Hwnd, 5593);
					}
					break;
					case WM_GETMINMAXINFO: 
					{
						LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
						lpMMI->ptMinTrackSize.x = 280;
						lpMMI->ptMinTrackSize.y = 150;
					}
					return S_OK;
					case WM_SIZE: 
					{
						UINT width = LOWORD(lParam);
						UINT height = HIWORD(lParam);

						auto pbw_1 = (width - 24) >> 1;

						GlobalLayersWindowPtr->PushbuttonNewLayer.Move(8, 8);
						GlobalLayersWindowPtr->PushbuttonNewLayer.Width = pbw_1;

						GlobalLayersWindowPtr->PushbuttonAddSelection.Move(16 + pbw_1, 8);
						GlobalLayersWindowPtr->PushbuttonAddSelection.Width = pbw_1;

						GlobalLayersWindowPtr->EdittextFilter.Width = width - 
							(GlobalLayersWindowPtr->EdittextFilter.Left + 8);
					}
					break;
				}

				return CallWindowProc(GlobalLayersWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			LRESULT LayersWindow::sub(HWND TVHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				auto iRes = EditorAPI::EditorUI::HKSendMessageA(TVHwnd, Message, wParam, lParam);

				SetWindowLongPtrA(TVHwnd, GWL_STYLE, GetWindowLongPtrA(TVHwnd, GWL_STYLE) | WS_BORDER);
				TreeView_SetExtendedStyle(TVHwnd, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

				return iRes;
			}

			bool LayersWindow::MoveWindowBody(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint)
			{
				auto cRect = GlobalLayersWindowPtr->ClientRect();

				return MoveWindow(hWnd, 8, Y - 8, cRect.Width - 16, cRect.Height - Y, bRepaint);
			}

			bool LayersWindow::MoveWindowHeader(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint)
			{
				auto cRect = GlobalLayersWindowPtr->ClientRect();
				auto cBounds = GlobalLayersWindowPtr->PushbuttonPlus.BoundsRect;

				return MoveWindow(hWnd, 8, cBounds.Bottom + 8, cRect.Width - 16, nHeight, bRepaint);
			}
		}
	}
}