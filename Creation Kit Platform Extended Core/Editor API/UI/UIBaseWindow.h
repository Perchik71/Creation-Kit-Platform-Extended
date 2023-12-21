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

#pragma once

#include "UIMenus.h"
#include "UIGraphics.h"

#define UI_CHANGEWINDOWSTATE (WM_USER + 0xC10)

namespace Core
{
	enum WindowState_t {
		wsNormal,
		wsMaximized,
		wsMinimized,
		wsHide
	};

	namespace Classes
	{
		namespace UI
		{
			class CUIBaseWindow;
			class CUIBaseControl;
			class CUICustomWindow;
			class CUIMainWindow;
			class CUIToolWindow;

			class CUIBaseWindow
			{
			private:
				BOOL m_LockUpdate;
				HDC m_hDC;
			protected:
				HWND m_hWnd;
			public:
				inline HWND GetHandle(void) const { return m_hWnd; }
				WindowState_t GetWindowState(void) const;
				void SetWindowState(const WindowState_t state);
				BOOL GetVisible(void);
				BOOL GetConstVisible(void) const;
				void SetVisible(const BOOL value);
				std::string GetCaption(void) const;
				void SetCaption(const std::string &value);
				std::wstring GetWideCaption(void) const;
				void SetWideCaption(const std::wstring &value);
				ULONG_PTR GetStyle(void) const;
				void SetStyle(const ULONG_PTR value);
				ULONG_PTR GetStyleEx(void) const;
				void SetStyleEx(const ULONG_PTR value);
				BOOL GetEnabled(void) const;
				void SetEnabled(const BOOL value);
				std::string GetName(void) const;
				std::wstring GetWideName(void) const;
				CRECT GetBoundsRect(void) const;
				virtual void SetBoundsRect(const CRECT &bounds);
				LONG GetLeft(void) const;
				virtual void SetLeft(const LONG value);
				LONG GetTop(void) const;
				virtual void SetTop(const LONG value);
				LONG GetWidth(void) const;
				virtual void SetWidth(const LONG value);
				LONG GetHeight(void) const;
				virtual void SetHeight(const LONG value);
			public:
				BOOL Is(void) const;
				BOOL IsWindowMaximized(void) const;
				CRECT WindowRect(void) const;
				CRECT ClientRect(void) const;
				UINT DpiForWindow(void) const;
				LONG ClientWidth(void) const;
				LONG ClientHeight(void) const;
				void Move(const LONG x, const LONG y, const BOOL topmost = TRUE);
				void SetSize(const LONG cx, const LONG cy, const BOOL topmost = TRUE);
				void SetParent(HWND hParent);
				void SetParent(const CUIBaseWindow& Parent);
				HWND Parent(void) const;
				CUIBaseWindow ParentWindow(void) const;
				POINT ScreenToControl(const POINT &p) const;
				POINT ControlToScreen(const POINT &p) const;
				inline BOOL IsLockUpdate(void) const { return m_LockUpdate; }
				void Refresh(void);
				void LockUpdate(void);
				void UnlockUpdate(void);
				void Invalidate(void);
				void Invalidate(const LPRECT r);
				void Invalidate(const CRECT& r);
				void Repaint(void);
				void SetFocus(void);
				inline HDC DeviceContext(void) const { return m_hDC; }
			public:
				virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			public:
				LRESULT Perform(UINT uMsg, WPARAM wParam, LPARAM lParam);
				LRESULT Perform(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			public:
				CUIBaseWindow(void) : m_LockUpdate(FALSE), m_hWnd(NULL), m_hDC(GetDC(0))
					{ }
				CUIBaseWindow(const HWND hWnd) : m_LockUpdate(FALSE), m_hWnd(hWnd), m_hDC(GetDC(hWnd))
					{ }
				CUIBaseWindow(const CUIBaseWindow &base) : m_LockUpdate(base.m_LockUpdate), m_hWnd(base.m_hWnd), m_hDC(GetDC(m_hWnd))
					{ }
				virtual ~CUIBaseWindow(void) { ReleaseDC(m_hWnd, m_hDC); }
			public:
				READ_PROPERTY(GetHandle) HWND Handle;
				PROPERTY(GetWindowState, SetWindowState) WindowState_t WindowState;
				PROPERTY(GetConstWindowState, SetWindowState) const WindowState_t ConstWindowState;
				PROPERTY(GetVisible, SetVisible) BOOL Visible;
				PROPERTY(GetConstVisible, SetVisible) const BOOL ConstVisible;
				PROPERTY(GetCaption, SetCaption) std::string Caption;
				PROPERTY(GetWideCaption, SetWideCaption) const std::wstring WideCaption;
				PROPERTY(GetStyle, SetStyle) ULONG_PTR Style;
				PROPERTY(GetStyleEx, SetStyleEx) ULONG_PTR StyleEx;
				PROPERTY(GetEnabled, SetEnabled) BOOL Enabled;
				READ_PROPERTY(GetName) const std::string Name;
				READ_PROPERTY(GetWideName) const std::wstring WideName;
				PROPERTY(GetBoundsRect, SetBoundsRect) CRECT BoundsRect;
				PROPERTY(GetLeft, SetLeft) LONG Left;
				PROPERTY(GetTop, SetTop) LONG Top;
				PROPERTY(GetWidth, SetWidth) LONG Width;
				PROPERTY(GetHeight, SetHeight) LONG Height;
			};

			class CUICustomWindow : public CUIBaseWindow
			{
			public:
				CUIBaseControl GetControl(const uint32_t index);
				void Foreground(void);
			public:
				CUICustomWindow(void) : CUIBaseWindow() {}
				CUICustomWindow(const HWND hWnd) : CUIBaseWindow(hWnd) {}
				CUICustomWindow(const CUICustomWindow &base) : CUIBaseWindow(base) {}
			};

			class CUIToolWindow : public CUIBaseWindow
			{
			public:
				inline void SetBoundsRect(const CRECT &) {}
				inline void SetLeft(const LONG) {}
				inline void SetTop(const LONG) {}
				inline void SetWidth(const LONG) {}
				inline void SetHeight(const LONG) {}
			public:
				CUIToolWindow(void) : CUIBaseWindow() {}
				CUIToolWindow(const HWND hWnd) : CUIBaseWindow(hWnd) {}
				CUIToolWindow(const CUIToolWindow &base) : CUIBaseWindow(base) {}
			};

			class CUIMainWindow : public CUICustomWindow
			{
			private:
				CUIMenu m_MainMenu;
			public:
				void FindToolWindow(void);
			public:
				void SetTextToStatusBar(const uint32_t index, const std::string text);
				void SetTextToStatusBar(const uint32_t index, const std::wstring text);
				std::string GetTextToStatusBarA(const uint32_t index);
				std::wstring GetTextToStatusBarW(const uint32_t index);
				static void ProcessMessages(void);
				static INT32 MessageDlg(const std::string message, const std::string caption, const UINT32 flags);
				static INT32 MessageDlg(const std::wstring message, const std::wstring caption, const UINT32 flags);
				static INT32 MessageWarningDlg(const std::string message);
				static INT32 MessageWarningDlg(const std::wstring message);
			public:
				CUIMainWindow(void) : CUICustomWindow() {}
				CUIMainWindow(const HWND hWnd) : CUICustomWindow(hWnd) {}
				CUIMainWindow(const CUIMainWindow &base) : CUICustomWindow(base) {}
			public:
				CUIMenu MainMenu;
				CUIToolWindow Toolbar;
				CUIToolWindow Statusbar;
			};

			class CUICustomDialog : public CUICustomWindow
			{
			private:
				UINT m_ResId;
				DLGPROC m_DlgProc;
				CUICustomWindow* m_Parent;
			public:
				void Create(void);
				void FreeRelease(void);
				inline UINT GetResourceID() const { return m_ResId; }
				inline DLGPROC GetDialogProc() const { return m_DlgProc; }
			public:
				CUICustomDialog(CUICustomWindow* parent, const UINT res_id, DLGPROC dlg_proc);
				virtual ~CUICustomDialog(void);
			public:
				READ_PROPERTY(GetResourceID) UINT ResourceID;
				READ_PROPERTY(GetDialogProc) DLGPROC DialogProc;
			};

			class CUIBaseControl : public CUIBaseWindow
			{
			public:
				CUIBaseControl(void) : CUIBaseWindow() {}
				CUIBaseControl(const HWND hWnd) : CUIBaseWindow(hWnd) {}
				CUIBaseControl(const CUIBaseControl &base) : CUIBaseWindow(base) {}
			};
		}
	}
}
