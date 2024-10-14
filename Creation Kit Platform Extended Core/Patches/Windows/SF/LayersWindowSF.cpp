// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "UITheme/VarCommon.h"
#include "LayersWindowSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
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
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool LayersWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc = voltek::detours_function_class_jump(_RELDATA_ADDR(0), (uintptr_t)&HKWndProc);
					// Layers Window enable doublebuffered treeview control
					lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&sub);
					// Remove set color
					if (UITheme::IsDarkTheme()) lpRelocator->PatchNop(_RELDATA_RAV(2), 0x29);
					// Layers dialog fix resize
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&MoveWindowBody);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&MoveWindowHeader);
					
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
						GlobalLayersWindowPtr->PushbuttonMinus = GetDlgItem(Hwnd, 5594);
						GlobalLayersWindowPtr->Trackbar = GetDlgItem(Hwnd, 7121);
						GlobalLayersWindowPtr->CheckboxMaintain = GetDlgItem(Hwnd, 41584);
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
						auto pbw_2 = 24;

						GlobalLayersWindowPtr->PushbuttonNewLayer.LockUpdate();
						GlobalLayersWindowPtr->PushbuttonAddSelection.LockUpdate();
						GlobalLayersWindowPtr->PushbuttonMinus.LockUpdate();
						GlobalLayersWindowPtr->PushbuttonPlus.LockUpdate();
						GlobalLayersWindowPtr->Trackbar.LockUpdate();

						GlobalLayersWindowPtr->PushbuttonNewLayer.Move(8, 8);
						GlobalLayersWindowPtr->PushbuttonNewLayer.Width = pbw_1;

						GlobalLayersWindowPtr->PushbuttonAddSelection.Move(16 + pbw_1, 8);
						GlobalLayersWindowPtr->PushbuttonAddSelection.Width = pbw_1;

						GlobalLayersWindowPtr->PushbuttonMinus.Left = width - pbw_2 - 8;
						GlobalLayersWindowPtr->PushbuttonMinus.Height = GlobalLayersWindowPtr->EdittextFilter.Height;
						GlobalLayersWindowPtr->PushbuttonMinus.Width = pbw_2;
						GlobalLayersWindowPtr->PushbuttonPlus.Left = width - (pbw_2 << 1) - 16;
						GlobalLayersWindowPtr->PushbuttonPlus.Width = pbw_2;
						GlobalLayersWindowPtr->PushbuttonPlus.Height = GlobalLayersWindowPtr->PushbuttonMinus.Height;

						GlobalLayersWindowPtr->EdittextFilter.Width = (GlobalLayersWindowPtr->PushbuttonPlus.Left - 8) - 
							GlobalLayersWindowPtr->EdittextFilter.Left;

						GlobalLayersWindowPtr->Trackbar.Width = width - GlobalLayersWindowPtr->Trackbar.Left - 4;

						GlobalLayersWindowPtr->Trackbar.UnlockUpdate();
						GlobalLayersWindowPtr->PushbuttonPlus.UnlockUpdate();
						GlobalLayersWindowPtr->PushbuttonMinus.UnlockUpdate();
						GlobalLayersWindowPtr->PushbuttonAddSelection.UnlockUpdate();
						GlobalLayersWindowPtr->PushbuttonNewLayer.UnlockUpdate();

						GlobalLayersWindowPtr->Trackbar.Repaint();
						GlobalLayersWindowPtr->PushbuttonPlus.Repaint();
						GlobalLayersWindowPtr->PushbuttonMinus.Repaint();
						GlobalLayersWindowPtr->PushbuttonAddSelection.Repaint();
						GlobalLayersWindowPtr->PushbuttonNewLayer.Repaint();
					}
					break;
				}

				return CallWindowProc(GlobalLayersWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			LRESULT LayersWindow::sub(HWND TVHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				auto iRes = EditorAPI::EditorUI::HKSendMessageA(TVHwnd, Message, wParam, lParam);

				GlobalLayersWindowPtr->TreeView = TVHwnd;

				if (UITheme::IsDarkTheme())
				{
					TreeView_SetTextColor(TVHwnd, 0);
					TreeView_SetBkColor(TVHwnd, RGB(255, 255, 255));
				}

				TreeView_SetExtendedStyle(TVHwnd, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

				return iRes;
			}

			bool LayersWindow::MoveWindowBody(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint)
			{
				auto cRect = GlobalLayersWindowPtr->ClientRect();

				return MoveWindow(hWnd, 8, Y, cRect.Width - 16, cRect.Height - Y - 8, bRepaint);
			}

			bool LayersWindow::MoveWindowHeader(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint)
			{
				auto cRect = GlobalLayersWindowPtr->ClientRect();
				auto cBounds = GlobalLayersWindowPtr->CheckboxMaintain.GetBoundsRect();

				return MoveWindow(hWnd, 8, cBounds.Bottom + 12, cRect.Width - 16, nHeight + 4, bRepaint);
			}
		}
	}
}