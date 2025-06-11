// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.ErrorHandler.h>
#include <commctrl.h>
#include <format>
#include <map>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			static std::map<HWND, CRECT> SafeSize;

			// CUIBaseWindow

			bool CUIBaseWindow::IsWindowMaximized() const noexcept(true)
			{
				WINDOWPLACEMENT placement = { 0 };
				placement.length = sizeof(WINDOWPLACEMENT);
				if (GetWindowPlacement(m_hWnd, &placement)) {
					return placement.showCmd == SW_SHOWMAXIMIZED;
				}
				return false;
			}

			UINT CUIBaseWindow::DpiForWindow() const noexcept(true)
			{
				return GetDpiForWindow(m_hWnd);
			}

			void CUIBaseWindow::LockUpdate() noexcept(true)
			{
				if (m_LockUpdate)
					return;

				m_LockUpdate = true;
				SendMessageA(m_hWnd, WM_SETREDRAW, false, 0);
			}

			void CUIBaseWindow::UnlockUpdate() noexcept(true)
			{
				if (!m_LockUpdate)
					return;

				m_LockUpdate = false;
				SendMessageA(m_hWnd, WM_SETREDRAW, true, 0);
			}

			void CUIBaseWindow::Move(const std::int32_t x, const std::int32_t y, const bool topmost) noexcept(true)
			{
				SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | ((topmost) ? 0 : SWP_NOZORDER));
			}

			void CUIBaseWindow::Invalidate() noexcept(true)
			{
				InvalidateRect(m_hWnd, nullptr, false);
			}

			void CUIBaseWindow::Invalidate(const LPRECT r) noexcept(true)
			{
				InvalidateRect(m_hWnd, r, false);
			}

			void CUIBaseWindow::Invalidate(const CRECT& r) noexcept(true)
			{
				InvalidateRect(m_hWnd, (LPRECT)&r, false);
			}

			void CUIBaseWindow::Refresh() noexcept(true)
			{
				InvalidateRect(m_hWnd, nullptr, true);
				UpdateWindow(m_hWnd);
			}

			void CUIBaseWindow::SetParent(HWND hParent) noexcept(true)
			{
				::SetParent(m_hWnd, hParent);
			}

			void CUIBaseWindow::SetParent(const CUIBaseWindow& Parent) noexcept(true)
			{
				::SetParent(m_hWnd, Parent.Handle);
			}

			void CUIBaseWindow::Repaint() noexcept(true)
			{
				RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW);
			}

			bool CUIBaseWindow::GetVisible() noexcept(true)
			{
				return IsWindowVisible(m_hWnd);
			}

			bool CUIBaseWindow::GetConstVisible() const noexcept(true)
			{
				return IsWindowVisible(m_hWnd);
			}

			void CUIBaseWindow::SetSize(const std::int32_t cx, const std::int32_t cy, const bool topmost) noexcept(true)
			{
				CRECT r = BoundsRect;
				SetWindowPos(m_hWnd, nullptr, 0, 0, cx + r.Left, cy + r.Top, SWP_NOOWNERZORDER | SWP_NOMOVE | ((topmost) ? 0 : SWP_NOZORDER | SWP_FRAMECHANGED));
			}

			void CUIBaseWindow::SetFocus() noexcept(true)
			{
				::SetFocus(m_hWnd);
			}

			void CUIBaseWindow::SetVisible(const bool value) noexcept(true)
			{
				if (value != Visible)
					WindowState = (value) ? wsNormal : wsHide;
			}

			std::string CUIBaseWindow::GetCaption() const noexcept(true)
			{
				std::string s;
				INT nLen = GetWindowTextLengthA(m_hWnd) + 1;

				if (!nLen)
					return "";

				s.resize(nLen);
				GetWindowTextA(m_hWnd, &s[0], MAX_PATH);

				return s;
			}

			void CUIBaseWindow::SetCaption(const std::string &value) noexcept(true)
			{
				SetWindowTextA(m_hWnd, value.c_str());
			}

			std::wstring CUIBaseWindow::GetWideCaption() const noexcept(true)
			{
				std::wstring s;
				INT nLen = GetWindowTextLengthW(m_hWnd) + 1;

				if (!nLen)
					return L"";

				s.resize(nLen);
				GetWindowTextW(m_hWnd, &s[0], MAX_PATH);

				return s;
			}

			void CUIBaseWindow::SetWideCaption(const std::wstring &value) noexcept(true)
			{
				SetWindowTextW(m_hWnd, value.c_str());
			}

			std::uintptr_t CUIBaseWindow::GetStyle() const noexcept(true)
			{
				return (std::uintptr_t)GetWindowLongPtrA(m_hWnd, GWL_STYLE);
			}

			void CUIBaseWindow::SetStyle(const std::uintptr_t value) noexcept(true)
			{
				SetWindowLongPtrA(m_hWnd, GWL_STYLE, (LONG_PTR)value);
			}

			std::uintptr_t CUIBaseWindow::GetStyleEx() const noexcept(true)
			{
				return (std::uintptr_t)GetWindowLongPtrA(m_hWnd, GWL_EXSTYLE);
			}

			void CUIBaseWindow::SetStyleEx(const std::uintptr_t value) noexcept(true)
			{
				SetWindowLongPtrA(m_hWnd, GWL_EXSTYLE, (LONG_PTR)value);
			}

			bool CUIBaseWindow::GetEnabled() const noexcept(true)
			{
				return IsWindowEnabled(m_hWnd);
			}

			void CUIBaseWindow::SetEnabled(const bool value) noexcept(true)
			{
				if (value != Enabled)
					EnableWindow(m_hWnd, value);
			}

			std::string CUIBaseWindow::GetName() const noexcept(true)
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

			std::wstring CUIBaseWindow::GetWideName() const noexcept(true)
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

			WindowState_t CUIBaseWindow::GetWindowState() const noexcept(true)
			{ 
				std::int32_t style = GetWindowLongA(m_hWnd, GWL_STYLE);

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

			void CUIBaseWindow::SetWindowState(const WindowState_t state) noexcept(true)
			{
				if (WindowState == state)
					return;

				int flag = SW_NORMAL;

				std::int32_t style = GetWindowLongA(m_hWnd, GWL_STYLE);

				if ((style & WS_POPUP) == WS_POPUP)
				{
					if (state == wsMaximized)
					{
						CRECT WorkArea, WndRect;
						SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0);
						WndRect = WindowRect();

						SafeSize.emplace(m_hWnd, WndRect);

						SetWindowPos(m_hWnd, nullptr, WorkArea.Left, WorkArea.Top, WorkArea.Right, WorkArea.Bottom,
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

							SetWindowPos(m_hWnd, nullptr, WndRect.Left, WndRect.Top, WndRect.Right, WndRect.Bottom,
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

			CRECT CUIBaseWindow::WindowRect() const noexcept(true)
			{
				RECT r;
				GetWindowRect(m_hWnd, &r);
				return r;
			}

			CRECT CUIBaseWindow::ClientRect() const noexcept(true)
			{
				RECT r;
				GetClientRect(m_hWnd, &r);
				return r;
			}

			std::int32_t CUIBaseWindow::ClientWidth() const noexcept(true)
			{
				return ClientRect().Right;
			}

			std::int32_t CUIBaseWindow::ClientHeight() const noexcept(true)
			{
				return ClientRect().Bottom;
			}

			bool CUIBaseWindow::Is() const noexcept(true)
			{
				return m_hWnd && IsWindow(m_hWnd);
			}

			HWND CUIBaseWindow::Parent() const noexcept(true)
			{
				return GetParent(m_hWnd);
			}

			CUIBaseWindow CUIBaseWindow::ParentWindow() const noexcept(true)
			{
				return CUIBaseWindow(Parent());
			}

			POINT CUIBaseWindow::ScreenToControl(const POINT &p) const noexcept(true)
			{
				if (!Is())
					return POINT{ 0 };

				POINT pnt = p;

				return (ScreenToClient(m_hWnd, &pnt)) ? pnt : POINT{ 0 };
			}

			POINT CUIBaseWindow::ControlToScreen(const POINT &p) const noexcept(true)
			{
				if (!Is())
					return POINT{ 0 };

				POINT pnt = p;

				return (ClientToScreen(m_hWnd, &pnt)) ? pnt : POINT{ 0 };
			}

			CRECT CUIBaseWindow::GetBoundsRect() const noexcept(true)
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

			void CUIBaseWindow::SetBoundsRect(const CRECT &bounds) noexcept(true)
			{
				MoveWindow(m_hWnd, bounds.Left, bounds.Top, bounds.Width, bounds.Height, true);
			}

			std::int32_t CUIBaseWindow::GetLeft() const noexcept(true)
			{
				return BoundsRect.Left;
			}

			void CUIBaseWindow::SetLeft(const std::int32_t value) noexcept(true)
			{
				CRECT bounds = BoundsRect;
				std::int32_t w = bounds.Width;
				bounds.Left = value;
				bounds.Width = w;
				BoundsRect = bounds;
			}

			std::int32_t CUIBaseWindow::GetTop() const noexcept(true)
			{
				return BoundsRect.Top;
			}

			void CUIBaseWindow::SetTop(const std::int32_t value) noexcept(true)
			{
				CRECT bounds = BoundsRect;
				std::int32_t h = bounds.Height;
				bounds.Top = value;
				bounds.Height = h;
				BoundsRect = bounds;
			}

			std::int32_t CUIBaseWindow::GetWidth() const noexcept(true)
			{
				return BoundsRect.Width;
			}

			void CUIBaseWindow::SetWidth(const std::int32_t value) noexcept(true)
			{
				CRECT bounds = BoundsRect;
				bounds.Width = value;
				BoundsRect = bounds;
			}

			std::int32_t CUIBaseWindow::GetHeight() const noexcept(true)
			{
				return BoundsRect.Height;
			}

			void CUIBaseWindow::SetHeight(const std::int32_t value) noexcept(true)
			{
				CRECT bounds = BoundsRect;
				bounds.Height = value;
				BoundsRect = bounds;
			}

			LRESULT CUIBaseWindow::Perform(std::uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
			{ 
				return Perform(m_hWnd, uMsg, wParam, lParam); 
			}

			LRESULT CUIBaseWindow::Perform(HWND hWnd, std::uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
			{ 
				return EditorUI::Hook::HKSendMessageA(hWnd, uMsg, wParam, lParam);
			}

			LRESULT CUIBaseWindow::WndProc(HWND hWnd, std::uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			}

			// CUICustomWindow

			CUIBaseControl CUICustomWindow::GetControl(const uint32_t index) noexcept(true)
			{
				HWND hWnd = GetDlgItem(Handle, index);
				if (!hWnd) ErrorHandler::Trigger(std::format("control {:x} no found", index));
				return CUIBaseControl(hWnd);
			}

			void CUICustomWindow::Foreground() noexcept(true)
			{
				SetForegroundWindow(m_hWnd);
			}

			// CUIDialog

			CUICustomDialog::CUICustomDialog(CUICustomWindow* parent, const std::uint32_t res_id, 
				DLGPROC dlg_proc) noexcept(true) : CUICustomWindow()
			{
				m_ResId = res_id;
				m_DlgProc = dlg_proc;
				m_Parent = parent;
			}

			void CUICustomDialog::Create() noexcept(true)
			{
				m_hWnd = (HWND)EditorUI::Hook::HKCreateDialogParamA(GetModuleHandle(nullptr), MAKEINTRESOURCEA(m_ResId), 
					m_Parent->Handle, (std::uintptr_t)m_DlgProc, (LONG_PTR)this);
				if (m_hWnd)
				{
					ShowWindow(m_hWnd, SW_SHOW);
					UpdateWindow(m_hWnd);
				}
			}

			void CUICustomDialog::FreeRelease() noexcept(true)
			{
				if (!m_hWnd) return;
				DestroyWindow(m_hWnd);
				m_hWnd = nullptr;
			};

			CUICustomDialog::~CUICustomDialog() noexcept(true)
			{
				FreeRelease();
			}

			// CUIBaseControl

			std::uint16_t CUIBaseControl::GetID() const noexcept(true)
			{
				return GetDlgCtrlID(m_hWnd);
			}

			// CUIMainWindow

			void CUIMainWindow::FindToolWindow() noexcept(true)
			{
				EnumChildWindows(Handle, [](HWND hwnd, LPARAM lParam) -> BOOL {
					CUIMainWindow* main = (CUIMainWindow*)lParam;
					CUIToolWindow Tool(hwnd);

					// For some reason, only the standard comparison function finds it...

					if (!_stricmp(Tool.GetName().c_str(), TOOLBARCLASSNAMEA))
						main->Toolbar = Tool;
					else if (!_stricmp(Tool.GetName().c_str(), STATUSCLASSNAMEA))
						main->Statusbar = Tool;
					return true;
				}, (LPARAM)this);
			}

			void CUIMainWindow::ProcessMessages() noexcept(true)
			{
				auto app = Interface::GetSingleton()->GetApplication();
				app->MessageProcessing();
			}

			MessageBox::Result CUIMainWindow::MessageDlg(const std::string& message, const std::string& caption,
				const MessageBox::Buttons buttons, const MessageBox::Icon icon) noexcept(true)
			{
				return MessageBox::Open(message, caption, buttons, icon);
			}

			MessageBox::Result CUIMainWindow::MessageDlg(const std::wstring& message, const std::wstring& caption,
				const MessageBox::Buttons buttons, const MessageBox::Icon icon) noexcept(true)
			{
				return MessageBox::Open(message, caption, buttons, icon);
			}

			MessageBox::Result CUIMainWindow::MessageWarningDlg(const std::string& message) noexcept(true)
			{
				return MessageBox::OpenWarning(message);
			}

			MessageBox::Result CUIMainWindow::MessageWarningDlg(const std::wstring& message) noexcept(true)
			{
				return MessageBox::OpenWarning(message);
			}

			void CUIMainWindow::SetTextToStatusBar(const uint32_t index, const std::string& text) noexcept(true)
			{
				Statusbar.Perform(SB_SETTEXTA, index, (LPARAM)text.c_str());
			}

			void CUIMainWindow::SetTextToStatusBar(const uint32_t index, const std::wstring& text) noexcept(true)
			{
				Statusbar.Perform(SB_SETTEXTW, index, (LPARAM)text.c_str());
			}

			std::string CUIMainWindow::GetTextToStatusBarA(const std::uint32_t index) noexcept(true)
			{
				auto nLen = Statusbar.Perform(SB_GETTEXTLENGTHA, index, NULL);
				if (nLen > 0)
				{
					std::string s;
					s.resize(++nLen);
					Statusbar.Perform(SB_GETTEXTA, index, (LPARAM)&s[0]);
					
					return s;
				}
				else return "";
			}

			std::wstring CUIMainWindow::GetTextToStatusBarW(const std::uint32_t index) noexcept(true)
			{
				auto nLen = Statusbar.Perform(SB_GETTEXTLENGTHW, index, NULL);
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


