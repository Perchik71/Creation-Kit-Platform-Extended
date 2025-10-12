// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>

#include <CKPE.SafeWrite.h>
#include <CKPE.Detours.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.LayersWindow.h>

#include <commctrl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			LayersWindow::LayersWindow() : Common::PatchBaseWindow()
			{
				SetName("Layers Window");
				Singleton = this;
			}

			bool LayersWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* LayersWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool LayersWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> LayersWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool LayersWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool LayersWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				// Layers Window enable doublebuffered treeview control
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				// Layers dialog fix resize
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&MoveWindowBody);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&MoveWindowHeader);

				return true;
			}

			INT_PTR CALLBACK LayersWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						LayersWindow::Singleton->m_hWnd = Hwnd;
						LayersWindow::Singleton->PushbuttonNewLayer = GetDlgItem(Hwnd, 1124);
						LayersWindow::Singleton->PushbuttonAddSelection = GetDlgItem(Hwnd, 3682);
						LayersWindow::Singleton->EdittextFilter = GetDlgItem(Hwnd, 2581);
						LayersWindow::Singleton->PushbuttonPlus = GetDlgItem(Hwnd, 5593);
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

						LayersWindow::Singleton->PushbuttonNewLayer.Move(8, 8);
						LayersWindow::Singleton->PushbuttonNewLayer.Width = pbw_1;

						LayersWindow::Singleton->PushbuttonAddSelection.Move(16 + pbw_1, 8);
						LayersWindow::Singleton->PushbuttonAddSelection.Width = pbw_1;

						LayersWindow::Singleton->EdittextFilter.Width = width -
							(LayersWindow::Singleton->EdittextFilter.Left + 8);
					}
					break;
				}

				return CallWindowProc(LayersWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			LRESULT LayersWindow::sub(HWND TVHwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				auto iRes = Common::EditorUI::Hook::HKSendMessageA(TVHwnd, Message, wParam, lParam);

				SetWindowLongPtrA(TVHwnd, GWL_STYLE, GetWindowLongPtrA(TVHwnd, GWL_STYLE) | WS_BORDER);
				TreeView_SetExtendedStyle(TVHwnd, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

				return iRes;
			}

			bool LayersWindow::MoveWindowBody(HWND hWnd, std::int32_t X, std::int32_t Y, std::int32_t nWidth, 
				std::int32_t nHeight, bool bRepaint) noexcept(true)
			{
				auto cRect = LayersWindow::Singleton->ClientRect();

				return MoveWindow(hWnd, 8, Y - 8, cRect.Width - 16, cRect.Height - Y, bRepaint);
			}

			bool LayersWindow::MoveWindowHeader(HWND hWnd, std::int32_t X, std::int32_t Y, std::int32_t nWidth,
				std::int32_t nHeight, bool bRepaint) noexcept(true)
			{
				auto cRect = LayersWindow::Singleton->ClientRect();
				auto cBounds = LayersWindow::Singleton->PushbuttonPlus.BoundsRect;

				return MoveWindow(hWnd, 8, cBounds.Bottom + 8, cRect.Width - 16, nHeight, bRepaint);
			}
		}
	}
}