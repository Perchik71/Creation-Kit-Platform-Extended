// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <memory>

#include <CKPE.Utils.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Docking.h>
#include <CKPE.Common.UIVarCommon.h>

namespace CKPE
{
	namespace Common
	{
		enum DockingState : std::uint32_t
		{
			DS_NONE = 0,
			DS_MOUSEMOVED = 1 << 0,
		};

		struct DOCKINFO
		{
			HWND DraggingDockWnd;
			HWND SizingDockWnd;
			HWND ContainerWnd;
			RECT DragRecPlacement;
			std::uint32_t State;
			DockingHoverWindow* HoverWnd;
			POINT MousePos;
		} CKPE_DockInfo{ 0 };

		// DockingWindow

		DockingWindow::DockingWindow(std::uintptr_t hWnd) : _Wnd(hWnd)
		{}

		std::uintptr_t DockingWindow::GetWindow() const noexcept(true)
		{
			return _Wnd;
		}

		// DockingHoverWindow

		static constexpr char DOCKING_HOVER_CLASSNAME[] = "CKPE_DockingHoverClass";

		static LRESULT CKPE_CDockingHoverProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
		{
			HDC hDC;
			PAINTSTRUCT ps;

			switch (uMsg)
			{
			case WM_CREATE:
			{
				auto lpCreateStruct = (LPCREATESTRUCTA)lParam;
				SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)lpCreateStruct->lpCreateParams);
				return S_OK;
			}
			case WM_PAINT:
				hDC = BeginPaint(hWnd, &ps);

				// ....

				EndPaint(hWnd, &ps);
				break;
			case WM_DESTROY:
				break;
			default:
				break;
			}

			return DefWindowProcA(hWnd, uMsg, wParam, lParam);
		};

		DockingHoverWindow::DockingHoverWindow() : DockingWindow()
		{
			WNDCLASSA wc{ 0 };

			auto cColor = RGB(19, 127, 238);

			wc.hbrBackground = ::CreateSolidBrush(cColor);
			wc.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(32654));
			wc.hInstance = GetModuleHandleA(NULL);
			wc.lpszClassName = DOCKING_HOVER_CLASSNAME;
			wc.style = CS_VREDRAW | CS_HREDRAW;
			wc.lpfnWndProc = &CKPE_CDockingHoverProc;

			if (!RegisterClassA(&wc))
				_FATALERROR("DockingHoverWindow::RegisterClassA() return failed \"%s\"", 
					ErrorHandler::GetSystemMessage(GetLastError()).c_str());

			HWND _Wnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
				DOCKING_HOVER_CLASSNAME, "", WS_POPUP, 200, 150, 100, 110, NULL, NULL, wc.hInstance, this);
			if (!_Wnd)
				_FATALERROR("DockingHoverWindow::CreateWindowExA() return failed \"%s\"",
					ErrorHandler::GetSystemMessage(GetLastError()).c_str());
		
			SetLayeredWindowAttributes(_Wnd, cColor, 128, LWA_ALPHA);
		}

		DockingHoverWindow::~DockingHoverWindow()
		{
			DestroyWindow((HWND)_Wnd);
			UnregisterClassA(DOCKING_HOVER_CLASSNAME, GetModuleHandleA(NULL));
		}

		void DockingHoverWindow::Show() const noexcept(true)
		{
			SetWindowPos((HWND)_Wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			UpdateWindow((HWND)_Wnd);
		}

		void DockingHoverWindow::Show(std::int32_t x, std::int32_t y, std::int32_t wx, std::int32_t wy) const noexcept(true)
		{
			SetWindowPos((HWND)_Wnd, HWND_TOPMOST, x, y, wx - x, wy - y, SWP_SHOWWINDOW);
		}

		void DockingHoverWindow::Hide() const noexcept(true)
		{
			ShowWindow((HWND)_Wnd, SW_HIDE);
		}

		void DockingHoverWindow::Move(std::int32_t x, std::int32_t y, std::int32_t wx, std::int32_t wy) const noexcept(true)
		{
			SetWindowPos((HWND)_Wnd, HWND_TOPMOST, x, y, wx - x, wy - y, SWP_NONE);
		}

		DockingWindow* DockingHoverWindow::GetLinkWindow() const noexcept(true)
		{
			return _link_wnd;
		}

		void DockingHoverWindow::SetLinkWindow(DockingWindow* wnd) noexcept(true)
		{
			_link_wnd = wnd;
		}

		// DockingFrameWindow

		static constexpr char DOCKING_FRAME[] = "CKPE_DockingFrame";

		static void CKPE_CDockingFrameDrawBorder(HDC DC, RECT rWindow, RECT rClient) noexcept(true)
		{
			auto hRgnWindow = CreateRectRgnIndirect(&rWindow);
			auto hRgnClient = CreateRectRgnIndirect(&rClient);
			auto hNCRgn = CreateRectRgn(0, 0, 0, 0);

			CombineRgn(hNCRgn, hRgnWindow, hRgnClient, RGN_DIFF);

			if (UI::IsDarkTheme())
			{
				auto Color = UI::GetThemeSysColor(UI::ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End);
				auto hLighter = ::CreateSolidBrush(Color);

				FillRgn(DC, hNCRgn, (HBRUSH)UI::Comctl32GetSysColorBrush(COLOR_BTNFACE));
				FrameRgn(DC, hNCRgn, hLighter, 2, 2);
				FrameRgn(DC, hNCRgn, (HBRUSH)UI::Comctl32GetSysColorBrush(COLOR_WINDOWFRAME), 1, 1);

				DeleteObject(hLighter);
			}
			else
			{
				FillRgn(DC, hNCRgn, (HBRUSH)GetStockObject(WHITE_BRUSH));
				FrameRgn(DC, hNCRgn, (HBRUSH)GetStockObject(DKGRAY_BRUSH), 1, 1);
			}

			DeleteObject(hRgnWindow);
			DeleteObject(hRgnClient);
			DeleteObject(hNCRgn);
		}

		static void CKPE_CDockingFrameDrawCaption(HDC DC, HWND hWnd, std::int32_t bx, std::int32_t by) noexcept(true)
		{
			CKPE::Font font(DC);
			font.Size = 10;

			if (CKPE_UserUseWine())
				font.SetName("Tahoma");
			else
				font.SetName("Consolas");

			SetBkMode(DC, TRANSPARENT);
			SelectObject(DC, font.Handle);

			auto buffer = std::make_unique<char[]>(1024);
			auto len = GetWindowTextA(hWnd, buffer.get(), 1024);

			RECT rClient{};
			GetClientRect(hWnd, &rClient);
			InflateRect(&rClient, -(bx << 1), 0);
			rClient.bottom = by;

			if (UI::IsDarkTheme())
				SetTextColor(DC, UI::Comctl32GetSysColor(COLOR_BTNTEXT));
			else
				SetTextColor(DC, RGB(20, 20, 20));

			DrawTextA(DC, buffer.get(), len, &rClient, DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);
			SetBkMode(DC, OPAQUE);
		}

		static LRESULT CKPE_CDockingFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam,
			LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
		{
			auto pFrame = (DockingFrameWindow*)dwRefData;
			if (!pFrame)
			{
			ReturnDef:
				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			}

			switch (uMsg)
			{
			case WM_SETTEXT:
			{
				RedrawWindow((HWND)hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
				break;
			}
			case WM_NCPAINT:
			{
				RECT rWindow{};
				GetWindowRect(hWnd, &rWindow);

				POINT pt{ 0, 0 };
				MapWindowPoints(hWnd, NULL, &pt, 1);

				pt.x -= rWindow.left;
				pt.y -= rWindow.top;

				rWindow.right -= rWindow.left;
				rWindow.left = 0;
				rWindow.bottom -= rWindow.top;
				rWindow.top = 0;

				RECT rClient{};
				GetClientRect(hWnd, &rClient);
				rClient.left = pt.x;
				rClient.top = pt.y;
				rClient.right += pt.x;
				rClient.bottom += pt.y;

				auto DC = GetWindowDC(hWnd);

				CKPE_CDockingFrameDrawBorder(DC, rWindow, rClient);
				CKPE_CDockingFrameDrawCaption(DC, hWnd, pt.x, pt.y);

				ReleaseDC(hWnd, DC);

				return S_OK;
			}
			case WM_NCLBUTTONDBLCLK:
			{
				// User clicked on the caption area of the Docking Frame?
				if ((wParam == HTCAPTION) && pFrame->HasDocking())
				{
					// Yes he did. Toggle between docking and floating
					pFrame->Undock();

					return S_OK;
				}

				break;
			}
			case WM_NCLBUTTONDOWN:
			{
				if (wParam == HTCAPTION)
				{
					// Get mouse pointer position in screen coords			
					CKPE_DockInfo.MousePos.x = GET_X_LPARAM(lParam);
					CKPE_DockInfo.MousePos.y = GET_Y_LPARAM(lParam);
					// Indicate dragging now
					CKPE_DockInfo.DraggingDockWnd = hWnd;
					// Get the current position/size of Docking Frame and save it in a
					// global RECT. This may either be the docked size, or the floating size
					GetWindowRect(hWnd, &CKPE_DockInfo.DragRecPlacement);
					// Is it docked?
					if (pFrame->HasDocking())
					{
						// Prevent the operating system from dragging the window
						return S_OK;
					}
				}

				break;
			}
			case WM_CANCELMODE:
			case WM_LBUTTONUP:
			{
				// Dragging?
				if (CKPE_DockInfo.DraggingDockWnd)
				{
					// Indicate done with the dragging
					CKPE_DockInfo.DraggingDockWnd = NULL;
					// Did the user actually move the Docking Frame, and not cancel the operation?
					if ((uMsg != WM_CANCELMODE) && ((CKPE_DockInfo.State & DS_MOUSEMOVED) == DS_MOUSEMOVED))
					{

					}

				}

				break;
			}
			case WM_MOVING:
			{
				// Dragging?
				if (CKPE_DockInfo.DraggingDockWnd)
				{
					auto lprc = (LPRECT)lParam;
					POINT pt = { lprc->left - 3, lprc->top - 3 };

					auto hPointWnd = WindowFromPoint(pt);
					if (hPointWnd && GetPropA(hPointWnd, DOCKING_FRAME))
					{
						CKPE_DockInfo.HoverWnd->Show();


						CKPE_DockInfo.State |= DS_MOUSEMOVED;

						ScreenToClient(hPointWnd, &pt);

						CKPE_DockInfo.DragRecPlacement = *lprc;
						//CKPE_CDockingFrameCheckRect(hPointWnd, &CKPE_DockInfo.DragRecPlacement, &pt);

						//drawDragFrame();

						_CONSOLE("1 %d %d %x", lprc->left, lprc->top, hPointWnd);
					}
					else
					{
						CKPE_DockInfo.HoverWnd->Hide();
						CKPE_DockInfo.State &= ~DS_MOUSEMOVED;
					}

				}

				break;
			}
			}

			// TODO

			goto ReturnDef;
		}

		void DockingFrameWindow::RestoreWindowStyles() noexcept(true)
		{
			if (HasFlag(EF_DOCKSTYLES))
			{
				SetWindowLongA((HWND)_Wnd, GWL_EXSTYLE, _OldExStyles);
				SetWindowLongA((HWND)_Wnd, GWL_STYLE, _OldStyles);
				RedrawWindow((HWND)_Wnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
				ShowWindow((HWND)_Wnd, SW_SHOWNORMAL);
				SetForegroundWindow((HWND)_Wnd);
				UnsetFlag(EF_DOCKSTYLES);
			}
		}

		void DockingFrameWindow::ApplyDockingWindowStyles() noexcept(true)
		{
			if (!HasFlag(EF_DOCKSTYLES))
			{
				SetWindowLongA((HWND)_Wnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
				SetWindowLongA((HWND)_Wnd, GWL_STYLE, (_OldStyles & ~WS_POPUP) | WS_CHILD);
				RedrawWindow((HWND)_Wnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
				SetFlag(EF_DOCKSTYLES);
			}
		}

		void DockingFrameWindow::SetFlag(std::uint32_t f) noexcept(true)
		{
			_Flags |= f;
		}

		void DockingFrameWindow::UnsetFlag(std::uint32_t f) noexcept(true)
		{
			_Flags &= ~f;
		}

		DockingFrameWindow::DockingFrameWindow(std::uintptr_t hWnd) : DockingWindow(hWnd)
		{
			if (hWnd && IsWindow((HWND)_Wnd))
			{
				auto Result = SetWindowSubclass((HWND)_Wnd, CKPE_CDockingFrameProc, (DWORD_PTR)CKPE_CDockingFrameProc,
					(DWORD_PTR)this);
				
				char szBuf[200];
				GetWindowTextA((HWND)_Wnd, szBuf, 200);

				if (Result)
				{
					_OldStyles = GetWindowLongA((HWND)_Wnd, GWL_STYLE);
					_OldExStyles = GetWindowLongA((HWND)_Wnd, GWL_EXSTYLE);

					SetPropA((HWND)_Wnd, DOCKING_FRAME, this);

					_MESSAGE("Docking \"%s\" Window created", szBuf);
				}
				else
					_ERROR("Docking \"%s\" Window failed", szBuf);
			}
		}

		DockingFrameWindow::~DockingFrameWindow()
		{
			if (IsWindow((HWND)_Wnd))
			{
				RemovePropA((HWND)_Wnd, DOCKING_FRAME);
				RemoveWindowSubclass((HWND)_Wnd, CKPE_CDockingFrameProc, (DWORD_PTR)CKPE_CDockingFrameProc);
				RestoreWindowStyles();
			}
		}

		bool DockingFrameWindow::Undock() noexcept(true)
		{
			if (!HasDocking()) return false;

			_Container = nullptr;

			RestoreWindowStyles();
			SetParent((HWND)_Wnd, NULL);

			return true;
		}

		bool DockingFrameWindow::Dock(std::uintptr_t hWnd) noexcept(true)
		{
			if (HasDocking()) return false;

			_Container = (DockingWindow*)GetPropA((HWND)hWnd, DOCKING_FRAME);
			if (!_Container) return false;

			ApplyDockingWindowStyles();
			SetParent((HWND)_Wnd, (HWND)hWnd);

			return true;
		}

		bool DockingFrameWindow::HasDocking() const noexcept(true)
		{
			return _Container != nullptr;
		}

		DockingWindow* DockingFrameWindow::GetDockContainer() const noexcept(true)
		{
			return _Container;
		}

		std::uint32_t DockingFrameWindow::GetFlags() const noexcept(true)
		{
			return _Flags;
		}

		bool DockingFrameWindow::HasFlag(std::uint32_t f) const noexcept(true)
		{
			return (_Flags & f) == f;
		}

		DockingRootWindow::DockingRootWindow() : DockingFrameWindow(0)
		{}

		bool DockingRootWindow::SetWindow(std::uintptr_t hWnd) noexcept(true)
		{
			if (IsWindow((HWND)hWnd))
			{
				_Wnd = hWnd;

				auto Result = SetWindowSubclass((HWND)_Wnd, CKPE_CDockingFrameProc, (DWORD_PTR)CKPE_CDockingFrameProc,
					(DWORD_PTR)this);

				if (Result)
				{
					_OldStyles = GetWindowLongA((HWND)_Wnd, GWL_STYLE);
					_OldExStyles = GetWindowLongA((HWND)_Wnd, GWL_EXSTYLE);

					SetPropA((HWND)_Wnd, DOCKING_FRAME, this);

					_MESSAGE("Docking Root Window created");
				}
				else
					_ERROR("Docking Root Window failed");

				return Result;
			}

			return false;
		}

		DockingManager::DockingManager() : _Container(new std::map<std::uintptr_t, DockingFrameWindow*>),
			DockingRootWindow()
		{
			CKPE_DockInfo.HoverWnd = &_HoverWindow;
		}

		DockingManager::~DockingManager()
		{
			FreeAll();

			if (_Container)
				delete _Container;
		}

		void DockingManager::FreeAll() noexcept(true)
		{
			ScopeCriticalSection lock(_Guard);

			if (!_Container)
				return;

			for (auto& i : *_Container)
				if (i.second)
					delete i.second;

			_Container->clear();
		}

		bool DockingManager::AddWindow(std::uintptr_t hWnd, std::uint32_t style) noexcept(true)
		{
			ScopeCriticalSection lock(_Guard);

			if (!_Container || !IsWindow((HWND)hWnd) || (_Container->find(hWnd) != _Container->end()))
				return false;

			DockingFrameWindow* DockWnd = nullptr;

			switch (style)
			{
			case E_FRAME:
				DockWnd = new DockingFrameWindow(hWnd);
				break;
			default:
				break;
			} 

			if (DockWnd)
			{
				_Container->insert({hWnd, DockWnd });
				return true;
			}

			return false;
		}

		bool DockingManager::RemoveWindow(std::uintptr_t hWnd) noexcept(true)
		{
			ScopeCriticalSection lock(_Guard);

			if (!_Container)
				return false;

			auto it = _Container->find(hWnd);
			if (it == _Container->end())
				return false;

			if (it->second)
				delete it->second;

			_Container->erase(it);

			return true;
		}

		std::size_t DockingManager::Count() const noexcept(true)
		{
			ScopeCriticalSection lock(_Guard);

			return _Container ? _Container->size() : 0;
		}

		DockingWindow* DockingManager::At(std::uintptr_t hWnd) const noexcept(true)
		{
			ScopeCriticalSection lock(_Guard);

			if (!_Container)
				return nullptr;

			auto it = _Container->find(hWnd);
			if (it == _Container->end())
				return nullptr;

			return it->second;
		}

		DockingWindow* DockingManager::AtByIndex(std::size_t id) const noexcept(true)
		{
			ScopeCriticalSection lock(_Guard);

			if (!_Container || (_Container->size() <= id))
				return nullptr;

			auto it = _Container->begin();
			std::advance(it, id);

			return it->second;
		}
	}
}