// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <vssym32.h>
#include <richedit.h>
#include <dwmapi.h>
#include <CKPE.Application.h>
#include <CKPE.Exception.h>
#include <CKPE.Detours.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Graphics.h>
#include <CKPE.HardwareInfo.h>
#include <algorithm>
#include <array>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UICheckBox.h>
#include <CKPE.Common.UIComboBox.h>
#include <CKPE.Common.UICustomBox.h>
#include <CKPE.Common.UIEditText.h>
#include <CKPE.Common.UIGroupBox.h>
#include <CKPE.Common.UIHeader.h>
#include <CKPE.Common.UIListBox.h>
#include <CKPE.Common.UIListView.h>
#include <CKPE.Common.UIMDIClient.h>
#include <CKPE.Common.UIMemo.h>
#include <CKPE.Common.UIPageControl.h>
#include <CKPE.Common.UIPopupMenu.h>
#include <CKPE.Common.UIProgressBar.h>
#include <CKPE.Common.UIPushButton.h>
#include <CKPE.Common.UIRadioButton.h>
#include <CKPE.Common.UIRichEdit20.h>
#include <CKPE.Common.UIScrollBar.h>
#include <CKPE.Common.UIStatusBar.h>
#include <CKPE.Common.UIToolBar.h>
#include <CKPE.Common.UITrackBar.h>
#include <CKPE.Common.UITreeView.h>
#include <CKPE.Common.UIUpDown.h>

namespace CKPE
{
	namespace Common
	{
		static ModernTheme* _smoderntheme = nullptr;

		// see https://github.com/adzm/win32-custom-menubar-aero-theme/blob/main/UAHMenuBar.h

		// window messages related to menu bar drawing
		#define WM_UAHDESTROYWINDOW    0x0090	// handled by DefWindowProc
		#define WM_UAHDRAWMENU         0x0091	// lParam is UAHMENU
		#define WM_UAHDRAWMENUITEM     0x0092	// lParam is UAHDRAWMENUITEM
		#define WM_UAHINITMENU         0x0093	// handled by DefWindowProc
		#define WM_UAHMEASUREMENUITEM  0x0094	// lParam is UAHMEASUREMENUITEM
		#define WM_UAHNCPAINTMENUPOPUP 0x0095	// handled by DefWindowProc

		// describes the sizes of the menu bar or menu item
		typedef union tagUAHMENUITEMMETRICS
		{
			// cx appears to be 14 / 0xE less than rcItem's width!
			// cy 0x14 seems stable, i wonder if it is 4 less than rcItem's height which is always 24 atm
			struct {
				DWORD cx;
				DWORD cy;
			} rgsizeBar[2];
			struct {
				DWORD cx;
				DWORD cy;
			} rgsizePopup[4];
		} UAHMENUITEMMETRICS;

		// not really used in our case but part of the other structures
		typedef struct tagUAHMENUPOPUPMETRICS
		{
			DWORD rgcx[4];
			DWORD fUpdateMaxWidths : 2; // from kernel symbols, padded to full dword
		} UAHMENUPOPUPMETRICS;

		// hmenu is the main window menu; hdc is the context to draw in
		typedef struct tagUAHMENU
		{
			HMENU hmenu;
			HDC hdc;
			DWORD dwFlags; // no idea what these mean, in my testing it's either 0x00000a00 or sometimes 0x00000a10
		} UAHMENU;

		// menu items are always referred to by iPosition here
		typedef struct tagUAHMENUITEM
		{
			int iPosition; // 0-based position of menu item in menubar
			UAHMENUITEMMETRICS umim;
			UAHMENUPOPUPMETRICS umpm;
		} UAHMENUITEM;

		// the DRAWITEMSTRUCT contains the states of the menu items, as well as
		// the position index of the item in the menu, which is duplicated in
		// the UAHMENUITEM's iPosition as well
		typedef struct UAHDRAWMENUITEM
		{
			DRAWITEMSTRUCT dis; // itemID looks uninitialized
			UAHMENU um;
			UAHMENUITEM umi;
		} UAHDRAWMENUITEM;

		// the MEASUREITEMSTRUCT is intended to be filled with the size of the item
		// height appears to be ignored, but width can be modified
		typedef struct tagUAHMEASUREMENUITEM
		{
			MEASUREITEMSTRUCT mis;
			UAHMENU um;
			UAHMENUITEM umi;
		} UAHMEASUREMENUITEM;

		static void UAHDrawMenuNCBottomLine(HWND hWnd, HBRUSH hbrBarBackground)
		{
			MENUBARINFO mbi = { sizeof(mbi) };
			if (!GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi))
				return;

			RECT rcClient = { 0 };
			GetClientRect(hWnd, &rcClient);
			MapWindowPoints(hWnd, nullptr, (POINT*)&rcClient, 2);

			RECT rcWindow = { 0 };
			GetWindowRect(hWnd, &rcWindow);
			OffsetRect(&rcClient, -rcWindow.left, -rcWindow.top);

			// the rcBar is offset by the window rect
			RECT rcAnnoyingLine = rcClient;
			rcAnnoyingLine.bottom = rcAnnoyingLine.top;
			rcAnnoyingLine.top--;

			HDC hdc = GetWindowDC(hWnd);
			FillRect(hdc, &rcAnnoyingLine, hbrBarBackground);
			ReleaseDC(hWnd, hdc);
		}

		std::unordered_set<std::string_view, std::hash<std::string_view>, std::equal_to<std::string_view>> 
			_spermanentWindowSubclasses
		{
			// Main class Creation Kit
			"Creation Kit",
			// Classes CK 
			"ActivatorClass",
			"AlchemyClass",
			"ArmorClass",
			"CreatureClass",
			"LockPickClass",
			"NPCClass",
			"WeaponClass",
			"FaceClass",
			"PlaneClass",
			"MonitorClass",
			"ViewerClass",
			"SpeakerClass",
			"LandClass",
			"TimeOfDayClass",
			"DownloadClass",
			"TreatAsClass",
			"ViewControlClass",
			"TreeviewToolTipClass",
			// Windows
			"Render Window",
			// Windows Popup
			"#32770",
		};

		enum class ThemeType {
			None,
			ScrollBar,
			StatusBar,
			MDIClient,
			Static,
			Edit,
			RichEdit20,
			RichEdit50,
			Button,
			ComboBox,
			Header,
			ListBox,
			ListView,
			TreeView,
			TabControl,
			ToolBar,
			TrackBar,
			ProgressBar,
			PopupMenu,
			Spin,
			PropGrid,
			Tooltips
		};

		// List of created windows
		concurrency::concurrent_unordered_map<HWND, BOOL, std::hash<HTHEME>, std::equal_to<HTHEME>> WindowHandles;
		// List of registered visual styles themes
		concurrency::concurrent_unordered_map<HTHEME, ThemeType, std::hash<HTHEME>, std::equal_to<HTHEME>> ThemeHandles;

		static HHOOK g_hkWndProcModernThemeHandle = nullptr;
		static WNDPROC OldPopupMenuWndClass = nullptr;
		static Font* listFont = nullptr;
		static HBRUSH g_brItemBackground = nullptr;
		static HBRUSH g_brItemBackgroundHot = nullptr;
		static HBRUSH g_brItemBackgroundSelected = nullptr;
		static HICON g_AppIcon = nullptr;

		struct string_equal_to {
			inline bool operator()(const std::string_view& lhs, const std::string_view& rhs) const
			{
				return !_stricmp(lhs.data(), rhs.data());
			}
		};

		static ThemeType GetThemeTypeFromWindow(HWND hWindow)
		{
			static std::unordered_map<std::string_view, ThemeType, std::hash<std::string_view>, 
				string_equal_to> TargetWindowThemes
			{
			{ STATUSCLASSNAMEA, ThemeType::StatusBar },
			{ "mdiclient", ThemeType::MDIClient },
			{ WC_STATICA, ThemeType::Static },
			{ UPDOWN_CLASS, ThemeType::Spin },
			{ WC_EDITA, ThemeType::Edit },
			{ RICHEDIT_CLASSA, ThemeType::RichEdit20 },
			{ "RICHEDIT50W", ThemeType::RichEdit50 },
			{ PROGRESS_CLASSA, ThemeType::ProgressBar },
			{ WC_BUTTONA, ThemeType::Button },
			{ WC_LISTBOXA, ThemeType::ListBox },
			{ WC_COMBOBOXA, ThemeType::ComboBox },
			{ WC_COMBOBOXEXA, ThemeType::ComboBox },
			{ WC_HEADERA, ThemeType::Header },
			{ WC_LISTVIEWA, ThemeType::ListView },
			{ WC_TREEVIEWA, ThemeType::TreeView },
			{ WC_TABCONTROLA, ThemeType::TabControl },
			{ TOOLBARCLASSNAMEA, ThemeType::ToolBar },
			{ TRACKBAR_CLASSA, ThemeType::TrackBar },
			{ TOOLTIPS_CLASSA, ThemeType::Tooltips },
			{ "PropGridCtl", ThemeType::PropGrid },
			{ "EditorPropertyGridCtl", ThemeType::PropGrid },
			{ "#32768", ThemeType::PopupMenu },
			};

			auto themeType = ThemeType::None;
			UI::CUICustomWindow Window = hWindow;

			if (auto itr = TargetWindowThemes.find(Window.GetName()); itr != TargetWindowThemes.end())
				themeType = itr->second;

			return themeType;
		}

		// Binds the specified class type to the visual theme
		// Returns TRUE if successful
		static bool RegisterThemeHandle(HTHEME hTheme, ThemeType eTheme) {
			if (ThemeType::None == eTheme)
				return false;

			for (auto it = ThemeHandles.begin(); it != ThemeHandles.end(); it++) 
				if ((*it).second == eTheme)
					return false;

			ThemeHandles.insert(std::make_pair(hTheme, eTheme));

			return true;
		}

		// Binds the specified class type to the visual theme. hWindow takes only HTHEME
		// Returns TRUE if successful
		static bool RegisterThemeHandle(HWND hWindow, ThemeType eTheme) {
			HTHEME windowTheme = GetWindowTheme(hWindow);
			if (!windowTheme)
				return FALSE;

			return RegisterThemeHandle(windowTheme, eTheme);
		}

		namespace APIHook {
			static COLORREF WINAPI SetTextColor(HDC hdc, COLORREF color) noexcept(true)
			{
				return ::SetTextColor(hdc, !color ? UI::GetThemeSysColor(UI::ThemeColor_Text_4) : color);
			}

			static BOOL WINAPI DrawEdge(HDC hdc, LPRECT qrc, UINT edge, UINT grfFlags) noexcept(true)
			{
				if (!qrc || !hdc)
					return FALSE;

				Canvas canvas = hdc;

				if ((grfFlags & BF_MIDDLE) == BF_MIDDLE)
					canvas.Fill(*qrc, UI::GetThemeSysColor(UI::ThemeColor_TreeView_Color));

				canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Default);

				if ((grfFlags & BF_LEFT) == BF_LEFT) {
					canvas.MoveTo(qrc->left, qrc->top);
					canvas.LineTo(qrc->left, qrc->bottom - 1);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->left++;
				}

				if ((grfFlags & BF_TOP) == BF_TOP) {
					canvas.MoveTo(qrc->left, qrc->top);
					canvas.LineTo(qrc->right, qrc->top);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->top++;
				}

				if ((grfFlags & BF_RIGHT) == BF_RIGHT) {
					canvas.MoveTo(qrc->right, qrc->top);
					canvas.LineTo(qrc->right, qrc->bottom - 1);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->right--;
				}

				if ((grfFlags & BF_BOTTOM) == BF_BOTTOM) {
					canvas.MoveTo(qrc->left, qrc->bottom - 1);
					canvas.LineTo(qrc->right, qrc->bottom - 1);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->bottom--;
				}

				if ((grfFlags & BF_DIAGONAL_ENDBOTTOMLEFT) == BF_DIAGONAL_ENDBOTTOMLEFT) {
					canvas.MoveTo(qrc->right, qrc->top);
					canvas.LineTo(qrc->left, qrc->bottom - 1);
				}

				if ((grfFlags & BF_DIAGONAL_ENDBOTTOMRIGHT) == BF_DIAGONAL_ENDBOTTOMRIGHT) {
					canvas.MoveTo(qrc->left, qrc->top);
					canvas.LineTo(qrc->right, qrc->bottom - 1);
				}

				if ((grfFlags & BF_DIAGONAL_ENDTOPLEFT) == BF_DIAGONAL_ENDTOPLEFT) {
					canvas.MoveTo(qrc->right, qrc->bottom - 1);
					canvas.LineTo(qrc->left, qrc->top);
				}

				if ((grfFlags & BF_DIAGONAL_ENDTOPRIGHT) == BF_DIAGONAL_ENDTOPRIGHT) {
					canvas.MoveTo(qrc->left, qrc->bottom - 1);
					canvas.LineTo(qrc->right, qrc->top);
				}

				return TRUE;
			}

			static HFONT WINAPI CreateFontA(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight,
				DWORD  bItalic, DWORD  bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
				DWORD  iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName) noexcept(true)
			{
				auto f = UI::ThemeData::GetSingleton()->ThemeFont;

				return (HFONT)::CreateFontA(f->Height, 0, cEscapement, cOrientation, cWeight, bItalic, bUnderline, 
					bStrikeOut, _READ_OPTION_INT("CreationKit", "nCharset", DEFAULT_CHARSET), iOutPrecision, 
					iClipPrecision, CLEARTYPE_NATURAL_QUALITY, VARIABLE_PITCH, f->GetName().c_str());
			}

			static DWORD Comctl32GetSysColor(INT nIndex) noexcept(true)
			{
				return (DWORD)UI::Comctl32GetSysColor(nIndex);
			}

			static HBRUSH Comctl32GetSysColorBrush(INT nIndex) noexcept(true)
			{
				return (HBRUSH)UI::Comctl32GetSysColorBrush(nIndex);
			}

			static HRESULT Comctl32DrawThemeText(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId,
				LPCWSTR pszText, INT cchText, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect) noexcept(true)
			{
				auto themeType = ThemeType::None;
				if (auto itr = ThemeHandles.find(hTheme); itr != ThemeHandles.end())
					themeType = itr->second;

				if (themeType == ThemeType::StatusBar)
				{
					// StatusBar flicker...
					// To completely get rid of a lot of renderings, it looks like commctrl does not have double buffering, as I was told.

					// Enabling double buffering

					RECT rc = { 0, 0, (pRect->right - pRect->left) + 6, pRect->bottom - pRect->top };
					HDC hdcMem = CreateCompatibleDC(hdc);
					HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
					HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

					Canvas canvas(hdcMem);

					auto f = UI::ThemeData::GetSingleton()->ThemeFont;
					UI::StatusBar::Render::DrawBackground(canvas, &rc);

					canvas.TransparentMode = true;
					canvas.Font.Assign(*f);

					std::uint32_t ff = dwTextFlags;
					UI::StatusBar::Event::OnBeforeDrawText(canvas, ff);

					canvas.TextRect(rc, pszText, ff, cchText);
					canvas.TransparentMode = false;

					BitBlt(hdc, pRect->left - 3, pRect->top, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

					SelectObject(hdcMem, hbmOld);
					DeleteObject(hbmMem);
					DeleteDC(hdcMem);
				}
				else
				{
					// detected standart OS theme (comdlg32)
					COLORREF clTest = GetPixel(hdc, pRect->left + ((pRect->right - pRect->left) >> 1), pRect->top + 2);
					if (CLR_INVALID != clTest && ((GetRValue(clTest) + GetGValue(clTest) + GetBValue(clTest)) / 3) > 128)
					{
						auto Ret = DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText, cchText, dwTextFlags, dwTextFlags2, pRect);
						return Ret;
					}

					RECT rc = *pRect;
					std::uint32_t ff = dwTextFlags;

					auto f = UI::ThemeData::GetSingleton()->ThemeFont;

					Canvas canvas(hdc);
					canvas.TransparentMode = true;
					canvas.Font.Assign(*f);

					switch (themeType) {
					case ThemeType::TabControl:
						UI::PageControl::Event::OnBeforeDrawText(canvas, ff);
						break;
					case ThemeType::Button:
					{
						if (iPartId == BP_PUSHBUTTON)
							UI::PushButton::Event::OnBeforeDrawText(canvas, ff, iStateId);
						else if (iPartId == BP_CHECKBOX)
						{
							rc.right += 10; // eliminate end "..." when this is not necessary
							UI::CheckBox::Event::OnBeforeDrawText(canvas, ff, iStateId);
						}
						else if (iPartId == BP_RADIOBUTTON)
						{
							rc.right += 10; // eliminate end "..." when this is not necessary
							UI::RadioButton::Event::OnBeforeDrawText(canvas, ff, iStateId);
						}
						else if (iPartId == BP_GROUPBOX)
						{
							rc.right += 10; // eliminate end "..." when this is not necessary
							UI::GroupBox::Event::OnBeforeDrawText(canvas, ff);
						}

					}
					break;
					case ThemeType::ComboBox:
						UI::ComboBox::Event::OnBeforeDrawText(canvas, ff, iStateId);
						break;
					default:
						canvas.ColorText = UI::GetThemeSysColor(UI::ThemeColor_Text_4);
						break;
					}

					canvas.TextRect(rc, pszText, ff, cchText);
					canvas.TransparentMode = false;
				}

				return S_OK;
			}

			static HRESULT WINAPI Comctl32DrawThemeTextEx(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId,
				LPCWSTR pszText, INT cchText, DWORD dwTextFlags, LPRECT pRect, const DTTOPTS* pOptions)
			{
				return Comctl32DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText, cchText, dwTextFlags, 0, pRect);
			}

			static HRESULT WINAPI Comctl32DrawThemeBackground(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId,
				LPCRECT pRect, LPCRECT pClipRect)
			{
				auto themeType = ThemeType::None;

				if (auto itr = ThemeHandles.find(hTheme); itr != ThemeHandles.end())
					themeType = itr->second;

				Canvas canvas(hdc);

				if (themeType == ThemeType::ScrollBar)
				{
					switch (iPartId) {
					case SBP_THUMBBTNHORZ:	// Horizontal drag bar
					{
						if (iStateId == SCRBS_HOT || iStateId == SCRBS_PRESSED)
							UI::ScrollBar::Render::DrawSliderHorz_Focused(canvas, pRect);
						else
							UI::ScrollBar::Render::DrawSliderHorz_Normal(canvas, pRect);
					}
					return S_OK;
					case SBP_THUMBBTNVERT:	// Vertical drag bar
					{
						if (iStateId == SCRBS_HOT || iStateId == SCRBS_PRESSED)
							UI::ScrollBar::Render::DrawSliderVert_Focused(canvas, pRect);
						else
							UI::ScrollBar::Render::DrawSliderVert_Normal(canvas, pRect);
					}
					return S_OK;

					case SBP_LOWERTRACKHORZ:// Horizontal background
					case SBP_UPPERTRACKHORZ:// Horizontal background
						UI::ScrollBar::Render::DrawBackgroundHorz(canvas, pRect);
						return S_OK;

					case SBP_LOWERTRACKVERT:// Vertical background
					case SBP_UPPERTRACKVERT:// Vertical background
						UI::ScrollBar::Render::DrawBackgroundVert(canvas, pRect);
						return S_OK;

					case SBP_ARROWBTN:		// Arrow button
					{
						// Assume the perspective of the arrow pointing upward ( /\ ) in GDI coordinates. NOTE: (0, 0) is the
						// top left corner of the screen. Awful code, but it works.
						const INT arrowWidth = (INT)std::ceil(std::abs(pRect->left - pRect->right) * 0.4f);
						const INT arrowHeight = (INT)std::ceil(std::abs(pRect->top - pRect->bottom) * 0.35f);

						std::array<DWORD, 6> counts{ 2, 2, 2, 2, 2, 2 };
						std::array<POINT, 12> verts
						{ {
								// Left segment
								{ 0, -0 },
								{ (arrowWidth / 2) + 1, -arrowHeight + 2 },

								{ 0, -1 },
								{ (arrowWidth / 2) + 1, -arrowHeight + 1 },

								{ 0, -2 },
								{ (arrowWidth / 2) + 1, -arrowHeight + 0 },

								// Right segment (final vertex Y adjusted to avoid a stray pixel)
								{ arrowWidth - 1, -0 },
								{ arrowWidth / 2, -arrowHeight + 2 },

								{ arrowWidth - 1, -1 },
								{ arrowWidth / 2, -arrowHeight + 1 },

								{ arrowWidth - 1, -2 },
								{ arrowWidth / 2, -arrowHeight + 1 },
							} };

						BOOL isHot = FALSE;
						BOOL isDisabled = FALSE;
						BOOL isHorz = FALSE;

						for (auto& vert : verts)
						{
							switch (iStateId)
							{
							case ABS_UPHOT:// Up
							case ABS_UPPRESSED:
								isHot = TRUE;
							case ABS_UPDISABLED:
								isDisabled = TRUE;
							case ABS_UPNORMAL:
							case ABS_UPHOVER:
								vert.x += pRect->left + arrowHeight - 1;
								vert.y += pRect->bottom - arrowHeight;
								break;

							case ABS_DOWNHOT:// Down
							case ABS_DOWNPRESSED:
								isHot = TRUE;
							case ABS_DOWNDISABLED:
								isDisabled = TRUE;
							case ABS_DOWNNORMAL:
							case ABS_DOWNHOVER:
								vert.x += pRect->left + arrowHeight - 1;
								vert.y = -vert.y + pRect->top + arrowHeight - 1;
								break;

							case ABS_LEFTHOT:// Left
							case ABS_LEFTPRESSED:
								isHot = TRUE;
							case ABS_LEFTDISABLED:
								isDisabled = TRUE;
							case ABS_LEFTNORMAL:
							case ABS_LEFTHOVER:
								isHorz = TRUE;
								std::swap(vert.x, vert.y);
								vert.x += pRect->right - arrowHeight;
								vert.y += pRect->top + arrowHeight - 1;
								break;

							case ABS_RIGHTHOT:// Right
							case ABS_RIGHTPRESSED:
								isHot = TRUE;
							case ABS_RIGHTDISABLED:
								isDisabled = TRUE;
							case ABS_RIGHTNORMAL:
							case ABS_RIGHTHOVER:
								isHorz = TRUE;
								std::swap(vert.x, vert.y);
								vert.x = -vert.x + pRect->left + arrowHeight - 1;
								vert.y += pRect->top + arrowHeight - 1;
								break;
							}
						}

						if (isHot)
						{
							UI::ScrollBar::Render::DrawButton_Hot(canvas, pRect);
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape);
						}
						else if (isDisabled)
						{
							UI::ScrollBar::Render::DrawButton_Disabled(canvas, pRect);
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape_Disabled);
						}
						else
						{
							if (isHorz)
								UI::ScrollBar::Render::DrawBackgroundHorz(canvas, pRect);
							else
								UI::ScrollBar::Render::DrawBackgroundVert(canvas, pRect);

							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape);
						}

						if (isDisabled)
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape_Shadow_Disabled);
						else
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape_Shadow);

						PolyPolyline((HDC)canvas.Handle, verts.data(), counts.data(), (DWORD)counts.size());

						if (isHot)
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape);
						else if (isDisabled)
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape_Disabled);
						else
							canvas.Pen.Color = UI::GetThemeSysColor(UI::ThemeColor_Shape);

						for (auto i = verts.begin(); i != verts.end(); i++)
							(*i).y++;

						PolyPolyline((HDC)canvas.Handle, verts.data(), counts.data(), (DWORD)counts.size());
					}
					return S_OK;

					case SBP_GRIPPERHORZ:	// Horizontal resize scrollbar
					case SBP_GRIPPERVERT:	// Vertical resize scrollbar
						break;
					case SBP_SIZEBOX:		// Resize box, bottom right
					case SBP_SIZEBOXBKGND:	// Resize box, background, unused
						canvas.Fill(*pRect, UI::GetThemeSysColor(UI::ThemeColor_Default));
						return S_OK;
					}
				}
				else if (themeType == ThemeType::StatusBar)
				{
					switch (iPartId)
					{
					case 0:
						// Outside border (top, right)
						// 
						// In fact, the whole window... it is necessary to avoid drawing the text area
						UI::StatusBar::Render::DrawBorder(canvas, pRect);
						return S_OK;

					case SP_PANE:
					case SP_GRIPPERPANE:
					case SP_GRIPPER:
						// Everything else
						// 
						// flicker.... I will draw along with the text
						//Theme::StatusBar::Render::DrawBackground(Canvas, pRect);
						return S_OK;
					}
				}
				else if (themeType == ThemeType::Spin)
				{
					switch (iPartId)
					{
					case SPNP_UP:
					{
						switch (iStateId)
						{
						case DNS_HOT:
							UI::UpDown::Render::DrawUp_Hot(canvas, pRect);
							break;
						case DNS_PRESSED:
							UI::UpDown::Render::DrawUp_Pressed(canvas, pRect);
							break;
						case DNS_DISABLED:
							UI::UpDown::Render::DrawUp_Disabled(canvas, pRect);
							break;
						default:
							UI::UpDown::Render::DrawUp_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case SPNP_DOWN:
					{
						switch (iStateId)
						{
						case UPS_HOT:
							UI::UpDown::Render::DrawDown_Hot(canvas, pRect);
							break;
						case UPS_PRESSED:
							UI::UpDown::Render::DrawDown_Pressed(canvas, pRect);
							break;
						case UPS_DISABLED:
							UI::UpDown::Render::DrawDown_Disabled(canvas, pRect);
							break;
						default:
							UI::UpDown::Render::DrawDown_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case SPNP_UPHORZ:
					{
						switch (iStateId)
						{
						case UPHZS_HOT:
							UI::UpDown::Render::DrawRight_Hot(canvas, pRect);
							break;
						case UPHZS_PRESSED:
							UI::UpDown::Render::DrawRight_Pressed(canvas, pRect);
							break;
						case UPHZS_DISABLED:
							UI::UpDown::Render::DrawRight_Disabled(canvas, pRect);
							break;
						default:
							UI::UpDown::Render::DrawRight_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case SPNP_DOWNHORZ:
					{
						switch (iStateId)
						{
						case DNHZS_HOT:
							UI::UpDown::Render::DrawLeft_Hot(canvas, pRect);
							break;
						case DNHZS_PRESSED:
							UI::UpDown::Render::DrawLeft_Pressed(canvas, pRect);
							break;
						case DNHZS_DISABLED:
							UI::UpDown::Render::DrawLeft_Disabled(canvas, pRect);
							break;
						default:
							UI::UpDown::Render::DrawLeft_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;
					}
				}
				else if (themeType == ThemeType::Edit)
				{
					switch (iPartId)
					{
					case EP_EDITBORDER_NOSCROLL:
					{
						switch (iStateId)
						{
						case ETS_DISABLED:
							UI::EditText::Render::DrawEditText_Disabled(canvas, pRect);
							break;
						case ETS_SELECTED:
						case ETS_FOCUSED:
							UI::EditText::Render::DrawEditText_Focused(canvas, pRect);
							break;
						case ETS_HOT:
							UI::EditText::Render::DrawEditText_Hot(canvas, pRect);
							break;
						default:
							UI::EditText::Render::DrawEditText_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case EP_EDITBORDER_VSCROLL:
					{
						switch (iStateId)
						{
						case EPSV_DISABLED:
							UI::EditText::Render::DrawEditText_Disabled(canvas, pRect);
							break;
						case EPSV_FOCUSED:
							UI::EditText::Render::DrawEditText_Focused(canvas, pRect);
							break;
						case EPSV_HOT:
							UI::EditText::Render::DrawEditText_Hot(canvas, pRect);
							break;
						default:
							UI::EditText::Render::DrawEditText_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case EP_EDITBORDER_HSCROLL:
					{
						switch (iStateId)
						{
						case EPSH_DISABLED:
							UI::EditText::Render::DrawEditText_Disabled(canvas, pRect);
							break;
						case EPSH_FOCUSED:
							UI::EditText::Render::DrawEditText_Focused(canvas, pRect);
							break;
						case EPSH_HOT:
							UI::EditText::Render::DrawEditText_Hot(canvas, pRect);
							break;
						default:
							UI::EditText::Render::DrawEditText_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case EP_EDITBORDER_HVSCROLL:
					{
						switch (iStateId)
						{
						case EPSHV_DISABLED:
							UI::EditText::Render::DrawEditText_Disabled(canvas, pRect);
							break;
						case EPSHV_FOCUSED:
							UI::EditText::Render::DrawEditText_Focused(canvas, pRect);
							break;
						case EPSHV_HOT:
							UI::EditText::Render::DrawEditText_Hot(canvas, pRect);
							break;
						default:
							UI::EditText::Render::DrawEditText_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;
					}
				}
				else if (themeType == ThemeType::ProgressBar)
				{
					switch (iPartId)
					{
					case PP_BAR:
					case PP_TRANSPARENTBAR:
						UI::ProgressBar::Render::DrawBar(canvas, pRect);
						return S_OK;
					case PP_FILL:
						UI::ProgressBar::Render::DrawFill(canvas, pRect);
						return S_OK;
					}
					// skip other
					return S_OK;
				}
				else if (themeType == ThemeType::Button)
				{
					switch (iPartId)
					{
					case BP_PUSHBUTTON:
					{
						switch (iStateId)
						{
						case PBS_HOT:
							UI::PushButton::Render::DrawPushButton_Hot(canvas, pRect);
							break;
						case PBS_DISABLED:
							UI::PushButton::Render::DrawPushButton_Disabled(canvas, pRect);
							break;
						case PBS_PRESSED:
							UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
							break;
						default:
							UI::PushButton::Render::DrawPushButton_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case BP_GROUPBOX: {
						switch (iStateId) {
						case GBS_DISABLED:
							UI::GroupBox::Render::DrawGroupBox_Disabled(canvas, pRect);
							break;
						default:
							UI::GroupBox::Render::DrawGroupBox_Normal(canvas, pRect);
							break;
						}
					}
									return S_OK;

					case BP_CHECKBOX:
					{
						switch (iStateId)
						{
						case CBS_UNCHECKEDDISABLED:
							UI::PushButton::Render::DrawPushButton_Disabled(canvas, pRect);
							break;
						case CBS_UNCHECKEDNORMAL:
							UI::PushButton::Render::DrawPushButton_Normal(canvas, pRect);
							break;
						case CBS_UNCHECKEDHOT:
							UI::PushButton::Render::DrawPushButton_Hot(canvas, pRect);
							break;
						case CBS_UNCHECKEDPRESSED:
							UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
							break;
						case CBS_CHECKEDDISABLED:
							UI::PushButton::Render::DrawPushButton_Disabled(canvas, pRect);
							UI::CheckBox::Render::DrawCheck_Disabled(canvas, pRect);
							break;
						case CBS_CHECKEDNORMAL:
							if (UI::GetTheme() == UI::Theme_NightBlue)
								UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
							else
								UI::PushButton::Render::DrawPushButton_Normal(canvas, pRect);
							UI::CheckBox::Render::DrawCheck_Normal(canvas, pRect);
							break;
						case CBS_CHECKEDHOT:
							if (UI::GetTheme() == UI::Theme_NightBlue)
							{
								UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
								UI::CheckBox::Render::DrawCheck_Normal(canvas, pRect);
							}
							else
							{
								UI::PushButton::Render::DrawPushButton_Hot(canvas, pRect);
								UI::CheckBox::Render::DrawCheck_Hot(canvas, pRect);
							}
							break;
						case CBS_CHECKEDPRESSED:
							UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
							UI::CheckBox::Render::DrawCheck_Pressed(canvas, pRect);
							break;
						case CBS_MIXEDDISABLED:
							UI::PushButton::Render::DrawPushButton_Disabled(canvas, pRect);
							UI::CheckBox::Render::DrawIdeterminate_Disabled(canvas, pRect);
							break;
						case CBS_MIXEDNORMAL:
							UI::PushButton::Render::DrawPushButton_Normal(canvas, pRect);
							UI::CheckBox::Render::DrawIdeterminate_Normal(canvas, pRect);
							break;
						case CBS_MIXEDHOT:
							UI::PushButton::Render::DrawPushButton_Hot(canvas, pRect);
							UI::CheckBox::Render::DrawIdeterminate_Hot(canvas, pRect);
							break;
						case CBS_MIXEDPRESSED:
							UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
							UI::CheckBox::Render::DrawIdeterminate_Pressed(canvas, pRect);
							break;
						default:
							return DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
						}
					}
					return S_OK;

					case BP_RADIOBUTTON:
					{
						switch (iStateId)
						{
						case RBS_UNCHECKEDDISABLED:
							UI::RadioButton::Render::DrawPushButtonR_Disabled(canvas, pRect);
							break;
						case RBS_UNCHECKEDNORMAL:
							UI::RadioButton::Render::DrawPushButtonR_Normal(canvas, pRect);
							break;
						case RBS_UNCHECKEDHOT:
							UI::RadioButton::Render::DrawPushButtonR_Hot(canvas, pRect);
							break;
						case RBS_UNCHECKEDPRESSED:
							UI::RadioButton::Render::DrawPushButtonR_Pressed(canvas, pRect);
							break;
						case RBS_CHECKEDDISABLED:
							UI::RadioButton::Render::DrawPushButtonR_Disabled(canvas, pRect);
							UI::RadioButton::Render::DrawRadioButton_Disabled(canvas, pRect);
							break;
						case RBS_CHECKEDNORMAL:
							UI::RadioButton::Render::DrawPushButtonR_Normal(canvas, pRect);
							UI::RadioButton::Render::DrawRadioButton_Normal(canvas, pRect);
							break;
						case RBS_CHECKEDHOT:
							UI::RadioButton::Render::DrawPushButtonR_Hot(canvas, pRect);
							UI::RadioButton::Render::DrawRadioButton_Hot(canvas, pRect);
							break;
						case RBS_CHECKEDPRESSED:
							UI::RadioButton::Render::DrawPushButtonR_Pressed(canvas, pRect);
							UI::RadioButton::Render::DrawRadioButton_Pressed(canvas, pRect);
							break;
						default:
							return DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
						}
					}
					return S_OK;
					}
				}
				else if (themeType == ThemeType::TrackBar)
				{
					switch (iPartId)
					{
					case TKP_TRACK:
					case TKP_TRACKVERT:
						UI::TrackBar::Render::DrawTrack(canvas, pRect);
						return S_OK;
					case TKP_THUMB:
					case TKP_THUMBVERT:
					case TKP_THUMBBOTTOM:
					case TKP_THUMBTOP:
					case TKP_THUMBLEFT:
					case TKP_THUMBRIGHT:
					{
						switch (iStateId)
						{
						case TUS_HOT:
							UI::TrackBar::Render::DrawSlider_Hot(canvas, pRect);
							break;
						case TUS_PRESSED:
							UI::TrackBar::Render::DrawSlider_Pressed(canvas, pRect);
							break;
						case TUS_FOCUSED:
							UI::TrackBar::Render::DrawSlider_Focused(canvas, pRect);
							break;
						case TUS_DISABLED:
							UI::TrackBar::Render::DrawSlider_Disabled(canvas, pRect);
							break;
						default:
							UI::TrackBar::Render::DrawSlider_Normal(canvas, pRect);
							break;
						}
					}
					}
					return S_OK;
				}
				else if (themeType == ThemeType::ComboBox)
				{
					switch (iPartId)
					{
					case CP_READONLY:			// Main control
					{
						switch (iStateId) {
						case CBRO_HOT:
							UI::PushButton::Render::DrawPushButton_Hot(canvas, pRect);
							break;
						case CBRO_DISABLED:
							UI::PushButton::Render::DrawPushButton_Disabled(canvas, pRect);
							break;
						case CBRO_PRESSED:
							UI::PushButton::Render::DrawPushButton_Pressed(canvas, pRect);
							break;
						default:
							UI::PushButton::Render::DrawPushButton_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case CP_BORDER:				// Main control with text edit
					{
						// Special case: dropdown arrow needs to be drawn
						DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

						switch (iStateId)
						{
						case CBB_HOT:
							UI::EditText::Render::DrawEditText_Hot(canvas, pRect);
							break;
						case CBB_DISABLED:
							UI::EditText::Render::DrawEditText_Disabled(canvas, pRect);
							break;
						case CBB_FOCUSED:
							UI::EditText::Render::DrawEditText_Focused(canvas, pRect);
							break;
						default:
							UI::EditText::Render::DrawEditText_Normal(canvas, pRect);
							break;
						}
					}
					return S_OK;

					case CP_DROPDOWNBUTTONRIGHT:// Dropdown arrow
					case CP_DROPDOWNBUTTONLEFT:	// Dropdown arrow
						if (iStateId == CBXS_DISABLED)
							UI::ComboBox::Render::DrawArrow_Disabled(canvas, pRect);
						else
							UI::ComboBox::Render::DrawArrow_Normal(canvas, pRect);
						return S_OK;

					case CP_DROPDOWNBUTTON:
						canvas.Fill(*pRect, RGB(255, 0, 0));
						return S_OK;
					case CP_BACKGROUND:
					case CP_TRANSPARENTBACKGROUND:
					case CP_CUEBANNER:
					case CP_DROPDOWNITEM:
						return S_OK;
					}
				}
				else if (themeType == ThemeType::Header)
				{
					switch (iPartId)
					{
					case 0:
					case HP_HEADERITEM:
					case HP_HEADERITEMLEFT:
					case HP_HEADERITEMRIGHT:
					{
						if ((iPartId == 0 && iStateId == HIS_NORMAL) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_NORMAL) ||
							(iPartId == HP_HEADERITEMLEFT && iStateId == HILS_NORMAL) ||
							(iPartId == HP_HEADERITEMRIGHT && iStateId == HIRS_NORMAL))
							UI::Header::Render::DrawBack_Normal(canvas, pRect);
						else if ((iPartId == 0 && iStateId == HIS_HOT) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_HOT) ||
							(iPartId == HP_HEADERITEMLEFT && iStateId == HILS_HOT) ||
							(iPartId == HP_HEADERITEMRIGHT && iStateId == HIRS_HOT))
							UI::Header::Render::DrawBack_Hot(canvas, pRect);
						else if ((iPartId == 0 && iStateId == HIS_PRESSED) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_PRESSED) ||
							(iPartId == HP_HEADERITEMLEFT && iStateId == HILS_PRESSED) ||
							(iPartId == HP_HEADERITEMRIGHT && iStateId == HIRS_PRESSED))
							UI::Header::Render::DrawBack_Pressed(canvas, pRect);
						else if ((iPartId == HP_HEADERITEM && iStateId == HIS_SORTEDNORMAL) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_SORTEDHOT) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_SORTEDPRESSED))
							UI::Header::Render::DrawBack_Selected(canvas, pRect);

						UI::Header::Render::DrawDiv(canvas, pRect);
					}
					return S_OK;

					case HP_HEADERSORTARROW:
					case HP_HEADERDROPDOWN:
					case HP_HEADERDROPDOWNFILTER:
						return S_OK;
					}
				}
				else if (themeType == ThemeType::TabControl)
				{
					switch (iPartId)
					{
					case TABP_PANE:
						UI::PageControl::Render::DrawBorder(canvas, pRect);
						break;
					case TABP_TABITEM:				// TCS_MULTILINE middle buttons
					case TABP_TABITEMLEFTEDGE:		// TCS_MULTILINE leftmost button
					case TABP_TABITEMRIGHTEDGE:		// TCS_MULTILINE rightmost button
					case TABP_TABITEMBOTHEDGE:		// TCS_MULTILINE ???
					case TABP_TOPTABITEM:			// Middle buttons
					case TABP_TOPTABITEMLEFTEDGE:	// Leftmost button
					case TABP_TOPTABITEMRIGHTEDGE:	// Rightmost button
					case TABP_TOPTABITEMBOTHEDGE:	// ???
					{
						CRECT rc = *pRect;

						BOOL isHover = (iPartId == TABP_TABITEM && iStateId == TIS_HOT) ||
							(iPartId == TABP_TABITEMLEFTEDGE && iStateId == TILES_HOT) ||
							(iPartId == TABP_TABITEMRIGHTEDGE && iStateId == TIRES_HOT) ||
							(iPartId == TABP_TABITEMBOTHEDGE && iStateId == TIBES_HOT) ||
							(iPartId == TABP_TOPTABITEM && iStateId == TTIS_HOT) ||
							(iPartId == TABP_TOPTABITEMLEFTEDGE && iStateId == TTILES_HOT) ||
							(iPartId == TABP_TOPTABITEMRIGHTEDGE && iStateId == TTIRES_HOT) ||
							(iPartId == TABP_TOPTABITEMBOTHEDGE && iStateId == TTIBES_HOT);

						BOOL isSelected = (iPartId == TABP_TABITEM && iStateId == TIS_SELECTED) ||
							(iPartId == TABP_TABITEMLEFTEDGE && iStateId == TILES_SELECTED) ||
							(iPartId == TABP_TABITEMRIGHTEDGE && iStateId == TIRES_SELECTED) ||
							(iPartId == TABP_TABITEMBOTHEDGE && iStateId == TIBES_SELECTED) ||
							(iPartId == TABP_TOPTABITEM && iStateId == TTIS_SELECTED) ||
							(iPartId == TABP_TOPTABITEMLEFTEDGE && iStateId == TTILES_SELECTED) ||
							(iPartId == TABP_TOPTABITEMRIGHTEDGE && iStateId == TTIRES_SELECTED) ||
							(iPartId == TABP_TOPTABITEMBOTHEDGE && iStateId == TTIBES_SELECTED);

						BOOL isDisabled = (iPartId == TABP_TABITEM && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TABITEMLEFTEDGE && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TABITEMRIGHTEDGE && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TABITEMBOTHEDGE && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TOPTABITEM && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TOPTABITEMLEFTEDGE && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TOPTABITEMRIGHTEDGE && iStateId == TIS_DISABLED) ||
							(iPartId == TABP_TOPTABITEMBOTHEDGE && iStateId == TIS_DISABLED);

						BOOL isFocused = (iPartId == TABP_TABITEM && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TABITEMLEFTEDGE && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TABITEMRIGHTEDGE && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TABITEMBOTHEDGE && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TOPTABITEM && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TOPTABITEMLEFTEDGE && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TOPTABITEMRIGHTEDGE && iStateId == TIS_FOCUSED) ||
							(iPartId == TABP_TOPTABITEMBOTHEDGE && iStateId == TIS_FOCUSED);

						if (isSelected || isFocused)
							UI::PageControl::Render::DrawButton_SelectedAndFocused(canvas, pRect);
						else if (isHover)
							UI::PageControl::Render::DrawButton_Hot(canvas, pRect);
						else if (isDisabled)
							UI::PageControl::Render::DrawButton_Disabled(canvas, pRect);
						else
							UI::PageControl::Render::DrawButton_Normal(canvas, pRect);
					}
					break;
					}
					return S_OK;
				}

				return DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
			}
		}

		static LRESULT CALLBACK WindowSubclassModernTheme(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
		{
			static HTHEME g_menuTheme = nullptr;

			switch (uMsg)
			{
			case WM_UAHDRAWMENU:
			{
				UAHMENU* pUDM = (UAHMENU*)lParam;
				RECT rc = { 0 };

				// get the menubar rect
				{
					MENUBARINFO mbi = { sizeof(mbi) };
					GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi);

					RECT rcWindow;
					GetWindowRect(hWnd, &rcWindow);

					// the rcBar is offset by the window rect
					rc = mbi.rcBar;
					OffsetRect(&rc, -rcWindow.left, -rcWindow.top);
				}

				FillRect(pUDM->hdc, &rc, APIHook::Comctl32GetSysColorBrush(COLOR_BTNFACE));
			}
			return 0;
			case WM_UAHDRAWMENUITEM:
			{
				UAHDRAWMENUITEM* pUDMI = (UAHDRAWMENUITEM*)lParam;

				HBRUSH* pbrBackground = &g_brItemBackground;

				// get the menu item string
				wchar_t menuString[256] = { 0 };
				MENUITEMINFOW mii = { sizeof(mii), MIIM_STRING };
				{
					mii.dwTypeData = menuString;
					mii.cch = (sizeof(menuString) / 2) - 1;

					GetMenuItemInfoW(pUDMI->um.hmenu, pUDMI->umi.iPosition, TRUE, &mii);
				}

				// get the item state for drawing

				DWORD dwFlags = DT_CENTER | DT_SINGLELINE | DT_VCENTER;

				int iTextStateID = 0;
				int iBackgroundStateID = 0;
				{
					if ((pUDMI->dis.itemState & ODS_INACTIVE) || (pUDMI->dis.itemState & ODS_DEFAULT)) 
					{
						// normal display
						iTextStateID = MBI_NORMAL;
						iBackgroundStateID = MBI_NORMAL;
					}
					if (pUDMI->dis.itemState & ODS_HOTLIGHT) 
					{
						// hot tracking
						iTextStateID = MBI_HOT;
						iBackgroundStateID = MBI_HOT;

						pbrBackground = &g_brItemBackgroundHot;
					}
					if (pUDMI->dis.itemState & ODS_SELECTED) 
					{
						// clicked -- MENU_POPUPITEM has no state for this, though MENU_BARITEM does
						iTextStateID = MBI_HOT;
						iBackgroundStateID = MBI_HOT;

						pbrBackground = &g_brItemBackgroundSelected;
					}
					if ((pUDMI->dis.itemState & ODS_GRAYED) || (pUDMI->dis.itemState & ODS_DISABLED)) 
					{
						// disabled / grey text
						iTextStateID = MBI_DISABLED;
						iBackgroundStateID = MBI_DISABLED;
					}
					if (pUDMI->dis.itemState & ODS_NOACCEL)
						dwFlags |= DT_HIDEPREFIX;
				}

				if (!g_menuTheme)
					g_menuTheme = OpenThemeData(hWnd, L"Menu");

				DTTOPTS opts = { sizeof(opts), DTT_TEXTCOLOR, iTextStateID != MBI_DISABLED ?
					APIHook::Comctl32GetSysColor(COLOR_WINDOWTEXT) :
					APIHook::Comctl32GetSysColor(COLOR_BTNTEXT) };

				FillRect(pUDMI->um.hdc, &pUDMI->dis.rcItem, *pbrBackground);
				DrawThemeTextEx(g_menuTheme, pUDMI->um.hdc, MENU_BARITEM, MBI_NORMAL, menuString, mii.cch,
					dwFlags, &pUDMI->dis.rcItem, &opts);

				return 0;
			}
			case WM_UAHMEASUREMENUITEM:
			{
				UAHMEASUREMENUITEM* pMmi = (UAHMEASUREMENUITEM*)lParam;

				// allow the default window procedure to handle the message
				// since we don't really care about changing the width
				auto Res = DefSubclassProc(hWnd, uMsg, wParam, lParam);

				// but we can modify it here to make it 1/3rd wider for example
				pMmi->mis.itemWidth = (pMmi->mis.itemWidth * 4) / 3;

				return Res;
			}
			case WM_NCPAINT:
			case WM_NCACTIVATE:
			{
				auto Res = DefSubclassProc(hWnd, uMsg, wParam, lParam);
				UAHDrawMenuNCBottomLine(hWnd, APIHook::Comctl32GetSysColorBrush(COLOR_BTNFACE));
				return Res;
			}

			case WM_CTLCOLOREDIT:
			{
				if (HDC hdc = reinterpret_cast<HDC>(wParam); hdc)
					return UI::EditText::OnCtlColorEdit(hdc);
			}

			case WM_CTLCOLORSTATIC:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORDLG:
			{
				if (HDC hdc = reinterpret_cast<HDC>(wParam); hdc)
				{
					SetTextColor(hdc, UI::GetThemeSysColor(UI::ThemeColor_Text_3));
					SetBkColor(hdc, UI::GetThemeSysColor(UI::ThemeColor_Default));
				}

				return reinterpret_cast<INT_PTR>(UI::Comctl32GetSysColorBrush(COLOR_BTNFACE));
			}

			case WM_CTLCOLORLISTBOX:
			{
				if (HDC hdc = reinterpret_cast<HDC>(wParam); hdc)
					return UI::ListBox::OnCtlColorListBox(hWnd, hdc);
			}

			case WM_INITDIALOG:
			case WM_CREATE:
			{
				DefSubclassProc(hWnd, uMsg, wParam, lParam);
				// Theme settings are initialized after WM_CREATE is processed

				auto f = UI::ThemeData::GetSingleton()->ThemeFont;
				HTHEME scrollBarTheme = nullptr;
				auto themeType = GetThemeTypeFromWindow(hWnd);
				UI::CUICustomWindow Window = hWnd;

				auto uStyles = Window.Style;
				auto uStylesEx = Window.StyleEx;
				std::string className = Window.GetName();

				switch (themeType) {
				case ThemeType::MDIClient:
					UI::MDIClient::Initialize(hWnd);
					break;
				case ThemeType::RichEdit20:
				case ThemeType::RichEdit50:
				{
					CHARFORMATA format = { 0 };
					format.cbSize = sizeof(CHARFORMATA);
					format.dwMask = CFM_COLOR | CFM_CHARSET;
					format.crTextColor = UI::GetThemeSysColor(UI::ThemeColor_Text_4);
					format.bCharSet = (BYTE)f->CharSet;
					SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);
					SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&format);
					SendMessageA(hWnd, EM_SETBKGNDCOLOR, FALSE, UI::GetThemeSysColor(UI::ThemeColor_Edit_Color));

					if (themeType == ThemeType::RichEdit20)
						scrollBarTheme = UI::RichEdit::Initialize(hWnd);
				}
				break;
				case ThemeType::Button:
					if (((uStylesEx & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE) && ((uStylesEx & WS_EX_STATICEDGE) == WS_EX_STATICEDGE))
						// IMAGE (DirectX window)
						UI::CustomBox::Initialize(hWnd, UI::CustomBox::abColor);
					break;
				case ThemeType::Edit:
					if (((uStyles & ES_MULTILINE) == ES_MULTILINE) &&
						(((uStyles & WS_VSCROLL) == WS_VSCROLL) || ((uStyles & WS_HSCROLL) == WS_HSCROLL)))
						// this memo control
						scrollBarTheme = UI::Memo::Initialize(hWnd);
					PostMessageA(hWnd, WM_SETFONT, (WPARAM)f->Handle, TRUE);
					PostMessageA(hWnd, WM_SYSCOLORCHANGE, 0, 0);
					break;
				case ThemeType::ListBox:
				{
					scrollBarTheme = UI::ListBox::Initialize(hWnd);

					LONG_PTR uID = GetWindowLongPtrA(hWnd, GWLP_ID);
					if (uID != 0x7D0 && uID != 0x7D1)
						PostMessageA(hWnd, WM_SETFONT, (WPARAM)f->Handle, TRUE);
				}
				break;
				case ThemeType::ListView:
					scrollBarTheme = UI::ListView::Initialize(hWnd);
					PostMessageA(hWnd, WM_SETFONT, (WPARAM)f->Handle, TRUE);
					break;
				case ThemeType::TreeView:
					scrollBarTheme = UI::TreeView::Initialize(hWnd);
					PostMessageA(hWnd, WM_SETFONT, (WPARAM)f->Handle, TRUE);
					break;
				case ThemeType::TabControl:
					UI::PageControl::Initialize(hWnd);
					break;
				case ThemeType::StatusBar:
					UI::StatusBar::Func::AdjustHeightByTextHeight(hWnd, (HFONT)f->Handle);
					break;
				case ThemeType::ToolBar:
					// Magic numbers to exclude paint of the standard dialog for opening and saving files
					if ((0x80 != uStylesEx) && (0x5600B84D != uStyles))
						UI::ToolBar::Initialize(hWnd);
					break;
				case ThemeType::ProgressBar:
					UI::ProgressBar::Initialize(hWnd);
					break;
				case ThemeType::PopupMenu:
				{
					Monitor monitor = Screen::GetSingleton()->MonitorFromWindow(hWnd);
					// Avoid the up and down arrows in PopupMenu
					if (monitor.WorkAreaRect.Height > 768)
						UI::PopupMenu::Initialize(hWnd);
				}
				break;
				case ThemeType::Spin:
					UI::UpDown::Initialize(hWnd);
					break;
				default:
					// fix slowdown render window... exclude from subclassing
					if ((ThemeType::Static == themeType) && ((uStyles & SS_BLACKRECT) == SS_BLACKRECT) && 
						(GetWindowLongPtrA(hWnd, GWLP_ID) == 0xA3B)) 
					{
						RemoveWindowSubclass(hWnd, WindowSubclassModernTheme, 0);
						break;
					}
					else if ((uStyles & SS_SUNKEN) == SS_SUNKEN) 
					{
						// 1. Label with frame
						// 2. ColorBox (and DialogColor)
						// 3. Edit (Disabled) (don't understand)	--- SS_EDITCONTROL
						// 4. IconBox (CK deprecated control)

						if ((uStyles & SS_BLACKFRAME) == SS_BLACKFRAME)
							// CK deprecated control (Icon)
							UI::CustomBox::Initialize(hWnd, UI::CustomBox::abNormal);
						//else if (((uStyles & SS_ETCHEDFRAME) == SS_ETCHEDFRAME) || ((uStyles & SS_ETCHEDHORZ) == SS_ETCHEDHORZ) ||
						//	((uStyles & SS_ETCHEDVERT) == SS_ETCHEDVERT))
						//	// CK seperator etc
						//	UI::CustomBox::Initialize(hWnd, UI::CustomBox::abNormal);
						else 
						{
							// skip edit control 
							if (((uStyles & SS_EDITCONTROL) == SS_EDITCONTROL) || ((uStyles & ES_AUTOHSCROLL) == ES_AUTOHSCROLL) ||
								((uStyles & ES_AUTOVSCROLL) == ES_AUTOVSCROLL))
								break;

							if (((uStyles & SS_OWNERDRAW) == SS_OWNERDRAW) || ((uStyles & SS_BLACKRECT) == SS_BLACKRECT))
								// CK defined everywhere this component so, except for light (SS_BLACKRECT)
								UI::CustomBox::Initialize(hWnd, UI::CustomBox::abColor);
						}

						break;
					}
					else if (((uStyles & SS_CENTERIMAGE) == SS_CENTERIMAGE) && ((uStylesEx & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE))
						// CK deprecated control (material count)
						UI::CustomBox::Initialize(hWnd, UI::CustomBox::abNormal);
					else if ((ThemeType::Static == themeType) && (((uStylesEx & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE) || 
						((uStylesEx & WS_EX_STATICEDGE) == WS_EX_STATICEDGE)))
						UI::CustomBox::Initialize(hWnd, UI::CustomBox::abNormal);
					else if (((uStyles & WS_CAPTION) == WS_CAPTION) && ((uStyles & WS_CHILD) != WS_CHILD))
					{
						if ((uStyles & WS_SYSMENU) == WS_SYSMENU)
						{
							// It's a bit wasteful, but it's guaranteed that all windows will have their title changed.
							ModernTheme::ApplyDarkThemeForWindow(reinterpret_cast<void*>(hWnd));

							SetClassLongPtrA(hWnd, GCLP_HICON, (LONG_PTR)g_AppIcon);
							SetClassLongPtrA(hWnd, GCLP_HICONSM, (LONG_PTR)g_AppIcon);
						}
					}
					break;
				}

				if (scrollBarTheme)
					RegisterThemeHandle(scrollBarTheme, ThemeType::ScrollBar);
				else
					RegisterThemeHandle(hWnd, themeType);

				if (!_spermanentWindowSubclasses.count(className))
					RemoveWindowSubclass(hWnd, WindowSubclassModernTheme, 0);
			}
			return S_OK;

			case WM_INITMENUPOPUP:
			{
				Monitor monitor = Screen::GetSingleton()->MonitorFromWindow(hWnd);
				// Avoid the up and down arrows in PopupMenu
				if (monitor.WorkAreaRect.Height > 768)
				{
					// The message about the initialization of the pop-up menu, set all its elements as owner draw
					UI::PopupMenu::Event::OnInitPopupMenu(hWnd, (HMENU)wParam);

					return S_OK;
				}
			}
			break;

			case WM_MEASUREITEM:
			{
				LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lParam;

				if (lpmis->CtlType == ODT_MENU)
				{
					// Calc size menu item
					UI::PopupMenu::Event::OnMeasureItem(hWnd, lpmis);

					return TRUE;
				}
			}
			break;

			// WHAT? They use this message to render the preview
			// Fixed
			case WM_DRAWITEM:
			{
				LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;

				switch (lpdis->CtlType)
				{
				case ODT_MENU:
					// Paint menu item
					UI::PopupMenu::Event::OnDrawItem(hWnd, lpdis);
					return TRUE;
				case ODT_LISTVIEW:
				{
					if (lpdis->CtlID == 1056)
					{
						// Paint draw item listview
						UI::ListView::OnCustomDrawItemPlugins(hWnd, lpdis);

						return TRUE;
					}
				}
				break;
				}
			}
			break;

			case WM_DESTROY:
			{
				auto it = WindowHandles.find(hWnd);
				if (it != WindowHandles.end())
					WindowHandles.unsafe_erase(it);
			}
			break;

			case WM_NOTIFY:
			{
				LPNMHDR nmhdr = (LPNMHDR)lParam;

				// Custom drawing
				if (nmhdr->code == NM_CUSTOMDRAW)
				{
					auto themeType = GetThemeTypeFromWindow(nmhdr->hwndFrom);

					if ((themeType == ThemeType::TreeView) && (nmhdr->idFrom != 2093))
						// I have no idea why TreeView uses the ListView functionality, this question is for microsoft.
						return UI::TreeView::OnCustomDraw(hWnd, (LPNMLVCUSTOMDRAW)lParam);

					if (themeType == ThemeType::ListView)
					{
						// To colorize the list of mods, WM_NOTIFY calls prevent me.
						// It's strange, but to change the color of the text from black to light in standard CK windows with properties, WM_NOTIFY is needed.
						if (nmhdr->idFrom != 1056)
							return UI::ListView::OnCustomDraw(hWnd, (LPNMLVCUSTOMDRAW)lParam);
						else
							return CDRF_DODEFAULT;
					}
					else if (themeType == ThemeType::ToolBar)
						return UI::ToolBar::OnCustomDraw(hWnd, (LPNMTBCUSTOMDRAW)lParam);
				}
			}
			break;

			}

			return DefSubclassProc(hWnd, uMsg, wParam, lParam);
		}

		static LRESULT CALLBACK DialogSubclassModernTheme(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
		{
			LRESULT result = WindowSubclassModernTheme(hWnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);

			switch (uMsg)
			{
			case WM_PAINT:
			{
				// Special override for DialogBoxIndirectParam (MessageBox) since the bottom half doesn't get themed correctly. 
				// ReactOS says this is MSGBOX_IDTEXT. Standard message boxes will have 3 buttons or less.
				if (GetDlgItem(hWnd, 0xFFFF))
				{
					int buttonCount = 0;

					EnumChildWindows(hWnd, [](HWND ChildWindow, LPARAM Param)
						{
							CHAR classname[256] = {};
							GetClassName(ChildWindow, classname, (int)std::size(classname));

							if (!_stricmp(classname, "Button"))
								(*reinterpret_cast<PINT>(Param))++;

							return TRUE;
						}, reinterpret_cast<LPARAM>(&buttonCount));

					if (buttonCount <= 3) 
					{
						UI::CUICustomWindow Window = hWnd;
						Canvas canvas(GetDC(hWnd));
						CRECT windowArea(Window.ClientRect());

						canvas.Fill(windowArea, UI::GetThemeSysColor(UI::ThemeColor_Default));
					}
				}
			}
			break;
			}

			return result;
		}

		void ModernTheme::ApplyDarkThemeForWindow(void* hWnd) noexcept(true)
		{
			auto _OsVer = HardwareInfo::OS::GetVersion();
			auto major = GET_EXE_VERSION_EX_MAJOR(_OsVer);
			if (major == 10)
			{
				if (GET_EXE_VERSION_EX_BUILD(_OsVer) >= 22000)
				{
				darkcustom:
					// Win11			
					COLORREF Color = UI::Comctl32GetSysColor(COLOR_BTNFACE);
					HRESULT hr = DwmSetWindowAttribute((HWND)hWnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &Color, sizeof(Color));
					if (SUCCEEDED(hr))
					{
						Color = UI::Comctl32GetSysColor(COLOR_BTNTEXT);
						hr = DwmSetWindowAttribute((HWND)hWnd, DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR, &Color, sizeof(Color));
						if (SUCCEEDED(hr))
						{
							Color = UI::Comctl32GetSysColor(COLOR_INACTIVEBORDER);
							hr = DwmSetWindowAttribute((HWND)hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &Color, sizeof(Color));
							if (SUCCEEDED(hr))
							{
								// TODO: Reg for inactive/active
							}
						}
					}
				}
				else
				{
					// Win10
					BOOL DarkMode = true;
					HRESULT hr = DwmSetWindowAttribute((HWND)hWnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
						&DarkMode, sizeof(DarkMode));
				}
			}
			else if (major > 10)
				goto darkcustom;
		}

		static bool ExcludeSubclassKnownWindowsAndApplyDarkTheme(HWND hWindow, bool bRemoved = false)
		{
			auto style = GetWindowLongA(hWindow, GWL_STYLE);
			if ((style & WS_CHILD) == WS_CHILD)
				return false;

			ModernTheme::ApplyDarkThemeForWindow(reinterpret_cast<void*>(hWindow));

			return true;
		}

		static LRESULT CALLBACK CallWndProcCallbackModernTheme(INT nCode, WPARAM wParam, LPARAM lParam) noexcept(true)
		{
			if (nCode == HC_ACTION)
			{
				auto messageData = reinterpret_cast<CWPSTRUCT*>(lParam);
				if (messageData)
				{
					switch (messageData->message)
					{
					case WM_CREATE:
					{
						LPCREATESTRUCTA lpCreateStruct = (LPCREATESTRUCTA)messageData->lParam;
						if (lpCreateStruct)
						{
							/*_CONSOLE("cw %p <%s> <%s>", lpCreateStruct->lpszClass,
								(((uintptr_t)lpCreateStruct->lpszClass > 0xFFFFull) ? lpCreateStruct->lpszClass : ""),
								(lpCreateStruct->lpszName ? lpCreateStruct->lpszName : ""));*/

							if ((lpCreateStruct->hInstance) &&
								(lpCreateStruct->hInstance != GetModuleHandleA("comdlg32.dll"))) 
							{
								if (WindowHandles.find(messageData->hwnd) == WindowHandles.end())
								{
									SetWindowSubclass(messageData->hwnd, WindowSubclassModernTheme, 0,
										reinterpret_cast<DWORD_PTR>(WindowSubclassModernTheme));
									WindowHandles.insert(std::make_pair(messageData->hwnd, false));
								}
							}
						}
						break;
					}
					case WM_INITDIALOG:
					{
						// _CONSOLE("iw %llx %llx", wParam, lParam);

						auto wnd = WindowHandles.find(messageData->hwnd);
						if (wnd == WindowHandles.end())
						{
							if (ExcludeSubclassKnownWindowsAndApplyDarkTheme(messageData->hwnd))
							{
								SetWindowSubclass(messageData->hwnd, DialogSubclassModernTheme, 0,
									reinterpret_cast<DWORD_PTR>(DialogSubclassModernTheme));
								WindowHandles.insert(std::make_pair(messageData->hwnd, true));
							}
						}
						else
						{
							if (ExcludeSubclassKnownWindowsAndApplyDarkTheme(messageData->hwnd, true))
							{
								RemoveWindowSubclass(messageData->hwnd, WindowSubclassModernTheme, 0);
								SetWindowSubclass(messageData->hwnd, DialogSubclassModernTheme, 0, 
									reinterpret_cast<DWORD_PTR>(DialogSubclassModernTheme));
								wnd->second = true;
							}
							else
								wnd->second = true;
						}
						break;
					}
					}
				}
			}

			return CallNextHookEx(g_hkWndProcModernThemeHandle, nCode, wParam, lParam);
		}

		void ModernTheme::AddSpermanentWindowSubclass(const char* Name) noexcept(true)
		{
			if (!_spermanentWindowSubclasses.count(Name))
				_spermanentWindowSubclasses.emplace(Name);
		}

		void ModernTheme::InitializeCurrentThread() noexcept(true)
		{
			InitializeThread(GetCurrentThreadId());
		}

		void ModernTheme::InitializeThread(uint32_t u32ThreadId) noexcept(true)
		{
			if (!UI::ThemeData::GetSingleton()->ThemeFont)
				UI::ThemeData::GetSingleton()->ThemeFont = new Font("Microsoft Sans Serif", 8, {},
					_READ_OPTION_INT("CreationKit", "Charset", DEFAULT_CHARSET),
					fqClearTypeNatural, fpVariable);

			g_hkWndProcModernThemeHandle = SetWindowsHookExA(WH_CALLWNDPROC, CallWndProcCallbackModernTheme, 
				0, u32ThreadId);
		}

		ModernTheme* ModernTheme::GetSingleton() noexcept(true)
		{
			return _smoderntheme;
		}

		void ModernTheme::Hook::Initialize(bool support_more_theme) noexcept(true)
		{
			_smoderntheme = new ModernTheme;
			_smoderntheme->_caption_styles_supported = UI::IsDarkThemeSystemSupported();

			std::uint32_t theme_id = 1;
			if (support_more_theme)
			{
				theme_id = std::min(_READ_OPTION_UINT("CreationKit", "uUIDarkThemeId", theme_id), 3ul);
				if (theme_id == 3)
				{
					if (!UI::OpenCustomTheme())
						UI::SetTheme(UI::Theme_DarkGray);
				}
				else
					UI::SetTheme(UI::Theme(theme_id + 3));
			}
			else
				// Visually, the two themes match each other better with qt5 plastique dark.
				UI::SetTheme(UI::Theme_Dark);

			// I will new it once and forget about its existence
			// I have no general idea where to destroy it. Yes, and it is not necessary, it will die along with the process.

			auto comDll = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("comctl32.dll"));
			if (!comDll)
				ErrorHandler::Trigger("No found comctl32.dll");

			Detours::DetourIAT(comDll, "USER32.dll", "GetSysColor", 
				(std::uintptr_t)&APIHook::Comctl32GetSysColor);
			Detours::DetourIAT(comDll, "USER32.dll", "GetSysColorBrush", 
				(std::uintptr_t)&APIHook::Comctl32GetSysColorBrush);
			Detours::DetourIATDelayed(comDll, "UxTheme.dll", "DrawThemeBackground", 
				(std::uintptr_t)&APIHook::Comctl32DrawThemeBackground);
			Detours::DetourIATDelayed(comDll, "UxTheme.dll", "DrawThemeText", 
				(std::uintptr_t)&APIHook::Comctl32DrawThemeText);

			// Bethesda began to move forward, but abandoned its UI in favor of Qt.
			// Perhaps this is justified, since it first forced them to redo the UI, and Qt is a reliable library.
			// However, there is one drawback, the size of the application is more than 64 MB,
			// which significantly reduces the start of the application.
			// It doesn't matter, we redefine the functions if this library exists.
			auto qt5Widgets = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("qt5widgets.dll"));
			if (qt5Widgets)
			{
				Detours::DetourIAT(qt5Widgets, "USER32.dll", "GetSysColor", 
					(std::uintptr_t)&APIHook::Comctl32GetSysColor);
				Detours::DetourIATDelayed(qt5Widgets, "UxTheme.dll", "DrawThemeBackground", 
					(std::uintptr_t)&APIHook::Comctl32DrawThemeBackground);
				Detours::DetourIATDelayed(qt5Widgets, "UxTheme.dll", "DrawThemeTextEx", 
					(std::uintptr_t)&APIHook::Comctl32DrawThemeTextEx);
			}

			g_brItemBackground = (HBRUSH)UI::Comctl32GetSysColorBrush(COLOR_BTNFACE);
			g_brItemBackgroundHot = ::CreateSolidBrush(UI::GetThemeSysColor(UI::ThemeColor_Button_Hot_Gradient_End));
			g_brItemBackgroundSelected = (HBRUSH)UI::Comctl32GetSysColorBrush(COLOR_HIGHLIGHT);
			g_AppIcon = LoadIconA(GetModuleHandleA(nullptr), MAKEINTRESOURCEA(318));

			InitializeCurrentThread();
		}
	}
}