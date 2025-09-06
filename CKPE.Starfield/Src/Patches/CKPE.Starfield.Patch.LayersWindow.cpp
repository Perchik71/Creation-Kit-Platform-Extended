// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.LayersWindow.h>

#include <commctrl.h>

namespace CKPE
{
	namespace Starfield
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
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool LayersWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);
				// Layers Window enable doublebuffered treeview control
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				// Remove set color
				if (Common::UI::IsDarkTheme()) SafeWrite::WriteNop(__CKPE_OFFSET(2), 0x29);
				// Layers dialog fix resize
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&MoveWindowBody);
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&MoveWindowHeader);

				return true;
			}

			LRESULT CALLBACK LayersWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						auto s = LayersWindow::Singleton.GetSingleton();

						s->m_hWnd = Hwnd;
						s->PushbuttonNewLayer = GetDlgItem(Hwnd, 1124);
						s->PushbuttonAddSelection = GetDlgItem(Hwnd, 3682);
						s->EdittextFilter = GetDlgItem(Hwnd, 2581);
						s->PushbuttonPlus = GetDlgItem(Hwnd, 5593);
						s->PushbuttonMinus = GetDlgItem(Hwnd, 5594);
						s->Trackbar = GetDlgItem(Hwnd, 7121);
						s->CheckboxMaintain = GetDlgItem(Hwnd, 41584);
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

						auto s = LayersWindow::Singleton.GetSingleton();

						s->PushbuttonNewLayer.LockUpdate();
						s->PushbuttonAddSelection.LockUpdate();
						s->PushbuttonMinus.LockUpdate();
						s->PushbuttonPlus.LockUpdate();
						s->Trackbar.LockUpdate();

						s->PushbuttonNewLayer.Move(8, 8);
						s->PushbuttonNewLayer.Width = pbw_1;

						s->PushbuttonAddSelection.Move(16 + pbw_1, 8);
						s->PushbuttonAddSelection.Width = pbw_1;

						s->PushbuttonMinus.Left = width - pbw_2 - 8;
						s->PushbuttonMinus.Height = s->EdittextFilter.Height;
						s->PushbuttonMinus.Width = pbw_2;
						s->PushbuttonPlus.Left = width - (pbw_2 << 1) - 16;
						s->PushbuttonPlus.Width = pbw_2;
						s->PushbuttonPlus.Height = s->PushbuttonMinus.Height;

						s->EdittextFilter.Width = (s->PushbuttonPlus.Left - 8) -
							s->EdittextFilter.Left;

						s->Trackbar.Width = width - s->Trackbar.Left - 4;

						s->Trackbar.UnlockUpdate();
						s->PushbuttonPlus.UnlockUpdate();
						s->PushbuttonMinus.UnlockUpdate();
						s->PushbuttonAddSelection.UnlockUpdate();
						s->PushbuttonNewLayer.UnlockUpdate();

						s->Trackbar.Repaint();
						s->PushbuttonPlus.Repaint();
						s->PushbuttonMinus.Repaint();
						s->PushbuttonAddSelection.Repaint();
						s->PushbuttonNewLayer.Repaint();
					}
					break;
				}

				return CallWindowProc(LayersWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			LRESULT LayersWindow::sub(HWND TVHwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				auto iRes = Common::EditorUI::Hook::HKSendMessageA(TVHwnd, Message, wParam, lParam);

				LayersWindow::Singleton->TreeView = TVHwnd;

				if (Common::UI::IsDarkTheme())
				{
					TreeView_SetTextColor(TVHwnd, 0);
					TreeView_SetBkColor(TVHwnd, RGB(255, 255, 255));
				}

				TreeView_SetExtendedStyle(TVHwnd, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

				return iRes;
			}

			bool LayersWindow::MoveWindowBody(HWND hWnd, std::int32_t X, std::int32_t Y, std::int32_t nWidth, 
				std::int32_t nHeight, bool bRepaint) noexcept(true)
			{
				auto cRect = LayersWindow::Singleton->ClientRect();

				return MoveWindow(hWnd, 8, Y, cRect.Width - 16, cRect.Height - Y - 8, bRepaint);
			}

			bool LayersWindow::MoveWindowHeader(HWND hWnd, std::int32_t X, std::int32_t Y, std::int32_t nWidth,
				std::int32_t nHeight, bool bRepaint) noexcept(true)
			{
				auto cRect = LayersWindow::Singleton->ClientRect();
				auto cBounds = LayersWindow::Singleton->CheckboxMaintain.GetBoundsRect();

				return MoveWindow(hWnd, 8, cBounds.Bottom + 12, cRect.Width - 16, nHeight + 4, bRepaint);
			}
		}
	}
}