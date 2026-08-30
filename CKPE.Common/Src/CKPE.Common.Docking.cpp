// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <dwmapi.h>

#include <memory>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <vector>
#include <map>
#include <string>
#include <fstream>

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
		
		static bool s_InSplitterSync = false;

		// Tracks which regions of each anchor are already claimed by a docked panel
		struct AnchorPanel
		{
			HWND Wnd;
			RECT Rect;
			std::uint32_t Zone;
		};
		static std::map<HWND, std::vector<AnchorPanel>> CKPE_AnchorDockedPanels;

		static void CKPE_CDockingPruneAnchorPanels(HWND anchorWnd, HWND hwnd) noexcept(true)
		{
			auto it = CKPE_AnchorDockedPanels.find(anchorWnd);
			if (it == CKPE_AnchorDockedPanels.end())
				return;

			auto& panels = it->second;
			panels.erase(std::remove_if(panels.begin(), panels.end(),
				[hwnd](const AnchorPanel& p)
				{
					return (p.Wnd == hwnd) || !IsWindow(p.Wnd) || !IsWindowVisible(p.Wnd);
				}), panels.end());
		}

		// DockingWindow

		DockingWindow::DockingWindow(std::uintptr_t hWnd) : _Wnd(hWnd)
		{}

		std::uintptr_t DockingWindow::GetWindow() const noexcept(true)
		{
			return _Wnd;
		}

		// DockingHoverWindow

		static constexpr char DOCKING_HOVER_CLASSNAME[] = "CKPE_DockingHoverClass";

		constexpr int kDockingHoverCornerRadius = 8;	
		constexpr COLORREF kDockingHoverFillColor = RGB(53, 92, 133);
		constexpr COLORREF kDockingHoverBorderColor = RGB(126, 161, 196);

		static void CKPE_CDockingHoverPaintBorder(HWND hWnd) noexcept(true)
		{
			PAINTSTRUCT ps{};
			auto hDC = BeginPaint(hWnd, &ps);

			RECT rc{};
			GetClientRect(hWnd, &rc);

			constexpr int kBorderWidth = 3;
			auto pen = CreatePen(PS_INSIDEFRAME, kBorderWidth, kDockingHoverBorderColor);
			auto oldPen = (HPEN)SelectObject(hDC, pen);
			auto oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

			RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, kDockingHoverCornerRadius * 2, kDockingHoverCornerRadius * 2);

			SelectObject(hDC, oldBrush);
			SelectObject(hDC, oldPen);
			DeleteObject(pen);

			EndPaint(hWnd, &ps);
		}

		static void CKPE_CDockingHoverApplyRoundRegion(HWND hWnd, int width, int height) noexcept(true)
		{
			auto rgn = CreateRoundRectRgn(0, 0, width + 1, height + 1, kDockingHoverCornerRadius * 2, kDockingHoverCornerRadius * 2);
			if (rgn && !SetWindowRgn(hWnd, rgn, TRUE))
				DeleteObject(rgn);
		}

		static LRESULT CKPE_CDockingHoverProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
		{
			switch (uMsg)
			{
			case WM_CREATE:
			{
				auto lpCreateStruct = (LPCREATESTRUCTA)lParam;
				SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)lpCreateStruct->lpCreateParams);
				return S_OK;
			}
			case WM_PAINT:
				CKPE_CDockingHoverPaintBorder(hWnd);
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

			wc.hbrBackground = ::CreateSolidBrush(kDockingHoverFillColor);
			wc.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(32654));
			wc.hInstance = GetModuleHandleA(NULL);
			wc.lpszClassName = DOCKING_HOVER_CLASSNAME;
			wc.style = CS_VREDRAW | CS_HREDRAW;
			wc.lpfnWndProc = &CKPE_CDockingHoverProc;

			if (!RegisterClassA(&wc))
				_FATALERROR_EX("DockingHoverWindow::RegisterClassA() return failed \"{}\""sv, 
					ErrorHandler::GetSystemMessageUTF8(GetLastError()));

			_Wnd = (std::uintptr_t)CreateWindowExA(WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST |
				WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
				DOCKING_HOVER_CLASSNAME, "", WS_POPUP, 200, 150, 100, 110, NULL, NULL, wc.hInstance, this);
			if (!_Wnd)
				_FATALERROR_EX("DockingHoverWindow::CreateWindowExA() return failed \"{}\""sv,
					ErrorHandler::GetSystemMessageUTF8(GetLastError()));

			SetLayeredWindowAttributes((HWND)_Wnd, kDockingHoverFillColor, 100, LWA_ALPHA);
		}

		DockingHoverWindow::~DockingHoverWindow()
		{
			DestroyWindow((HWND)_Wnd);
			UnregisterClassA(DOCKING_HOVER_CLASSNAME, GetModuleHandleA(NULL));
		}

		void DockingHoverWindow::Show() const noexcept(true)
		{
			SetWindowPos((HWND)_Wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
			UpdateWindow((HWND)_Wnd);
		}

		void DockingHoverWindow::Show(std::int32_t x, std::int32_t y, std::int32_t wx, std::int32_t wy) const noexcept(true)
		{
			CKPE_CDockingHoverApplyRoundRegion((HWND)_Wnd, wx - x, wy - y);
			SetWindowPos((HWND)_Wnd, HWND_TOPMOST, x, y, wx - x, wy - y, SWP_SHOWWINDOW | SWP_NOACTIVATE);
		}

		void DockingHoverWindow::Hide() const noexcept(true)
		{
			ShowWindow((HWND)_Wnd, SW_HIDE);
		}

		void DockingHoverWindow::Move(std::int32_t x, std::int32_t y, std::int32_t wx, std::int32_t wy) const noexcept(true)
		{
			CKPE_CDockingHoverApplyRoundRegion((HWND)_Wnd, wx - x, wy - y);
			SetWindowPos((HWND)_Wnd, HWND_TOPMOST, x, y, wx - x, wy - y, 0);
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

		static RECT CKPE_CDockingGetVisibleRect(HWND hWnd) noexcept(true)
		{
			RECT extended{};
			if (SUCCEEDED(DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &extended, sizeof(extended))) &&
				(extended.right > extended.left) && (extended.bottom > extended.top))
				return extended;

			RECT fallback{};
			GetWindowRect(hWnd, &fallback);
			return fallback;
		}

		static void CKPE_CDockingFrameSetVisibleRect(HWND hWnd, const RECT& visibleRect) noexcept(true)
		{
			RECT rawRect{};
			GetWindowRect(hWnd, &rawRect);

			auto visibleNow = CKPE_CDockingGetVisibleRect(hWnd);

			RECT target
			{
				visibleRect.left - (visibleNow.left - rawRect.left),
				visibleRect.top - (visibleNow.top - rawRect.top),
				visibleRect.right + (rawRect.right - visibleNow.right),
				visibleRect.bottom + (rawRect.bottom - visibleNow.bottom),
			};

			SetWindowPos(hWnd, NULL, target.left, target.top,
				target.right - target.left, target.bottom - target.top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}

		static RECT CKPE_CDockingFrameGetRawAnchorRect(HWND anchorHwnd) noexcept(true)
		{
			RECT client{};
			GetClientRect(anchorHwnd, &client);

			POINT topLeft{ client.left, client.top };
			POINT bottomRight{ client.right, client.bottom };
			ClientToScreen(anchorHwnd, &topLeft);
			ClientToScreen(anchorHwnd, &bottomRight);

			RECT usable{ topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };

			for (HWND child = GetWindow(anchorHwnd, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT))
			{
				if (!IsWindowVisible(child))
					continue;

				char className[64]{ 0 };
				GetClassNameA(child, className, sizeof(className));

				RECT childRect{};

				if (!_stricmp(className, TOOLBARCLASSNAME) || !_stricmp(className, REBARCLASSNAMEA))
				{
					GetWindowRect(child, &childRect);
					usable.top = (std::max)(usable.top, childRect.bottom);
				}
				else if (!_stricmp(className, STATUSCLASSNAMEA))
				{
					GetWindowRect(child, &childRect);
					usable.bottom = (std::min)(usable.bottom, childRect.top);
				}
			}

			return usable;
		}

		static AnchorPanel* CKPE_CDockingFrameFindOverlappingClaim(const RECT& rect, HWND anchorHwnd) noexcept(true)
		{
			auto it = CKPE_AnchorDockedPanels.find(anchorHwnd);
			if (it == CKPE_AnchorDockedPanels.end())
				return nullptr;

			RECT dummy{};
			for (auto& panel : it->second)
			{
				if (IntersectRect(&dummy, &rect, &panel.Rect))
					return &panel;
			}
			return nullptr;
		}

		static LONG CKPE_CDockingFrameFindColumnBoundaryX(HWND anchorHwnd, const RECT& raw) noexcept(true)
		{
			auto it = CKPE_AnchorDockedPanels.find(anchorHwnd);
			if (it != CKPE_AnchorDockedPanels.end())
			{
				for (auto& panel : it->second)
				{
					if (panel.Zone & DockingFrameWindow::EFR_DOCKLEFT)
						return panel.Rect.right;
					if (panel.Zone & DockingFrameWindow::EFR_DOCKRIGHT)
						return panel.Rect.left;
				}
			}

			return (raw.left + raw.right) / 2;
		}

		static LONG CKPE_CDockingFrameFindColumnBoundaryY(HWND anchorHwnd, const RECT& raw,
			std::uint32_t columnBit) noexcept(true)
		{
			auto it = CKPE_AnchorDockedPanels.find(anchorHwnd);
			if (it != CKPE_AnchorDockedPanels.end())
			{
				for (auto& panel : it->second)
				{
					if (!(panel.Zone & columnBit))
						continue;
					if (panel.Zone & DockingFrameWindow::EFR_DOCKTOP)
						return panel.Rect.bottom;
					if (panel.Zone & DockingFrameWindow::EFR_DOCKBOTTOM)
						return panel.Rect.top;
				}
			}

			return (raw.top + raw.bottom) / 2;
		}

		static void CKPE_CDockingFrameComputeZoneRect(HWND anchorHwnd, std::uint32_t zone, RECT& outRect) noexcept(true)
		{
			auto raw = CKPE_CDockingFrameGetRawAnchorRect(anchorHwnd);
			auto midX = CKPE_CDockingFrameFindColumnBoundaryX(anchorHwnd, raw);

			auto columnZone = zone & (DockingFrameWindow::EFR_DOCKLEFT | DockingFrameWindow::EFR_DOCKRIGHT);
			bool leftColumn = (columnZone == DockingFrameWindow::EFR_DOCKLEFT);
			RECT column = leftColumn ?
				RECT{ raw.left, raw.top, midX, raw.bottom } :
				RECT{ midX, raw.top, raw.right, raw.bottom };

			if (zone & DockingFrameWindow::EFR_DOCKTOP)
			{
				auto midY = CKPE_CDockingFrameFindColumnBoundaryY(anchorHwnd, raw, columnZone);
				outRect = { column.left, raw.top, column.right, midY };
			}
			else if (zone & DockingFrameWindow::EFR_DOCKBOTTOM)
			{
				auto midY = CKPE_CDockingFrameFindColumnBoundaryY(anchorHwnd, raw, columnZone);
				outRect = { column.left, midY, column.right, raw.bottom };
			}
			else
			{
				outRect = column;
			}
		}


		static bool CKPE_CDockingFrameTestAnchorEdge(HWND anchorHwnd, const POINT& cursor,
			RECT& outZoneRect, std::uint32_t& outZone) noexcept(true)
		{
			constexpr LONG kEdgeSlop = 16;
			constexpr LONG kQuadrantPercent = 35;
			constexpr LONG kColumnDeadZonePercent = 50;

			CKPE_CDockingPruneAnchorPanels(anchorHwnd, NULL);

			auto raw = CKPE_CDockingFrameGetRawAnchorRect(anchorHwnd);

			RECT loose = raw;
			InflateRect(&loose, kEdgeSlop, kEdgeSlop);
			if (!PtInRect(&loose, cursor))
				return false;

			auto midX = CKPE_CDockingFrameFindColumnBoundaryX(anchorHwnd, raw);

			auto leftDeadZoneEdge = midX - (midX - raw.left) * kColumnDeadZonePercent / 100;
			auto rightDeadZoneEdge = midX + (raw.right - midX) * kColumnDeadZonePercent / 100;
			if ((cursor.x > leftDeadZoneEdge) && (cursor.x < rightDeadZoneEdge))
				return false;

			auto columnHeight = raw.bottom - raw.top;
			auto quadBand = columnHeight * kQuadrantPercent / 100;

			bool leftColumn = cursor.x < midX;
			auto columnZone = leftColumn ? DockingFrameWindow::EFR_DOCKLEFT : DockingFrameWindow::EFR_DOCKRIGHT;

			std::uint32_t zone;
			if ((cursor.y - raw.top) <= quadBand)
				zone = columnZone | DockingFrameWindow::EFR_DOCKTOP;
			else if ((raw.bottom - cursor.y) <= quadBand)
				zone = columnZone | DockingFrameWindow::EFR_DOCKBOTTOM;
			else
				zone = columnZone;

			RECT candidate{};
			CKPE_CDockingFrameComputeZoneRect(anchorHwnd, zone, candidate);

			auto newIsQuadrant = (zone & (DockingFrameWindow::EFR_DOCKTOP | DockingFrameWindow::EFR_DOCKBOTTOM)) != 0;
			auto overlap = CKPE_CDockingFrameFindOverlappingClaim(candidate, anchorHwnd);
			if (overlap && !(newIsQuadrant && (overlap->Zone == columnZone)))
				return false;

			outZoneRect = candidate;
			outZone = zone;
			return true;
		}

		static std::uint32_t CKPE_CDockingFrameComputeDropZone(DockingFrameWindow* target, const POINT& cursor,
			RECT& outZoneRect) noexcept(true)
		{
			auto targetHwnd = (HWND)target->GetWindow();

			std::uint32_t zone{};
			if (!CKPE_CDockingFrameTestAnchorEdge(targetHwnd, cursor, outZoneRect, zone))
				return DockingFrameWindow::EF_NONE;
			return zone;
		}

		static DockingFrameWindow* CKPE_CDockingFrameFindHoverTarget(HWND self, const POINT& cursor) noexcept(true)
		{
			auto mgr = Common::Interface::GetSingleton()->GetDockingManager();
			if (!mgr)
				return nullptr;

			for (std::size_t i = 0; i < mgr->Count(); i++)
			{
				auto wnd = mgr->AtByIndex(i);
				if (!wnd)
					continue;

				auto candidateHwnd = (HWND)wnd->GetWindow();
				if (!candidateHwnd || (candidateHwnd == self) ||
					!IsWindow(candidateHwnd) || !IsWindowVisible(candidateHwnd))
					continue;

				auto dockFrame = (DockingFrameWindow*)wnd;
				if (!dockFrame->HasFlag(DockingFrameWindow::EF_ANCHOR))
					continue;

				RECT zoneRect{};
				if (CKPE_CDockingFrameComputeDropZone(dockFrame, cursor, zoneRect) != DockingFrameWindow::EF_NONE)
					return dockFrame;
			}

			return nullptr;
		}

		static void CKPE_CDockInfoAbortDrag() noexcept(true)
		{
			if (!CKPE_DockInfo.DraggingDockWnd)
				return;

			CKPE_DockInfo.DraggingDockWnd = NULL;
			CKPE_DockInfo.ContainerWnd = NULL;
			CKPE_DockInfo.State &= ~DS_MOUSEMOVED;

			if (CKPE_DockInfo.HoverWnd)
				CKPE_DockInfo.HoverWnd->Hide();
		}

		static void CKPE_CDockingFrameClampToMinSize(HWND hWnd, RECT& rect, std::uint32_t zone) noexcept(true)
		{
			MINMAXINFO mmi{};
			mmi.ptMaxTrackSize.x = GetSystemMetrics(SM_CXMAXTRACK);
			mmi.ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK);
			SendMessageA(hWnd, WM_GETMINMAXINFO, 0, (LPARAM)&mmi);

			auto width = rect.right - rect.left;
			if (width < mmi.ptMinTrackSize.x)
			{
				auto grow = mmi.ptMinTrackSize.x - width;
				if (zone & DockingFrameWindow::EFR_DOCKRIGHT)
					rect.left -= grow;
				else
					rect.right += grow;
			}

			auto height = rect.bottom - rect.top;
			if (height < mmi.ptMinTrackSize.y)
			{
				auto grow = mmi.ptMinTrackSize.y - height;
				if (zone & DockingFrameWindow::EFR_DOCKBOTTOM)
					rect.top -= grow;
				else
					rect.bottom += grow;
			}
		}

		static void CKPE_CDockingFrameApplyZone(HWND anchorHwnd, HWND hWnd, std::uint32_t zone, RECT zoneRect) noexcept(true)
		{
			auto columnBit = zone & (DockingFrameWindow::EFR_DOCKLEFT | DockingFrameWindow::EFR_DOCKRIGHT);
			if (auto overlap = CKPE_CDockingFrameFindOverlappingClaim(zoneRect, anchorHwnd);
				overlap && (overlap->Zone == columnBit))
			{
				auto shrinkWnd = overlap->Wnd;
				RECT shrunk = overlap->Rect;

				if (zone & DockingFrameWindow::EFR_DOCKTOP)
					shrunk.top = zoneRect.bottom;		// new claim took the top - keep the bottom
				else
					shrunk.bottom = zoneRect.top;		// new claim took the bottom - keep the top

				auto shrinkZone = columnBit | ((zone & DockingFrameWindow::EFR_DOCKTOP) ?
					DockingFrameWindow::EFR_DOCKBOTTOM : DockingFrameWindow::EFR_DOCKTOP);

				CKPE_CDockingFrameClampToMinSize(shrinkWnd, shrunk, shrinkZone);
				CKPE_CDockingFrameSetVisibleRect(shrinkWnd, shrunk);

				for (auto& panel : CKPE_AnchorDockedPanels[anchorHwnd])
				{
					if (panel.Wnd == shrinkWnd)
					{
						panel.Rect = shrunk;
						panel.Zone = shrinkZone;
						break;
					}
				}
			}

			CKPE_CDockingFrameClampToMinSize(hWnd, zoneRect, zone);
			CKPE_CDockingFrameSetVisibleRect(hWnd, zoneRect);
			CKPE_CDockingPruneAnchorPanels(anchorHwnd, hWnd);
			CKPE_AnchorDockedPanels[anchorHwnd].push_back({ hWnd, zoneRect, zone });
		}

		static std::wstring CKPE_CDockingGetLayoutFilePath() noexcept(true)
		{
			return Common::Interface::GetSingleton()->GetApplication()->GetFilePath() +
				L"CreationKitPlatformExtendedDockingLayout.ini";
		}

		static std::map<std::string, std::uint32_t>& CKPE_CDockingGetLayoutCache() noexcept(true)
		{
			static std::map<std::string, std::uint32_t> s_Cache;
			static bool s_Loaded = false;

			if (!s_Loaded)
			{
				s_Loaded = true;

				std::ifstream file(CKPE_CDockingGetLayoutFilePath());
				std::string line;
				while (std::getline(file, line))
				{
					auto eq = line.find('=');
					if (eq == std::string::npos)
						continue;

					auto title = StringUtils::Trim(line.substr(0, eq));
					auto zone = (std::uint32_t)strtoul(line.substr(eq + 1).c_str(), nullptr, 10);
					if (!title.empty() && zone)
						s_Cache[title] = zone;
				}
			}

			return s_Cache;
		}

		static void CKPE_CDockingFlushLayoutCache() noexcept(true)
		{
			std::ofstream file(CKPE_CDockingGetLayoutFilePath(), std::ios::trunc);
			if (!file.is_open())
				return;

			for (auto& entry : CKPE_CDockingGetLayoutCache())
				file << entry.first << "=" << entry.second << "\n";
		}

		static void CKPE_CDockingSaveLayout(HWND anchorHwnd) noexcept(true)
		{
			auto& cache = CKPE_CDockingGetLayoutCache();

			auto mgr = Common::Interface::GetSingleton()->GetDockingManager();
			if (mgr)
			{
				for (std::size_t i = 0; i < mgr->Count(); i++)
				{
					auto wnd = (DockingFrameWindow*)mgr->AtByIndex(i);
					if (!wnd || wnd->HasFlag(DockingFrameWindow::EF_ANCHOR))
						continue;

					char title[200]{ 0 };
					GetWindowTextA((HWND)wnd->GetWindow(), title, sizeof(title));
					if (title[0])
						cache.erase(title);
				}
			}

			auto it = CKPE_AnchorDockedPanels.find(anchorHwnd);
			if (it != CKPE_AnchorDockedPanels.end())
			{
				for (auto& panel : it->second)
				{
					char title[200]{ 0 };
					GetWindowTextA(panel.Wnd, title, sizeof(title));
					if (title[0])
						cache[title] = panel.Zone;
				}
			}

			CKPE_CDockingFlushLayoutCache();
		}

		static void CKPE_CDockingTryRestoreSavedZone(HWND anchorHwnd, HWND hWnd) noexcept(true)
		{
			char title[200]{ 0 };
			GetWindowTextA(hWnd, title, sizeof(title));
			if (!title[0])
				return;

			auto& cache = CKPE_CDockingGetLayoutCache();
			auto it = cache.find(title);
			if (it == cache.end())
				return;

			if (!IsWindowVisible(hWnd))
				ShowWindow(hWnd, SW_SHOWNOACTIVATE);

			RECT zoneRect{};
			CKPE_CDockingFrameComputeZoneRect(anchorHwnd, it->second, zoneRect);
			CKPE_CDockingFrameApplyZone(anchorHwnd, hWnd, it->second, zoneRect);
		}

		static void CKPE_CDockInfoFinishDrag(HWND hWnd, bool wasCancelled) noexcept(true)
		{
			if (!CKPE_DockInfo.DraggingDockWnd || (CKPE_DockInfo.DraggingDockWnd != hWnd))
				return;

			if (!wasCancelled && ((CKPE_DockInfo.State & DS_MOUSEMOVED) == DS_MOUSEMOVED) && CKPE_DockInfo.ContainerWnd)
			{
				auto containerWnd = CKPE_DockInfo.ContainerWnd;

				auto mgr = Common::Interface::GetSingleton()->GetDockingManager();
				auto containerDockWnd = mgr ? (DockingFrameWindow*)mgr->At((std::uintptr_t)containerWnd) : nullptr;

				POINT cursor{};
				GetCursorPos(&cursor);

				RECT zoneRect{};
				auto zone = containerDockWnd ? CKPE_CDockingFrameComputeDropZone(containerDockWnd, cursor, zoneRect) : DockingFrameWindow::EF_NONE;

				if (zone != DockingFrameWindow::EF_NONE)
				{
					CKPE_CDockingFrameApplyZone(containerWnd, hWnd, zone, zoneRect);
					CKPE_CDockingSaveLayout(containerWnd);

					//_MESSAGE("[dock-diag] snapped into place, zone=0x%X", zone);
				}
				else
				{
					//_MESSAGE("[dock-diag] drop didn't qualify at release (cursor drifted off the edge band)");
				}
			}

			CKPE_CDockInfoAbortDrag();
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

			//if (uMsg == WM_NCLBUTTONDOWN)
			//	_MESSAGE("[dock-diag] WM_NCLBUTTONDOWN hWnd=%p hittest=%llu (HTCAPTION=2)", hWnd, (unsigned long long)wParam);

			switch (uMsg)
			{
			case WM_ACTIVATEAPP:
			{
				if (!wParam)
					CKPE_CDockInfoAbortDrag();

				break;
			}
			case WM_SETTEXT:
			{
				RedrawWindow((HWND)hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
				break;
			}
			case WM_WINDOWPOSCHANGED:
			{
				if (pFrame->HasFlag(DockingFrameWindow::EF_ANCHOR) &&
					!pFrame->HasFlag(DockingFrameWindow::EF_LAYOUT_SWEPT))
				{
					pFrame->SetFlag(DockingFrameWindow::EF_LAYOUT_SWEPT);

					auto mgr = Common::Interface::GetSingleton()->GetDockingManager();
					if (mgr)
					{
						for (std::size_t i = 0; i < mgr->Count(); i++)
						{
							auto wnd = (DockingFrameWindow*)mgr->AtByIndex(i);
							if (wnd && !wnd->HasFlag(DockingFrameWindow::EF_ANCHOR))
								CKPE_CDockingTryRestoreSavedZone(hWnd, (HWND)wnd->GetWindow());
						}
					}
				}

				if (!s_InSplitterSync)
				{
					for (auto& entry : CKPE_AnchorDockedPanels)
					{
						for (auto& panel : entry.second)
						{
							if (panel.Wnd != hWnd)
								continue;

							auto oldRect = panel.Rect;
							auto newRect = CKPE_CDockingGetVisibleRect(hWnd);
							panel.Rect = newRect;

							constexpr LONG kTolerance = 4;
							auto leftMoved = newRect.left != oldRect.left;
							auto rightMoved = newRect.right != oldRect.right;
							auto topMoved = newRect.top != oldRect.top;
							auto bottomMoved = newRect.bottom != oldRect.bottom;

							if (leftMoved || rightMoved || topMoved || bottomMoved)
							{
								s_InSplitterSync = true;

								for (auto& other : entry.second)
								{
									if (other.Wnd == hWnd || !IsWindow(other.Wnd))
										continue;

									auto otherNew = other.Rect;
									bool changed = false;

									if (leftMoved && (std::abs(other.Rect.right - oldRect.left) <= kTolerance))
									{
										otherNew.right = newRect.left;
										changed = true;
									}
									if (rightMoved && (std::abs(other.Rect.left - oldRect.right) <= kTolerance))
									{
										otherNew.left = newRect.right;
										changed = true;
									}

									bool horizontallyOverlaps = (other.Rect.left < oldRect.right) &&
										(other.Rect.right > oldRect.left);

									if (topMoved && horizontallyOverlaps &&
										(std::abs(other.Rect.bottom - oldRect.top) <= kTolerance))
									{
										otherNew.bottom = newRect.top;
										changed = true;
									}
									if (bottomMoved && horizontallyOverlaps &&
										(std::abs(other.Rect.top - oldRect.bottom) <= kTolerance))
									{
										otherNew.top = newRect.bottom;
										changed = true;
									}

									if (leftMoved && (std::abs(other.Rect.left - oldRect.left) <= kTolerance))
									{
										otherNew.left = newRect.left;
										changed = true;
									}
									if (rightMoved && (std::abs(other.Rect.right - oldRect.right) <= kTolerance))
									{
										otherNew.right = newRect.right;
										changed = true;
									}

									if (changed)
									{
										CKPE_CDockingFrameClampToMinSize(other.Wnd, otherNew, other.Zone);
										CKPE_CDockingFrameSetVisibleRect(other.Wnd, otherNew);
										other.Rect = otherNew;
									}
								}

								s_InSplitterSync = false;
							}

							break;
						}
					}
				}

				break;
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

				if ((wParam == HTCAPTION) && !pFrame->HasFlag(DockingFrameWindow::EF_ANCHOR))
				{
					CKPE_DockInfo.MousePos.x = GET_X_LPARAM(lParam);
					CKPE_DockInfo.MousePos.y = GET_Y_LPARAM(lParam);
					CKPE_DockInfo.DraggingDockWnd = hWnd;

					for (auto& entry : CKPE_AnchorDockedPanels)
					{
						CKPE_CDockingPruneAnchorPanels(entry.first, hWnd);
						CKPE_CDockingSaveLayout(entry.first);
					}
					GetWindowRect(hWnd, &CKPE_DockInfo.DragRecPlacement);
					if (pFrame->HasDocking())
					{
						return S_OK;
					}
				}

				break;
			}

			case WM_CANCELMODE:
			case WM_LBUTTONUP:
			{
				CKPE_CDockInfoFinishDrag(hWnd, uMsg == WM_CANCELMODE);
				break;
			}
			case WM_EXITSIZEMOVE:
			{
				CKPE_CDockInfoFinishDrag(hWnd, false);
				break;
			}
			case WM_MOVING:
			{
				if (CKPE_DockInfo.DraggingDockWnd && (CKPE_DockInfo.DraggingDockWnd == hWnd))
				{
					auto lprc = (LPRECT)lParam;

					POINT cursor{};
					GetCursorPos(&cursor);

					auto target = CKPE_CDockingFrameFindHoverTarget(hWnd, cursor);

					//static DockingFrameWindow* s_lastTarget = nullptr;
					//if (target != s_lastTarget)
					//{
					//	s_lastTarget = target;
					//	char szTargetText[64]{ 0 };
					//	if (target)
					//		GetWindowTextA((HWND)target->GetWindow(), szTargetText, sizeof(szTargetText));
					//	_MESSAGE("[dock-diag] hover target changed -> %s", target ? szTargetText : "(none)");
					//}

					RECT zoneRect{};
					auto zone = target ?
						CKPE_CDockingFrameComputeDropZone(target, cursor, zoneRect) :
						DockingFrameWindow::EF_NONE;

					if (zone != DockingFrameWindow::EF_NONE)
					{
						auto targetHwnd = (HWND)target->GetWindow();

						CKPE_CDockingFrameClampToMinSize(hWnd, zoneRect, zone);
						CKPE_DockInfo.HoverWnd->Show(zoneRect.left, zoneRect.top, zoneRect.right, zoneRect.bottom);

						CKPE_DockInfo.State |= DS_MOUSEMOVED;
						CKPE_DockInfo.ContainerWnd = targetHwnd;
						CKPE_DockInfo.DragRecPlacement = *lprc;
					}
					else
					{
						CKPE_DockInfo.HoverWnd->Hide();
						CKPE_DockInfo.State &= ~DS_MOUSEMOVED;
						CKPE_DockInfo.ContainerWnd = NULL;
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
			case E_ANCHOR:
				DockWnd = new DockingFrameWindow(hWnd);
				DockWnd->SetFlag(DockingFrameWindow::EF_ANCHOR);
				break;
			default:
				break;
			}

			if (DockWnd)
			{
				_Container->insert({hWnd, DockWnd });
				if (style == E_FRAME)
				{
					for (auto& entry : *_Container)
					{
						if (entry.second && entry.second->HasFlag(DockingFrameWindow::EF_ANCHOR) &&
							entry.second->HasFlag(DockingFrameWindow::EF_LAYOUT_SWEPT))
						{
							CKPE_CDockingTryRestoreSavedZone((HWND)entry.first, (HWND)hWnd);
							break;
						}
					}
				}

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