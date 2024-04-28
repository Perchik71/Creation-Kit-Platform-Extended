// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

// Special thanks (very cool): https://github.com/adzm/win32-custom-menubar-aero-theme

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/UI/UIBaseWindow.h"
#include "UIThemePatch.h"

#include "UITheme/VarCommon.h"
#include "UITheme/MDIClient.h"
#include "UITheme/PopupMenu.h"
#include "UITheme/ListView.h"
#include "UITheme/TreeView.h"
#include "UITheme/ToolBar.h"
#include "UITheme/PushButton.h"
#include "UITheme/CheckBox.h"
#include "UITheme/RadioButton.h"
#include "UITheme/GroupBox.h"
#include "UITheme/EditText.h"
#include "UITheme/ComboBox.h"
#include "UITheme/StatusBar.h"
#include "UITheme/ScrollBar.h"
#include "UITheme/TrackBar.h"
#include "UITheme/UpDown.h"
#include "UITheme/ListBox.h"
#include "UITheme/ProgressBar.h"
#include "UITheme/CustomBox.h"
#include "UITheme/TimeOfDay.h"
#include "UITheme/Header.h"
#include "UITheme/PageControl.h"
#include "UITheme/RichEdit20.h"
#include "UITheme/Memo.h"

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

void UAHDrawMenuNCBottomLine(HWND hWnd, HBRUSH hbrBarBackground)
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

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Graphics = ::Core::Classes::UI;
		namespace UITheme = CreationKitPlatformExtended::UITheme;

		struct string_equal_to {
			inline bool operator()(const std::string_view& lhs, const std::string_view& rhs) const {
				return !_stricmp(lhs.data(), rhs.data());
			}
		};

		const UnorderedSet<std::string_view, std::hash<std::string_view>, std::equal_to<std::string_view>> PermanentWindowSubclasses
		{
			"Creation Kit",
			"Creation Kit Skyrim Special Edition [v1.5.3]",
			"Creation Kit Skyrim Special Edition [v1.5.73]",
			"Creation Kit Skyrim Special Edition [v1.6.438]",
			"Creation Kit Skyrim Special Edition [v1.6.1130]",
			"Creation Kit Fallout 4 [v1.10.162.0]",
			"Creation Kit Fallout 4 [v1.10.943.1]",
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
			"Render Window",
			"#32770",
		};

		// List of created windows
		ConcurrencyMap<HWND, BOOL, std::hash<HTHEME>, std::equal_to<HTHEME>> WindowHandles;
		// List of registered visual styles themes
		ConcurrencyMap<HTHEME, ThemeType, std::hash<HTHEME>, std::equal_to<HTHEME>> ThemeHandles;

		static WNDPROC OldPopupMenuWndClass = NULL;
		static Graphics::CUIFont* listFont = NULL;
		static HBRUSH g_brItemBackground = NULL;
		static HBRUSH g_brItemBackgroundHot = NULL;
		static HBRUSH g_brItemBackgroundSelected = NULL;

		namespace hook_func {
			COLORREF WINAPI SetTextColor(HDC hdc, COLORREF color) {
				return ::SetTextColor(hdc, !color ? UITheme::GetThemeSysColor(UITheme::ThemeColor_Text_4) : color);
			}

			BOOL WINAPI DrawEdge(HDC hdc, LPRECT qrc, UINT edge, UINT grfFlags) {
				if (!qrc || !hdc)
					return FALSE;

				Graphics::CUICanvas canvas = hdc;

				if ((grfFlags & BF_MIDDLE) == BF_MIDDLE)
					canvas.Fill(*qrc, UITheme::GetThemeSysColor(UITheme::ThemeColor_TreeView_Color));

				canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Default);

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

			HFONT WINAPI CreateFontA(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight,
				DWORD  bItalic, DWORD  bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
				DWORD  iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName) 
			{
				return (HFONT)::CreateFontA(UITheme::ThemeFont->Height, 0, cEscapement, cOrientation, cWeight, bItalic, bUnderline,
					bStrikeOut, _READ_OPTION_INT("CreationKit", "nCharset", DEFAULT_CHARSET), iOutPrecision, iClipPrecision,
					CLEARTYPE_NATURAL_QUALITY, VARIABLE_PITCH, UITheme::ThemeFont->Name.c_str());
			}
		}

		UIThemePatch::UIThemePatch() : Module(GlobalEnginePtr)
		{}

		bool UIThemePatch::HasOption() const
		{
			return true;
		}

		bool UIThemePatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* UIThemePatch::GetOptionName() const
		{
			return "CreationKit:bUIDarkTheme";
		}

		const char* UIThemePatch::GetName() const
		{
			return "UI Dark Theme";
		}

		bool UIThemePatch::HasDependencies() const
		{
			return false;
		}

		Array<String> UIThemePatch::GetDependencies() const
		{
			return {};
		}

		bool UIThemePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool UIThemePatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			//if (lpRelocationDatabaseItem->Version() == 1)
			//{
				// I will new it once and forget about its existence
				// I have no general idea where to destroy it. Yes, and it is not necessary, it will die along with the process.

				auto comDll = reinterpret_cast<uintptr_t>(GetModuleHandle("comctl32.dll"));
				Assert(comDll);

				auto Id = _READ_OPTION_UINT("CreationKit", "uUIDarkThemeId", 0);
				Id = std::min(Id, (unsigned long)2);

				if (Id == 2)
				{
					if (!UITheme::OpenCustomTheme())
						UITheme::SetTheme(UITheme::Theme_DarkGray);
				}
				else
					UITheme::SetTheme(UITheme::Theme(Id + 3));

				voltek::detours_patch_iat(comDll, "USER32.dll", "GetSysColor", (uintptr_t)&Comctl32GetSysColor);
				voltek::detours_patch_iat(comDll, "USER32.dll", "GetSysColorBrush", (uintptr_t)&Comctl32GetSysColorBrush);
				voltek::detours_patch_iat_delayed(comDll, "UxTheme.dll", "DrawThemeBackground", (uintptr_t)&Comctl32DrawThemeBackground);
				voltek::detours_patch_iat_delayed(comDll, "UxTheme.dll", "DrawThemeText", (uintptr_t)&Comctl32DrawThemeText);

				g_brItemBackground = UITheme::Comctl32GetSysColorBrush(COLOR_BTNFACE);
				g_brItemBackgroundHot = CreateSolidBrush(UITheme::GetThemeSysColor(UITheme::ThemeColor_Button_Hot_Gradient_End));
				g_brItemBackgroundSelected = UITheme::Comctl32GetSysColorBrush(COLOR_HIGHLIGHT);

				return true;
			//}
			
			//return false;
		}

		bool UIThemePatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		void UIThemePatch::InitializeThread()
		{
			if (!UITheme::ThemeFont)
				UITheme::ThemeFont = new Graphics::CUIFont("Microsoft Sans Serif", 8, {},
					_READ_OPTION_INT("CreationKit", "Charset", DEFAULT_CHARSET),
					Graphics::fqClearTypeNatural, Graphics::fpVariable);

			SetWindowsHookExA(WH_CALLWNDPROC, CallWndProcCallback, NULL, GetCurrentThreadId());
		}

		bool UIThemePatch::ExcludeSubclassKnownWindows(HWND hWindow, BOOL bRemoved)
		{
			auto style = GetWindowLongA(hWindow, GWL_STYLE);
			if ((style & WS_CHILD) == WS_CHILD)
				return FALSE;

			CHAR szBuffer[MAX_PATH];
			GetWindowTextA(hWindow, szBuffer, MAX_PATH);

			static std::unordered_map<std::string_view, SUBCLASSPROC, std::hash<std::string_view>, string_equal_to> KnownWindows{
				{ "Render Window", NULL },
			};

			if (auto itr = KnownWindows.find(szBuffer); itr != KnownWindows.end())
			{
				if (bRemoved)
					RemoveWindowSubclass(hWindow, WindowSubclass, 0);

				return TRUE;
			}

			return FALSE;
		}

		ThemeType UIThemePatch::GetThemeTypeFromWindow(HWND hWindow)
		{
			static std::unordered_map<std::string_view, ThemeType, std::hash<std::string_view>, string_equal_to> TargetWindowThemes{
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
			{ "#32768", ThemeType::PopupMenu },
			};

			auto themeType = ThemeType::None;
			Graphics::CUICustomWindow Window = hWindow;

			if (auto itr = TargetWindowThemes.find(Window.Name); itr != TargetWindowThemes.end())
				themeType = itr->second;

			return themeType;
		}

		// Binds the specified class type to the visual theme. hWindow takes only HTHEME
		// Returns TRUE if successful
		bool UIThemePatch::RegisterThemeHandle(HWND hWindow, ThemeType eTheme) {
			HTHEME windowTheme = GetWindowTheme(hWindow);
			if (!windowTheme)
				return FALSE;

			return RegisterThemeHandle(windowTheme, eTheme);
		}

		// Binds the specified class type to the visual theme
		// Returns TRUE if successful
		bool UIThemePatch::RegisterThemeHandle(HTHEME hTheme, ThemeType eTheme) {
			if (ThemeType::None == eTheme)
				return FALSE;

			for (auto it = ThemeHandles.begin(); it != ThemeHandles.end(); it++) {
				if ((*it).second == eTheme)
					return FALSE;
			}

			ThemeHandles.insert(std::make_pair(hTheme, eTheme));

			return TRUE;
		}

		LRESULT CALLBACK UIThemePatch::CallWndProcCallback(INT nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode == HC_ACTION) 
			{
				auto messageData = reinterpret_cast<CWPSTRUCT*>(lParam);
				if (messageData->message == WM_CREATE) 
				{
					LPCREATESTRUCTA lpCreateStruct = (LPCREATESTRUCTA)messageData->lParam;
					if (lpCreateStruct) 
					{
						if ((lpCreateStruct->hInstance) &&
							(lpCreateStruct->hInstance != GetModuleHandleA("comdlg32.dll"))) {
							if (WindowHandles.find(messageData->hwnd) == WindowHandles.end()) {
								SetWindowSubclass(messageData->hwnd, WindowSubclass, 0, reinterpret_cast<DWORD_PTR>(WindowSubclass));
								WindowHandles.insert(std::make_pair(messageData->hwnd, FALSE));
							}
						}
					}
				}
				else if (messageData->message == WM_INITDIALOG) 
				{
					auto wnd = WindowHandles.find(messageData->hwnd);
					if (wnd == WindowHandles.end()) 
					{
						if (!ExcludeSubclassKnownWindows(messageData->hwnd))
						{
							SetWindowSubclass(messageData->hwnd, DialogSubclass, 0, reinterpret_cast<DWORD_PTR>(DialogSubclass));
							WindowHandles.insert(std::make_pair(messageData->hwnd, TRUE));
						}
					}
					else 
					{
						if (!ExcludeSubclassKnownWindows(messageData->hwnd, TRUE))
						{
							RemoveWindowSubclass(messageData->hwnd, WindowSubclass, 0);
							SetWindowSubclass(messageData->hwnd, DialogSubclass, 0, reinterpret_cast<DWORD_PTR>(DialogSubclass));
							wnd->second = TRUE;
						}
						else
							wnd->second = TRUE;
					}
				}
			}

			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}

		LRESULT CALLBACK UIThemePatch::WindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
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

					FillRect(pUDM->hdc, &rc, UITheme::Comctl32GetSysColorBrush(COLOR_BTNFACE));
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
						if ((pUDMI->dis.itemState & ODS_INACTIVE) || (pUDMI->dis.itemState & ODS_DEFAULT)) {
							// normal display
							iTextStateID = MBI_NORMAL;
							iBackgroundStateID = MBI_NORMAL;
						}
						if (pUDMI->dis.itemState & ODS_HOTLIGHT) {
							// hot tracking
							iTextStateID = MBI_HOT;
							iBackgroundStateID = MBI_HOT;

							pbrBackground = &g_brItemBackgroundHot;
						}
						if (pUDMI->dis.itemState & ODS_SELECTED) {
							// clicked -- MENU_POPUPITEM has no state for this, though MENU_BARITEM does
							iTextStateID = MBI_HOT;
							iBackgroundStateID = MBI_HOT;

							pbrBackground = &g_brItemBackgroundSelected;
						}
						if ((pUDMI->dis.itemState & ODS_GRAYED) || (pUDMI->dis.itemState & ODS_DISABLED)) {
							// disabled / grey text
							iTextStateID = MBI_DISABLED;
							iBackgroundStateID = MBI_DISABLED;
						}
						if (pUDMI->dis.itemState & ODS_NOACCEL) {
							dwFlags |= DT_HIDEPREFIX;
						}
					}

					if (!g_menuTheme)
						g_menuTheme = OpenThemeData(hWnd, L"Menu");

					DTTOPTS opts = { sizeof(opts), DTT_TEXTCOLOR, iTextStateID != MBI_DISABLED ? 
						UITheme::Comctl32GetSysColor(COLOR_WINDOWTEXT) :
						UITheme::Comctl32GetSysColor(COLOR_BTNTEXT) };
					
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
					UAHDrawMenuNCBottomLine(hWnd, UITheme::Comctl32GetSysColorBrush(COLOR_BTNFACE));
					return Res;
				}

				case WM_CTLCOLOREDIT: 
				{
					if (HDC hdc = reinterpret_cast<HDC>(wParam); hdc)
						return UITheme::EditText::OnCtlColorEdit(hdc);
				}

				case WM_CTLCOLORSTATIC:
				case WM_CTLCOLORBTN:
				case WM_CTLCOLORDLG: 
				{
					if (HDC hdc = reinterpret_cast<HDC>(wParam); hdc)
					{
						SetTextColor(hdc, UITheme::GetThemeSysColor(UITheme::ThemeColor_Text_3));
						SetBkColor(hdc, UITheme::GetThemeSysColor(UITheme::ThemeColor_Default));
					}

					return reinterpret_cast<INT_PTR>(UITheme::Comctl32GetSysColorBrush(COLOR_BTNFACE));
				}

				case WM_CTLCOLORLISTBOX: 
				{
					if (HDC hdc = reinterpret_cast<HDC>(wParam); hdc)
						return UITheme::ListBox::OnCtlColorListBox(hWnd, hdc);
				}

				case WM_INITDIALOG:
				case WM_CREATE: 
				{
					DefSubclassProc(hWnd, uMsg, wParam, lParam);
					// Theme settings are initialized after WM_CREATE is processed

					HTHEME scrollBarTheme = NULL;
					auto themeType = GetThemeTypeFromWindow(hWnd);
					Graphics::CUICustomWindow Window = hWnd;

					UINT uStyles = Window.Style;
					UINT uStylesEx = Window.StyleEx;
					std::string className = Window.Name;

					switch (themeType) {
					case ThemeType::MDIClient:
						UITheme::MDIClient::Initialize(hWnd);
						break;
					case ThemeType::RichEdit20:
					case ThemeType::RichEdit50:
					{
						CHARFORMATA format = { 0 };
						format.cbSize = sizeof(CHARFORMATA);
						format.dwMask = CFM_COLOR | CFM_CHARSET;
						format.crTextColor = UITheme::GetThemeSysColor(UITheme::ThemeColor_Text_4);
						format.bCharSet = (BYTE)UITheme::ThemeFont->CharSet;
						SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);
						SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&format);
						SendMessageA(hWnd, EM_SETBKGNDCOLOR, FALSE, UITheme::GetThemeSysColor(UITheme::ThemeColor_Edit_Color));

						if (themeType == ThemeType::RichEdit20)
							scrollBarTheme = UITheme::RichEdit::Initialize(hWnd);
					}
					break;
					case ThemeType::Button:
						if (((uStylesEx & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE) && ((uStylesEx & WS_EX_STATICEDGE) == WS_EX_STATICEDGE))
							// IMAGE (DirectX window)
							UITheme::CustomBox::Initialize(hWnd, UITheme::CustomBox::abColor);
						break;
					case ThemeType::Edit:
						if (((uStyles & ES_MULTILINE) == ES_MULTILINE) &&
							(((uStyles & WS_VSCROLL) == WS_VSCROLL) || ((uStyles & WS_HSCROLL) == WS_HSCROLL)))
							// this memo control
							scrollBarTheme = UITheme::Memo::Initialize(hWnd);
						PostMessageA(hWnd, WM_SETFONT, (WPARAM)UITheme::ThemeFont->Handle, TRUE);
						PostMessageA(hWnd, WM_SYSCOLORCHANGE, 0, 0);
						break;
					case ThemeType::ListBox: {
						scrollBarTheme = UITheme::ListBox::Initialize(hWnd);

						LONG_PTR uID = GetWindowLongPtrA(hWnd, GWLP_ID);
						if (uID != 0x7D0 && uID != 0x7D1)
							PostMessageA(hWnd, WM_SETFONT, (WPARAM)UITheme::ThemeFont->Handle, TRUE);
					}
										   break;
					case ThemeType::ListView:
						scrollBarTheme = UITheme::ListView::Initialize(hWnd);
						PostMessageA(hWnd, WM_SETFONT, (WPARAM)UITheme::ThemeFont->Handle, TRUE);
						break;
					case ThemeType::TreeView:
						scrollBarTheme = UITheme::TreeView::Initialize(hWnd);
						PostMessageA(hWnd, WM_SETFONT, (WPARAM)UITheme::ThemeFont->Handle, TRUE);
						break;
					case ThemeType::TabControl:
						UITheme::PageControl::Initialize(hWnd);
						break;
					case ThemeType::StatusBar:
						UITheme::StatusBar::Func::AdjustHeightByTextHeight(hWnd, (HFONT)UITheme::ThemeFont->Handle);
						break;
					case ThemeType::ToolBar:
						// Magic numbers to exclude paint of the standard dialog for opening and saving files
						if ((0x80 != uStylesEx) && (0x5600B84D != uStyles))
							UITheme::ToolBar::Initialize(hWnd);
						break;
					case ThemeType::ProgressBar:
						UITheme::ProgressBar::Initialize(hWnd);
						break;
					case ThemeType::PopupMenu: {
						Graphics::CUIMonitor Monitor = Graphics::Screen.MonitorFromWindow(hWnd);
						// Avoid the up and down arrows in PopupMenu
						if (Monitor.WorkAreaRect.Height > 768)
							UITheme::PopupMenu::Initialize(hWnd);
					}
											 break;
					case ThemeType::Spin:
						UITheme::UpDown::Initialize(hWnd);
						break;
					default:
						// fix slowdown render window... exclude from subclassing
						if ((ThemeType::Static == themeType) && ((uStyles & SS_BLACKRECT) == SS_BLACKRECT) && (GetWindowLongPtrA(hWnd, GWLP_ID) == 0xA3B)) {
							RemoveWindowSubclass(hWnd, WindowSubclass, 0);
							break;
						}
						else if ((uStyles & SS_SUNKEN) == SS_SUNKEN) {
							// 1. Label with frame
							// 2. ColorBox (and DialogColor)
							// 3. Edit (Disabled) (don't understand)	--- SS_EDITCONTROL
							// 4. IconBox (CK deprecated control)

							if ((uStyles & SS_BLACKFRAME) == SS_BLACKFRAME)
								// CK deprecated control (Icon)
								UITheme::CustomBox::Initialize(hWnd, UITheme::CustomBox::abNormal);
							else {
								// skip edit control 
								if (((uStyles & SS_EDITCONTROL) == SS_EDITCONTROL) || ((uStyles & ES_AUTOHSCROLL) == ES_AUTOHSCROLL) ||
									((uStyles & ES_AUTOVSCROLL) == ES_AUTOVSCROLL))
									break;

								if (((uStyles & SS_OWNERDRAW) == SS_OWNERDRAW) || ((uStyles & SS_BLACKRECT) == SS_BLACKRECT))
									// CK defined everywhere this component so, except for light (SS_BLACKRECT)
									UITheme::CustomBox::Initialize(hWnd, UITheme::CustomBox::abColor);
							}

							break;
						}
						else if (((uStyles & SS_CENTERIMAGE) == SS_CENTERIMAGE) && ((uStylesEx & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE))
							// CK deprecated control (material count)
							UITheme::CustomBox::Initialize(hWnd, UITheme::CustomBox::abNormal);
						else if ((ThemeType::Static == themeType) && (((uStylesEx & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE) || ((uStylesEx & WS_EX_STATICEDGE) == WS_EX_STATICEDGE)))
							UITheme::CustomBox::Initialize(hWnd, UITheme::CustomBox::abNormal);
						else if (((uStyles & WS_CAPTION) == WS_CAPTION) && ((uStyles & WS_CHILD) != WS_CHILD)) {
							if ((uStyles & WS_SYSMENU) == WS_SYSMENU) 
							{
								auto Inst = GlobalEnginePtr->GetModuleBase();
								SetClassLongPtrA(hWnd, GCLP_HICON, (LONG_PTR)LoadIconA((HINSTANCE)Inst, MAKEINTRESOURCEA(318)));
								SetClassLongPtrA(hWnd, GCLP_HICONSM, (LONG_PTR)LoadIconA((HINSTANCE)Inst, MAKEINTRESOURCEA(318)));
							}
						}
						break;
					}

					if (scrollBarTheme)
						RegisterThemeHandle(scrollBarTheme, ThemeType::ScrollBar);
					else
						RegisterThemeHandle(hWnd, themeType);

					if (!PermanentWindowSubclasses.count(className))
						RemoveWindowSubclass(hWnd, WindowSubclass, 0);
				}
				return S_OK;

				case WM_INITMENUPOPUP: 
				{
					Graphics::CUIMonitor Monitor = Graphics::Screen.MonitorFromWindow(hWnd);
					// Avoid the up and down arrows in PopupMenu
					if (Monitor.WorkAreaRect.Height > 768)
					{
						// The message about the initialization of the pop-up menu, set all its elements as owner draw
						UITheme::PopupMenu::Event::OnInitPopupMenu(hWnd, (HMENU)wParam);

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
						UITheme::PopupMenu::Event::OnMeasureItem(hWnd, lpmis);

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
						UITheme::PopupMenu::Event::OnDrawItem(hWnd, lpdis);
						return TRUE;
					case ODT_LISTVIEW:
					{
						if (lpdis->CtlID == EditorAPI::EditorUI::UI_DATA_DIALOG_PLUGINLISTVIEW)
						{
							// Paint draw item listview
							UITheme::ListView::OnCustomDrawItemPlugins(hWnd, lpdis);

							return TRUE;
						}
					}
					break;
					}
					if (lpdis->CtlType == ODT_MENU)
					{
						// Paint menu item
						UITheme::PopupMenu::Event::OnDrawItem(hWnd, lpdis);

						return TRUE;
					}

				}
				break;

				case WM_DESTROY: {
					auto it = WindowHandles.find(hWnd);
					if (it != WindowHandles.end())
						WindowHandles.unsafe_erase(it);
				}
				break;

				case WM_NOTIFY: {
					LPNMHDR nmhdr = (LPNMHDR)lParam;

					// Custom drawing
					if (nmhdr->code == NM_CUSTOMDRAW) {
						auto themeType = GetThemeTypeFromWindow(nmhdr->hwndFrom);

						if (themeType == ThemeType::ListView) {
							// To colorize the list of mods, WM_NOTIFY calls prevent me.
							// It's strange, but to change the color of the text from black to light in standard CK windows with properties, WM_NOTIFY is needed.
							if (nmhdr->idFrom != EditorAPI::EditorUI::UI_DATA_DIALOG_PLUGINLISTVIEW)
								return UITheme::ListView::OnCustomDraw(hWnd, (LPNMLVCUSTOMDRAW)lParam);
							else
								return CDRF_DODEFAULT;
						}
						else if (themeType == ThemeType::ToolBar)
							return UITheme::ToolBar::OnCustomDraw(hWnd, (LPNMTBCUSTOMDRAW)lParam);
					}
				}
				break;

			}

			return DefSubclassProc(hWnd, uMsg, wParam, lParam);
		}

		LRESULT CALLBACK UIThemePatch::DialogSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
		{
			LRESULT result = WindowSubclass(hWnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);

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

						if (buttonCount <= 3) {
							Graphics::CUICustomWindow Window = hWnd;
							Graphics::CUICanvas Canvas(GetDC(hWnd));
							Graphics::CRECT windowArea(Window.ClientRect());

							Canvas.Fill(windowArea, UITheme::GetThemeSysColor(UITheme::ThemeColor_Default));
						}
					}
				}
				break;
			}

			return result;
		}

		DWORD UIThemePatch::Comctl32GetSysColor(INT nIndex)
		{
			return UITheme::Comctl32GetSysColor(nIndex);
		}

		HBRUSH UIThemePatch::Comctl32GetSysColorBrush(INT nIndex)
		{
			return UITheme::Comctl32GetSysColorBrush(nIndex);
		}

		HRESULT UIThemePatch::Comctl32DrawThemeText(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId,
			LPCWSTR pszText, INT cchText, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
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

				Graphics::CUICanvas Canvas(hdcMem);

				UITheme::StatusBar::Render::DrawBackground(Canvas, &rc);

				Canvas.TransparentMode = TRUE;
				Canvas.Font.Assign(*UITheme::ThemeFont);

				UITheme::StatusBar::Event::OnBeforeDrawText(Canvas, dwTextFlags);

				Canvas.TextRect(rc, pszText, dwTextFlags, cchText);
				Canvas.TransparentMode = FALSE;

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

				Graphics::CUICanvas Canvas(hdc);
				Canvas.TransparentMode = TRUE;
				Canvas.Font.Assign(*UITheme::ThemeFont);

				switch (themeType) {
				case ThemeType::TabControl:
					UITheme::PageControl::Event::OnBeforeDrawText(Canvas, dwTextFlags);
					break;
				case ThemeType::Button:
				{
					if (iPartId == BP_PUSHBUTTON)
						UITheme::PushButton::Event::OnBeforeDrawText(Canvas, dwTextFlags, iStateId);
					else if (iPartId == BP_CHECKBOX)
					{
						rc.right += 10; // eliminate end "..." when this is not necessary
						UITheme::CheckBox::Event::OnBeforeDrawText(Canvas, dwTextFlags, iStateId);
					}
					else if (iPartId == BP_RADIOBUTTON)
					{
						rc.right += 10; // eliminate end "..." when this is not necessary
						UITheme::RadioButton::Event::OnBeforeDrawText(Canvas, dwTextFlags, iStateId);
					}
					else if (iPartId == BP_GROUPBOX)
					{
						rc.right += 10; // eliminate end "..." when this is not necessary
						UITheme::GroupBox::Event::OnBeforeDrawText(Canvas, dwTextFlags);
					}

				}
				break;
				case ThemeType::ComboBox:
					UITheme::ComboBox::Event::OnBeforeDrawText(Canvas, dwTextFlags, iStateId);
					break;
				default:
					Canvas.ColorText = UITheme::GetThemeSysColor(UITheme::ThemeColor_Text_4);
					break;
				}

				Canvas.TextRect(rc, pszText, dwTextFlags, cchText);
				Canvas.TransparentMode = FALSE;
			}

			return S_OK;
		}

		HRESULT UIThemePatch::Comctl32DrawThemeBackground(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId,
			LPCRECT pRect, LPCRECT pClipRect)
		{
			auto themeType = ThemeType::None;

			if (auto itr = ThemeHandles.find(hTheme); itr != ThemeHandles.end())
				themeType = itr->second;

			Graphics::CUICanvas Canvas(hdc);

			if (themeType == ThemeType::ScrollBar) 
			{
				switch (iPartId) {
				case SBP_THUMBBTNHORZ:	// Horizontal drag bar
				{
					if (iStateId == SCRBS_HOT || iStateId == SCRBS_PRESSED)
						UITheme::ScrollBar::Render::DrawSliderHorz_Focused(Canvas, pRect);
					else
						UITheme::ScrollBar::Render::DrawSliderHorz_Normal(Canvas, pRect);
				}
				return S_OK;
				case SBP_THUMBBTNVERT:	// Vertical drag bar
				{
					if (iStateId == SCRBS_HOT || iStateId == SCRBS_PRESSED)
						UITheme::ScrollBar::Render::DrawSliderVert_Focused(Canvas, pRect);
					else
						UITheme::ScrollBar::Render::DrawSliderVert_Normal(Canvas, pRect);
				}
				return S_OK;

				case SBP_LOWERTRACKHORZ:// Horizontal background
				case SBP_UPPERTRACKHORZ:// Horizontal background
					UITheme::ScrollBar::Render::DrawBackgroundHorz(Canvas, pRect);
					return S_OK;

				case SBP_LOWERTRACKVERT:// Vertical background
				case SBP_UPPERTRACKVERT:// Vertical background
					UITheme::ScrollBar::Render::DrawBackgroundVert(Canvas, pRect);
					return S_OK;

				case SBP_ARROWBTN:		// Arrow button
				{
					// Assume the perspective of the arrow pointing upward ( /\ ) in GDI coordinates. NOTE: (0, 0) is the
					// top left corner of the screen. Awful code, but it works.
					const INT arrowWidth = std::ceil(std::abs(pRect->left - pRect->right) * 0.4f);
					const INT arrowHeight = std::ceil(std::abs(pRect->top - pRect->bottom) * 0.35f);

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
						UITheme::ScrollBar::Render::DrawButton_Hot(Canvas, pRect);

						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape);
					}
					else if (isDisabled) 
					{
						UITheme::ScrollBar::Render::DrawButton_Disabled(Canvas, pRect);

						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape_Disabled);
					}
					else 
					{
						if (isHorz)
							UITheme::ScrollBar::Render::DrawBackgroundHorz(Canvas, pRect);
						else
							UITheme::ScrollBar::Render::DrawBackgroundVert(Canvas, pRect);

						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape);
					}

					if (isDisabled)
						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape_Shadow_Disabled);
					else
						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape_Shadow);

					PolyPolyline(Canvas.Handle, verts.data(), counts.data(), (DWORD)counts.size());

					if (isHot)
						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape);
					else if (isDisabled)
						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape_Disabled);
					else
						Canvas.Pen.Color = UITheme::GetThemeSysColor(UITheme::ThemeColor_Shape);

					for (auto i = verts.begin(); i != verts.end(); i++)
						(*i).y++;

					PolyPolyline(Canvas.Handle, verts.data(), counts.data(), (DWORD)counts.size());
				}
				return S_OK;

				case SBP_GRIPPERHORZ:	// Horizontal resize scrollbar
				case SBP_GRIPPERVERT:	// Vertical resize scrollbar
					break;
				case SBP_SIZEBOX:		// Resize box, bottom right
				case SBP_SIZEBOXBKGND:	// Resize box, background, unused
					Canvas.Fill(*pRect, UITheme::GetThemeSysColor(UITheme::ThemeColor_Default));
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
					UITheme::StatusBar::Render::DrawBorder(Canvas, pRect);
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
							UITheme::UpDown::Render::DrawUp_Hot(Canvas, pRect);
							break;
						case DNS_PRESSED:
							UITheme::UpDown::Render::DrawUp_Pressed(Canvas, pRect);
							break;
						case DNS_DISABLED:
							UITheme::UpDown::Render::DrawUp_Disabled(Canvas, pRect);
							break;
						default:
							UITheme::UpDown::Render::DrawUp_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case SPNP_DOWN: 
					{
						switch (iStateId) 
						{
						case UPS_HOT:
							UITheme::UpDown::Render::DrawDown_Hot(Canvas, pRect);
							break;
						case UPS_PRESSED:
							UITheme::UpDown::Render::DrawDown_Pressed(Canvas, pRect);
							break;
						case UPS_DISABLED:
							UITheme::UpDown::Render::DrawDown_Disabled(Canvas, pRect);
							break;
						default:
							UITheme::UpDown::Render::DrawDown_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case SPNP_UPHORZ: 
					{
						switch (iStateId) 
						{
						case UPHZS_HOT:
							UITheme::UpDown::Render::DrawRight_Hot(Canvas, pRect);
							break;
						case UPHZS_PRESSED:
							UITheme::UpDown::Render::DrawRight_Pressed(Canvas, pRect);
							break;
						case UPHZS_DISABLED:
							UITheme::UpDown::Render::DrawRight_Disabled(Canvas, pRect);
							break;
						default:
							UITheme::UpDown::Render::DrawRight_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case SPNP_DOWNHORZ: 
					{
						switch (iStateId)
						{
						case DNHZS_HOT:
							UITheme::UpDown::Render::DrawLeft_Hot(Canvas, pRect);
							break;
						case DNHZS_PRESSED:
							UITheme::UpDown::Render::DrawLeft_Pressed(Canvas, pRect);
							break;
						case DNHZS_DISABLED:
							UITheme::UpDown::Render::DrawLeft_Disabled(Canvas, pRect);
							break;
						default:
							UITheme::UpDown::Render::DrawLeft_Normal(Canvas, pRect);
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
							UITheme::EditText::Render::DrawEditText_Disabled(Canvas, pRect);
							break;
						case ETS_SELECTED:
						case ETS_FOCUSED:
							UITheme::EditText::Render::DrawEditText_Focused(Canvas, pRect);
							break;
						case ETS_HOT:
							UITheme::EditText::Render::DrawEditText_Hot(Canvas, pRect);
							break;
						default:
							UITheme::EditText::Render::DrawEditText_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case EP_EDITBORDER_VSCROLL: 
					{
						switch (iStateId)
						{
						case EPSV_DISABLED:
							UITheme::EditText::Render::DrawEditText_Disabled(Canvas, pRect);
							break;
						case EPSV_FOCUSED:
							UITheme::EditText::Render::DrawEditText_Focused(Canvas, pRect);
							break;
						case EPSV_HOT:
							UITheme::EditText::Render::DrawEditText_Hot(Canvas, pRect);
							break;
						default:
							UITheme::EditText::Render::DrawEditText_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case EP_EDITBORDER_HSCROLL: 
					{
						switch (iStateId) 
						{
						case EPSH_DISABLED:
							UITheme::EditText::Render::DrawEditText_Disabled(Canvas, pRect);
							break;
						case EPSH_FOCUSED:
							UITheme::EditText::Render::DrawEditText_Focused(Canvas, pRect);
							break;
						case EPSH_HOT:
							UITheme::EditText::Render::DrawEditText_Hot(Canvas, pRect);
							break;
						default:
							UITheme::EditText::Render::DrawEditText_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case EP_EDITBORDER_HVSCROLL:
					{
						switch (iStateId) 
						{
						case EPSHV_DISABLED:
							UITheme::EditText::Render::DrawEditText_Disabled(Canvas, pRect);
							break;
						case EPSHV_FOCUSED:
							UITheme::EditText::Render::DrawEditText_Focused(Canvas, pRect);
							break;
						case EPSHV_HOT:
							UITheme::EditText::Render::DrawEditText_Hot(Canvas, pRect);
							break;
						default:
							UITheme::EditText::Render::DrawEditText_Normal(Canvas, pRect);
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
					UITheme::ProgressBar::Render::DrawBar(Canvas, pRect);
					return S_OK;
				case PP_FILL:
					UITheme::ProgressBar::Render::DrawFill(Canvas, pRect);
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
							UITheme::PushButton::Render::DrawPushButton_Hot(Canvas, pRect);
							break;
						case PBS_DISABLED:
							UITheme::PushButton::Render::DrawPushButton_Disabled(Canvas, pRect);
							break;
						case PBS_PRESSED:
							UITheme::PushButton::Render::DrawPushButton_Pressed(Canvas, pRect);
							break;
						default:
							UITheme::PushButton::Render::DrawPushButton_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case BP_GROUPBOX: {
						switch (iStateId) {
						case GBS_DISABLED:
							UITheme::GroupBox::Render::DrawGroupBox_Disabled(Canvas, pRect);
							break;
						default:
							UITheme::GroupBox::Render::DrawGroupBox_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case BP_CHECKBOX: 
					{
						switch (iStateId) 
						{
						case CBS_UNCHECKEDDISABLED:
							UITheme::PushButton::Render::DrawPushButton_Disabled(Canvas, pRect);
							break;
						case CBS_UNCHECKEDNORMAL:
							UITheme::PushButton::Render::DrawPushButton_Normal(Canvas, pRect);
							break;
						case CBS_UNCHECKEDHOT:
							UITheme::PushButton::Render::DrawPushButton_Hot(Canvas, pRect);
							break;
						case CBS_UNCHECKEDPRESSED:
							UITheme::PushButton::Render::DrawPushButton_Pressed(Canvas, pRect);
							break;
						case CBS_CHECKEDDISABLED:
							UITheme::PushButton::Render::DrawPushButton_Disabled(Canvas, pRect);
							UITheme::CheckBox::Render::DrawCheck_Disabled(Canvas, pRect);
							break;
						case CBS_CHECKEDNORMAL:
							UITheme::PushButton::Render::DrawPushButton_Normal(Canvas, pRect);
							UITheme::CheckBox::Render::DrawCheck_Normal(Canvas, pRect);
							break;
						case CBS_CHECKEDHOT:
							UITheme::PushButton::Render::DrawPushButton_Hot(Canvas, pRect);
							UITheme::CheckBox::Render::DrawCheck_Hot(Canvas, pRect);
							break;
						case CBS_CHECKEDPRESSED:
							UITheme::PushButton::Render::DrawPushButton_Pressed(Canvas, pRect);
							UITheme::CheckBox::Render::DrawCheck_Pressed(Canvas, pRect);
							break;
						case CBS_MIXEDDISABLED:
							UITheme::PushButton::Render::DrawPushButton_Disabled(Canvas, pRect);
							UITheme::CheckBox::Render::DrawIdeterminate_Disabled(Canvas, pRect);
							break;
						case CBS_MIXEDNORMAL:
							UITheme::PushButton::Render::DrawPushButton_Normal(Canvas, pRect);
							UITheme::CheckBox::Render::DrawIdeterminate_Normal(Canvas, pRect);
							break;
						case CBS_MIXEDHOT:
							UITheme::PushButton::Render::DrawPushButton_Hot(Canvas, pRect);
							UITheme::CheckBox::Render::DrawIdeterminate_Hot(Canvas, pRect);
							break;
						case CBS_MIXEDPRESSED:
							UITheme::PushButton::Render::DrawPushButton_Pressed(Canvas, pRect);
							UITheme::CheckBox::Render::DrawIdeterminate_Pressed(Canvas, pRect);
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
							UITheme::RadioButton::Render::DrawPushButtonR_Disabled(Canvas, pRect);
							break;
						case RBS_UNCHECKEDNORMAL:
							UITheme::RadioButton::Render::DrawPushButtonR_Normal(Canvas, pRect);
							break;
						case RBS_UNCHECKEDHOT:
							UITheme::RadioButton::Render::DrawPushButtonR_Hot(Canvas, pRect);
							break;
						case RBS_UNCHECKEDPRESSED:
							UITheme::RadioButton::Render::DrawPushButtonR_Pressed(Canvas, pRect);
							break;
						case RBS_CHECKEDDISABLED:
							UITheme::RadioButton::Render::DrawPushButtonR_Disabled(Canvas, pRect);
							UITheme::RadioButton::Render::DrawRadioButton_Disabled(Canvas, pRect);
							break;
						case RBS_CHECKEDNORMAL:
							UITheme::RadioButton::Render::DrawPushButtonR_Normal(Canvas, pRect);
							UITheme::RadioButton::Render::DrawRadioButton_Normal(Canvas, pRect);
							break;
						case RBS_CHECKEDHOT:
							UITheme::RadioButton::Render::DrawPushButtonR_Hot(Canvas, pRect);
							UITheme::RadioButton::Render::DrawRadioButton_Hot(Canvas, pRect);
							break;
						case RBS_CHECKEDPRESSED:
							UITheme::RadioButton::Render::DrawPushButtonR_Pressed(Canvas, pRect);
							UITheme::RadioButton::Render::DrawRadioButton_Pressed(Canvas, pRect);
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
						UITheme::TrackBar::Render::DrawTrack(Canvas, pRect);
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
							UITheme::TrackBar::Render::DrawSlider_Hot(Canvas, pRect);
							break;
						case TUS_PRESSED:
							UITheme::TrackBar::Render::DrawSlider_Pressed(Canvas, pRect);
							break;
						case TUS_FOCUSED:
							UITheme::TrackBar::Render::DrawSlider_Focused(Canvas, pRect);
							break;
						case TUS_DISABLED:
							UITheme::TrackBar::Render::DrawSlider_Disabled(Canvas, pRect);
							break;
						default:
							UITheme::TrackBar::Render::DrawSlider_Normal(Canvas, pRect);
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
							UITheme::PushButton::Render::DrawPushButton_Hot(Canvas, pRect);
							break;
						case CBRO_DISABLED:
							UITheme::PushButton::Render::DrawPushButton_Disabled(Canvas, pRect);
							break;
						case CBRO_PRESSED:
							UITheme::PushButton::Render::DrawPushButton_Pressed(Canvas, pRect);
							break;
						default:
							UITheme::PushButton::Render::DrawPushButton_Normal(Canvas, pRect);
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
							UITheme::EditText::Render::DrawEditText_Hot(Canvas, pRect);
							break;
						case CBB_DISABLED:
							UITheme::EditText::Render::DrawEditText_Disabled(Canvas, pRect);
							break;
						case CBB_FOCUSED:
							UITheme::EditText::Render::DrawEditText_Focused(Canvas, pRect);
							break;
						default:
							UITheme::EditText::Render::DrawEditText_Normal(Canvas, pRect);
							break;
						}
					}
					return S_OK;

					case CP_DROPDOWNBUTTONRIGHT:// Dropdown arrow
					case CP_DROPDOWNBUTTONLEFT:	// Dropdown arrow
						if (iStateId == CBXS_DISABLED)
							UITheme::ComboBox::Render::DrawArrow_Disabled(Canvas, pRect);
						else
							UITheme::ComboBox::Render::DrawArrow_Normal(Canvas, pRect);
						return S_OK;

					case CP_DROPDOWNBUTTON:
						Canvas.Fill(*pRect, RGB(255, 0, 0));
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
							UITheme::Header::Render::DrawBack_Normal(Canvas, pRect);
						else if ((iPartId == 0 && iStateId == HIS_HOT) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_HOT) ||
							(iPartId == HP_HEADERITEMLEFT && iStateId == HILS_HOT) ||
							(iPartId == HP_HEADERITEMRIGHT && iStateId == HIRS_HOT))
							UITheme::Header::Render::DrawBack_Hot(Canvas, pRect);
						else if ((iPartId == 0 && iStateId == HIS_PRESSED) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_PRESSED) ||
							(iPartId == HP_HEADERITEMLEFT && iStateId == HILS_PRESSED) ||
							(iPartId == HP_HEADERITEMRIGHT && iStateId == HIRS_PRESSED))
							UITheme::Header::Render::DrawBack_Pressed(Canvas, pRect);
						else if ((iPartId == HP_HEADERITEM && iStateId == HIS_SORTEDNORMAL) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_SORTEDHOT) ||
							(iPartId == HP_HEADERITEM && iStateId == HIS_SORTEDPRESSED))
							UITheme::Header::Render::DrawBack_Selected(Canvas, pRect);

						UITheme::Header::Render::DrawDiv(Canvas, pRect);
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
						UITheme::PageControl::Render::DrawBorder(Canvas, pRect);
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
						Graphics::CRECT rc = *pRect;

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
							UITheme::PageControl::Render::DrawButton_SelectedAndFocused(Canvas, pRect);
						else if (isHover)
							UITheme::PageControl::Render::DrawButton_Hot(Canvas, pRect);
						else if (isDisabled)
							UITheme::PageControl::Render::DrawButton_Disabled(Canvas, pRect);
						else
							UITheme::PageControl::Render::DrawButton_Normal(Canvas, pRect);
					}
					break;
				}
				return S_OK;
			}

			return DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
		}
	}
}