//////////////////////////////////////////
/*
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "UIBaseWindow.h"
#include "..\EditorUI.h"

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			using namespace CreationKitPlatformExtended::EditorAPI;

			static std::map<HWND, CRECT> SafeSize;

			// CUIBaseWindow

			BOOL CUIBaseWindow::IsWindowMaximized(void) const
			{
				WINDOWPLACEMENT placement = { 0 };
				placement.length = sizeof(WINDOWPLACEMENT);
				if (GetWindowPlacement(m_hWnd, &placement)) {
					return placement.showCmd == SW_SHOWMAXIMIZED;
				}
				return FALSE;
			}

			UINT CUIBaseWindow::DpiForWindow(void) const
			{
				return GetDpiForWindow(m_hWnd);
			}

			void CUIBaseWindow::LockUpdate(void)
			{
				if (m_LockUpdate)
					return;

				m_LockUpdate = TRUE;
				SendMessageA(m_hWnd, WM_SETREDRAW, FALSE, 0);
			}

			void CUIBaseWindow::UnlockUpdate(void)
			{
				if (!m_LockUpdate)
					return;

				m_LockUpdate = FALSE;
				SendMessageA(m_hWnd, WM_SETREDRAW, TRUE, 0);
			}

			void CUIBaseWindow::Move(const LONG x, const LONG y, const BOOL topmost)
			{
				SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | ((topmost) ? 0 : SWP_NOZORDER));
			}

			void CUIBaseWindow::Invalidate(void)
			{
				InvalidateRect(m_hWnd, NULL, FALSE);
			}

			void CUIBaseWindow::Invalidate(const LPRECT r)
			{
				InvalidateRect(m_hWnd, r, FALSE);
			}

			void CUIBaseWindow::Invalidate(const CRECT& r)
			{
				InvalidateRect(m_hWnd, (LPRECT)&r, FALSE);
			}

			void CUIBaseWindow::Refresh(void)
			{
				InvalidateRect(m_hWnd, NULL, TRUE);
				UpdateWindow(m_hWnd);
			}

			void CUIBaseWindow::SetParent(HWND hParent)
			{
				::SetParent(m_hWnd, hParent);
			}

			void CUIBaseWindow::SetParent(const CUIBaseWindow& Parent)
			{
				::SetParent(m_hWnd, Parent.Handle);
			}

			void CUIBaseWindow::Repaint(void)
			{
				RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW);
			}

			BOOL CUIBaseWindow::GetVisible(void)
			{
				return IsWindowVisible(m_hWnd);
			}

			BOOL CUIBaseWindow::GetConstVisible(void) const
			{
				return IsWindowVisible(m_hWnd);
			}

			void CUIBaseWindow::SetSize(const LONG cx, const LONG cy, const BOOL topmost)
			{
				CRECT r = BoundsRect;
				SetWindowPos(m_hWnd, NULL, 0, 0, cx + r.Left, cy + r.Top, SWP_NOOWNERZORDER | SWP_NOMOVE | ((topmost) ? 0 : SWP_NOZORDER | SWP_FRAMECHANGED));
			}

			void CUIBaseWindow::SetFocus(void)
			{
				::SetFocus(m_hWnd);
			}

			void CUIBaseWindow::SetVisible(const BOOL value)
			{
				if (value != Visible)
					WindowState = (value) ? wsNormal : wsHide;
			}

			std::string CUIBaseWindow::GetCaption(void) const
			{
				std::string s;
				INT nLen = GetWindowTextLengthA(m_hWnd) + 1;

				if (!nLen)
					return "";

				s.resize(nLen);
				GetWindowTextA(m_hWnd, &s[0], MAX_PATH);

				return s;
			}

			void CUIBaseWindow::SetCaption(const std::string &value)
			{
				SetWindowTextA(m_hWnd, value.c_str());
			}

			std::wstring CUIBaseWindow::GetWideCaption(void) const
			{
				std::wstring s;
				INT nLen = GetWindowTextLengthW(m_hWnd) + 1;

				if (!nLen)
					return L"";

				s.resize(nLen);
				GetWindowTextW(m_hWnd, &s[0], MAX_PATH);

				return s;
			}

			void CUIBaseWindow::SetWideCaption(const std::wstring &value)
			{
				SetWindowTextW(m_hWnd, value.c_str());
			}

			ULONG_PTR CUIBaseWindow::GetStyle(void) const
			{
				return GetWindowLongA(m_hWnd, GWL_STYLE);
			}

			void CUIBaseWindow::SetStyle(const ULONG_PTR value)
			{
				SetWindowLongA(m_hWnd, GWL_STYLE, value);
			}

			ULONG_PTR CUIBaseWindow::GetStyleEx(void) const
			{
				return GetWindowLongA(m_hWnd, GWL_EXSTYLE);
			}

			void CUIBaseWindow::SetStyleEx(const ULONG_PTR value)
			{
				SetWindowLongA(m_hWnd, GWL_EXSTYLE, value);
			}

			BOOL CUIBaseWindow::GetEnabled(void) const
			{
				return IsWindowEnabled(m_hWnd);
			}

			void CUIBaseWindow::SetEnabled(const BOOL value)
			{
				if (value != Enabled)
					EnableWindow(m_hWnd, value);
			}

			std::string CUIBaseWindow::GetName(void) const
			{
				std::string s;
				s.resize(MAX_PATH);
				INT nLen = GetClassNameA(m_hWnd, &s[0], MAX_PATH);
				if (nLen)
					s.resize(nLen);
				else
					s = "";

				return s;
			}

			std::wstring CUIBaseWindow::GetWideName(void) const
			{
				std::wstring s;
				s.resize(MAX_PATH);
				INT nLen = GetClassNameW(m_hWnd, &s[0], MAX_PATH);
				if (nLen)
					s.resize(nLen);
				else
					s = L"";

				return s;
			}

			WindowState_t CUIBaseWindow::GetWindowState(void) const
			{ 
				LONG style = GetWindowLongA(m_hWnd, GWL_STYLE);

				if (!IsWindowVisible(m_hWnd))
				{
					return wsHide;
				}
				else
				{
					if ((style & WS_POPUP) == WS_POPUP)
					{
						CRECT WorkArea, WndRect;
						SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0);
						WndRect = WindowRect();

						if (WorkArea == WndRect)
						{
							return wsMaximized;
						}
						else if ((style & WS_MINIMIZE) == WS_MINIMIZE)
						{
							return wsMinimized;
						}
						else
						{
							return wsNormal;
						}
					}
					else
					{
						if ((style & WS_MAXIMIZE) == WS_MAXIMIZE)
						{
							return wsMaximized;
						}
						else if ((style & WS_MINIMIZE) == WS_MINIMIZE)
						{
							return wsMinimized;
						}
						else
						{
							return wsNormal;
						}
					}
				}
			}

			void CUIBaseWindow::SetWindowState(const WindowState_t state)
			{
				if (WindowState == state)
					return;

				int flag = SW_NORMAL;

				LONG style = GetWindowLongA(m_hWnd, GWL_STYLE);

				if ((style & WS_POPUP) == WS_POPUP)
				{
					if (state == wsMaximized)
					{
						CRECT WorkArea, WndRect;
						SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0);
						WndRect = WindowRect();

						SafeSize.emplace(m_hWnd, WndRect);

						SetWindowPos(m_hWnd, NULL, WorkArea.Left, WorkArea.Top, WorkArea.Right, WorkArea.Bottom,
							SWP_NOOWNERZORDER | ((StyleEx & WS_EX_TOPMOST) == WS_EX_TOPMOST ? 0 : SWP_NOZORDER) | SWP_FRAMECHANGED);
						Perform(UI_CHANGEWINDOWSTATE, (WPARAM)state, 0);
						return;
					}
					else if (wsMinimized == state)
					{
						flag = SW_MINIMIZE;
					}
					else if (wsHide == state)
					{
						flag = SW_HIDE;
					}
					else
					{
						if ((WindowState == wsMaximized) && (SafeSize.count(m_hWnd) > 0))
						{
							CRECT WndRect = SafeSize.at(m_hWnd);
							SafeSize.erase(m_hWnd);

							SetWindowPos(m_hWnd, NULL, WndRect.Left, WndRect.Top, WndRect.Right, WndRect.Bottom,
								SWP_NOOWNERZORDER | ((StyleEx & WS_EX_TOPMOST) == WS_EX_TOPMOST ? 0 : SWP_NOZORDER) | SWP_FRAMECHANGED);
							Perform(UI_CHANGEWINDOWSTATE, (WPARAM)state, 0);
							return;
						}
					}

					ShowWindow(m_hWnd, flag);
					Perform(UI_CHANGEWINDOWSTATE, (WPARAM)state, 0);
				}
				else
				{
					switch (state)
					{
					case wsMaximized:
					{
						flag = SW_MAXIMIZE;
						break;
					}
					case wsMinimized:
					{
						flag = SW_MINIMIZE;
						break;
					}
					case wsHide:
					{
						flag = SW_HIDE;
						break;
					}
					}

					ShowWindow(m_hWnd, flag);
					Perform(UI_CHANGEWINDOWSTATE, (WPARAM)state, 0);
				}
			}

			CRECT CUIBaseWindow::WindowRect(void) const
			{
				RECT r;
				GetWindowRect(m_hWnd, &r);
				return r;
			}

			CRECT CUIBaseWindow::ClientRect(void) const
			{
				RECT r;
				GetClientRect(m_hWnd, &r);
				return r;
			}

			LONG CUIBaseWindow::ClientWidth(void) const
			{
				return ClientRect().Right;
			}

			LONG CUIBaseWindow::ClientHeight(void) const
			{
				return ClientRect().Bottom;
			}

			BOOL CUIBaseWindow::Is(void) const
			{
				return m_hWnd && IsWindow(m_hWnd);
			}

			HWND CUIBaseWindow::Parent(void) const
			{
				return GetParent(m_hWnd);
			}

			CUIBaseWindow CUIBaseWindow::ParentWindow(void) const
			{
				return CUIBaseWindow(Parent());
			}

			POINT CUIBaseWindow::ScreenToControl(const POINT &p) const
			{
				if (!Is())
					return POINT{ 0 };

				POINT pnt = p;

				return (ScreenToClient(m_hWnd, &pnt)) ? pnt : POINT{ 0 };
			}

			POINT CUIBaseWindow::ControlToScreen(const POINT &p) const
			{
				if (!Is())
					return POINT{ 0 };

				POINT pnt = p;

				return (ClientToScreen(m_hWnd, &pnt)) ? pnt : POINT{ 0 };
			}

			CRECT CUIBaseWindow::GetBoundsRect(void) const
			{
				CUIBaseWindow parent = ParentWindow();

				if (!parent.Is())
					return WindowRect();

				CRECT wrect = WindowRect();
				POINT Off = parent.ScreenToControl({ wrect.Left, wrect.Top });

				wrect.Left = Off.x;
				wrect.Top = Off.y;

				return wrect;
			}

			void CUIBaseWindow::SetBoundsRect(const CRECT &bounds)
			{
				MoveWindow(m_hWnd, bounds.Left, bounds.Top, bounds.Width, bounds.Height, TRUE);
			}

			LONG CUIBaseWindow::GetLeft(void) const
			{
				return BoundsRect.Left;
			}

			void CUIBaseWindow::SetLeft(const LONG value)
			{
				CRECT bounds = BoundsRect;
				LONG w = bounds.Width;
				bounds.Left = value;
				bounds.Width = w;
				BoundsRect = bounds;
			}

			LONG CUIBaseWindow::GetTop(void) const
			{
				return BoundsRect.Top;
			}

			void CUIBaseWindow::SetTop(const LONG value)
			{
				CRECT bounds = BoundsRect;
				LONG h = bounds.Height;
				bounds.Top = value;
				bounds.Height = h;
				BoundsRect = bounds;
			}

			LONG CUIBaseWindow::GetWidth(void) const
			{
				return BoundsRect.Width;
			}

			void CUIBaseWindow::SetWidth(const LONG value)
			{
				CRECT bounds = BoundsRect;
				bounds.Width = value;
				BoundsRect = bounds;
			}

			LONG CUIBaseWindow::GetHeight(void) const
			{
				return BoundsRect.Height;
			}

			void CUIBaseWindow::SetHeight(const LONG value)
			{
				CRECT bounds = BoundsRect;
				bounds.Height = value;
				BoundsRect = bounds;
			}

			LRESULT CUIBaseWindow::Perform(UINT uMsg, WPARAM wParam, LPARAM lParam)
			{ 
				return Perform(m_hWnd, uMsg, wParam, lParam); 
			}

			LRESULT CUIBaseWindow::Perform(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{ 
				return EditorUI::HKSendMessageA(hWnd, uMsg, wParam, lParam);
			}

			LRESULT CUIBaseWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			}

			// CUICustomWindow

			CUIBaseControl CUICustomWindow::GetControl(const uint32_t index)
			{
				HWND hWnd = GetDlgItem(Handle, index);
				AssertMsgVa(hWnd, "control %d no found", index);

				return CUIBaseControl(hWnd);
			}

			void CUICustomWindow::Foreground(void)
			{
				SetForegroundWindow(m_hWnd);
			}

			// CUIDialog

			CUICustomDialog::CUICustomDialog(CUICustomWindow* parent, const UINT res_id, DLGPROC dlg_proc) : CUICustomWindow()
			{
				m_ResId = res_id;
				m_DlgProc = dlg_proc;
				m_Parent = parent;
			}

			void CUICustomDialog::Create(void)
			{
				// fix crashes dark theme
			//	CreateDialogParamA(GetModuleHandle(NULL), MAKEINTRESOURCEA(m_ResId), m_Parent->Handle, m_DlgProc, (LONG_PTR)this);
				EditorUI::HKCreateDialogParamA(GetModuleHandle(NULL), MAKEINTRESOURCEA(m_ResId), m_Parent->Handle, 
					m_DlgProc, (LONG_PTR)this);
				Assert(m_hWnd);
				ShowWindow(m_hWnd, SW_SHOW);
				UpdateWindow(m_hWnd);
			}

			void CUICustomDialog::FreeRelease(void)
			{
				if (!m_hWnd) return;
				DestroyWindow(m_hWnd);
				m_hWnd = NULL;
			};

			CUICustomDialog::~CUICustomDialog(void)
			{
				FreeRelease();
			}

			// CUIBaseControl

			WORD CUIBaseControl::GetID() const
			{
				return GetDlgCtrlID(m_hWnd);
			}

			// CUIMainWindow

			void CUIMainWindow::FindToolWindow(void)
			{
				EnumChildWindows(Handle, [](HWND hwnd, LPARAM lParam) -> BOOL {
					CUIMainWindow* main = (CUIMainWindow*)lParam;
					CUIToolWindow Tool(hwnd);

					// For some reason, only the standard comparison function finds it...

					if (!stricmp(Tool.Name.c_str(), TOOLBARCLASSNAMEA))
						main->Toolbar = Tool;
					else if (!stricmp(Tool.Name.c_str(), STATUSCLASSNAMEA))
						main->Statusbar = Tool;
					return TRUE;
				}, (LPARAM)this);
			}

			void CUIMainWindow::ProcessMessages(void)
			{
				MSG msg;
				while (true)
				{
					if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
					{
						DispatchMessageA(&msg);
						TranslateMessage(&msg);
					}
					else
						break;
				}
			}

			INT32 CUIMainWindow::MessageDlg(const std::string message, const std::string caption, const UINT32 flags)
			{
				return MessageBoxA(GetForegroundWindow(), message.c_str(), caption.c_str(), flags);
			}

			INT32 CUIMainWindow::MessageDlg(const std::wstring message, const std::wstring caption, const UINT32 flags)
			{
				return MessageBoxW(GetForegroundWindow(), message.c_str(), caption.c_str(), flags);
			}

			INT32 CUIMainWindow::MessageWarningDlg(const std::string message)
			{
				return MessageDlg(message, "Warning", MB_ICONWARNING);
			}

			INT32 CUIMainWindow::MessageWarningDlg(const std::wstring message)
			{
				return MessageDlg(message, L"Warning", MB_ICONWARNING);
			}

			void CUIMainWindow::SetTextToStatusBar(const uint32_t index, const std::string text)
			{
				Statusbar.Perform(SB_SETTEXTA, index, (LPARAM)text.c_str());
			}

			void CUIMainWindow::SetTextToStatusBar(const uint32_t index, const std::wstring text)
			{
				Statusbar.Perform(SB_SETTEXTW, index, (LPARAM)text.c_str());
			}

			std::string CUIMainWindow::GetTextToStatusBarA(const uint32_t index)
			{
				std::size_t nLen = (std::size_t)Statusbar.Perform(SB_GETTEXTLENGTHA, index, NULL);
				if (nLen > 0)
				{
					std::string s;
					s.resize(++nLen);
					Statusbar.Perform(SB_GETTEXTA, index, (LPARAM)&s[0]);
					
					return s;
				}
				else return "";
			}

			std::wstring CUIMainWindow::GetTextToStatusBarW(const uint32_t index)
			{
				INT nLen = Statusbar.Perform(SB_GETTEXTLENGTHW, index, NULL);
				if (nLen > 0)
				{
					std::wstring s;
					s.resize(++nLen);
					Statusbar.Perform(SB_GETTEXTW, index, (LPARAM)&s[0]);

					return s;
				}
				else return L"";
			}
		}
	}
}


