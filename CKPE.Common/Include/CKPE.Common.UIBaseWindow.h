// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.UIMenus.h>
#include <CKPE.Graphics.h>
#include <CKPE.MessageBox.h>

#define UI_CHANGEWINDOWSTATE (WM_USER + 0xC10)

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			enum WindowState_t : std::uint32_t
			{
				wsNormal = 0,
				wsMaximized,
				wsMinimized,
				wsHide
			};

			class CKPE_COMMON_API CUIBaseWindow;
			class CKPE_COMMON_API CUIBaseControl;
			class CKPE_COMMON_API CUICustomWindow;
			class CKPE_COMMON_API CUIMainWindow;
			class CKPE_COMMON_API CUIToolWindow;

			class CKPE_COMMON_API CUIBaseWindow
			{
			private:
				bool m_LockUpdate{ false };
				HDC m_hDC{ nullptr };
			protected:
				HWND m_hWnd{ nullptr };
			public:
				[[nodiscard]] inline HWND GetHandle() const noexcept(true) { return m_hWnd; }
				[[nodiscard]] WindowState_t GetWindowState() const noexcept(true);
				void SetWindowState(const WindowState_t state) noexcept(true);
				[[nodiscard]] bool GetVisible() noexcept(true);
				[[nodiscard]] bool GetConstVisible() const noexcept(true);
				void SetVisible(const bool value) noexcept(true);
				[[nodiscard]] std::string GetCaption() const noexcept(true);
				void SetCaption(const std::string &value) noexcept(true);
				[[nodiscard]] std::wstring GetWideCaption() const noexcept(true);
				void SetWideCaption(const std::wstring &value) noexcept(true);
				[[nodiscard]] std::uintptr_t GetStyle() const noexcept(true);
				void SetStyle(const std::uintptr_t value) noexcept(true);
				[[nodiscard]] std::uintptr_t GetStyleEx() const noexcept(true);
				void SetStyleEx(const std::uintptr_t value) noexcept(true);
				[[nodiscard]] bool GetEnabled() const noexcept(true);
				void SetEnabled(const bool value) noexcept(true);
				[[nodiscard]] std::string GetName() const noexcept(true);
				[[nodiscard]] std::wstring GetWideName() const noexcept(true);
				[[nodiscard]] CRECT GetBoundsRect() const noexcept(true);
				virtual void SetBoundsRect(const CRECT &bounds) noexcept(true);
				[[nodiscard]] std::int32_t GetLeft() const noexcept(true);
				virtual void SetLeft(const std::int32_t value) noexcept(true);
				[[nodiscard]] std::int32_t GetTop() const noexcept(true);
				virtual void SetTop(const std::int32_t value) noexcept(true);
				[[nodiscard]] std::int32_t GetWidth() const noexcept(true);
				virtual void SetWidth(const std::int32_t value) noexcept(true);
				[[nodiscard]] std::int32_t GetHeight() const noexcept(true);
				virtual void SetHeight(const std::int32_t value) noexcept(true);
			public:
				[[nodiscard]] bool Is() const noexcept(true);
				[[nodiscard]] bool IsWindowMaximized() const noexcept(true);
				[[nodiscard]] CRECT WindowRect() const noexcept(true);
				[[nodiscard]] CRECT ClientRect() const noexcept(true);
				[[nodiscard]] std::uint32_t DpiForWindow() const noexcept(true);
				[[nodiscard]] std::int32_t ClientWidth() const noexcept(true);
				[[nodiscard]] std::int32_t ClientHeight() const noexcept(true);
				void Move(const std::int32_t x, const std::int32_t y, const bool topmost = true) noexcept(true);
				void SetSize(const std::int32_t cx, const std::int32_t cy, const bool topmost = true) noexcept(true);
				void SetParent(HWND hParent) noexcept(true);
				void SetParent(const CUIBaseWindow& Parent) noexcept(true);
				[[nodiscard]] HWND Parent() const noexcept(true);
				[[nodiscard]] CUIBaseWindow ParentWindow() const noexcept(true);
				[[nodiscard]] POINT ScreenToControl(const POINT &p) const noexcept(true);
				[[nodiscard]] POINT ControlToScreen(const POINT &p) const noexcept(true);
				[[nodiscard]] inline bool IsLockUpdate() const noexcept(true) { return m_LockUpdate; }
				void Refresh() noexcept(true);
				void LockUpdate() noexcept(true);
				void UnlockUpdate() noexcept(true);
				void Invalidate() noexcept(true);
				void Invalidate(const LPRECT r) noexcept(true);
				void Invalidate(const CRECT& r) noexcept(true);
				void Repaint() noexcept(true);
				void SetFocus() noexcept(true);
				[[nodiscard]] inline HDC DeviceContext() const noexcept(true) { return m_hDC; }
			public:
				virtual LRESULT WndProc(HWND hWnd, std::uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept(true);
			public:
				LRESULT Perform(std::uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept(true);
				LRESULT Perform(HWND hWnd, std::uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept(true);
			public:
				CUIBaseWindow() noexcept(true) : m_hDC(GetDC(0))
					{}
				CUIBaseWindow(const HWND hWnd) noexcept(true) : m_hWnd(hWnd), m_hDC(GetDC(hWnd))
					{}
				CUIBaseWindow(const CUIBaseWindow& base) : m_LockUpdate(base.m_LockUpdate), m_hWnd(base.m_hWnd), m_hDC(GetDC(m_hWnd))
					{}
				virtual ~CUIBaseWindow() noexcept(true) { ReleaseDC(m_hWnd, m_hDC); }
			public:
				CKPE_READ_PROPERTY(GetHandle) HWND Handle;
				CKPE_PROPERTY(GetWindowState, SetWindowState) WindowState_t WindowState;
				CKPE_PROPERTY(GetVisible, SetVisible) bool Visible;
				CKPE_PROPERTY(GetConstVisible, SetVisible) const bool ConstVisible;
				// no dll interface (what??? okay)
				//CKPE_PROPERTY(GetCaption, SetCaption) std::string Caption;
				//CKPE_PROPERTY(GetWideCaption, SetWideCaption) const std::wstring WideCaption;
				CKPE_PROPERTY(GetStyle, SetStyle) std::uintptr_t Style;
				CKPE_PROPERTY(GetStyleEx, SetStyleEx) std::uintptr_t StyleEx;
				CKPE_PROPERTY(GetEnabled, SetEnabled) bool Enabled;
				// no dll interface (what??? okay)
				//CKPE_READ_PROPERTY(GetName) const std::string Name;
				//CKPE_READ_PROPERTY(GetWideName) const std::wstring WideName;
				CKPE_PROPERTY(GetBoundsRect, SetBoundsRect) CRECT BoundsRect;
				CKPE_PROPERTY(GetLeft, SetLeft) std::int32_t Left;
				CKPE_PROPERTY(GetTop, SetTop) std::int32_t Top;
				CKPE_PROPERTY(GetWidth, SetWidth) std::int32_t Width;
				CKPE_PROPERTY(GetHeight, SetHeight) std::int32_t Height;
			};

			class CKPE_COMMON_API CUICustomWindow : public CUIBaseWindow
			{
			public:
				CUIBaseControl GetControl(const std::uint32_t index) noexcept(true);
				void Foreground() noexcept(true);
			public:
				CUICustomWindow() noexcept(true) : CUIBaseWindow() {}
				CUICustomWindow(const HWND hWnd) noexcept(true) : CUIBaseWindow(hWnd) {}
				CUICustomWindow(const CUICustomWindow &base) : CUIBaseWindow(base) {}
			};

			class CKPE_COMMON_API CUIToolWindow : public CUIBaseWindow
			{
			public:
				inline void SetBoundsRect(const CRECT &) noexcept(true) {}
				inline void SetLeft(const std::int32_t) noexcept(true) {}
				inline void SetTop(const std::int32_t) noexcept(true) {}
				inline void SetWidth(const std::int32_t) noexcept(true) {}
				inline void SetHeight(const std::int32_t) noexcept(true) {}
			public:
				CUIToolWindow() : CUIBaseWindow() {}
				CUIToolWindow(const HWND hWnd) : CUIBaseWindow(hWnd) {}
				CUIToolWindow(const CUIToolWindow &base) : CUIBaseWindow(base) {}
			};

			class CKPE_COMMON_API CUIMainWindow : public CUICustomWindow
			{
			private:
				CUIMenu m_MainMenu;
			public:
				void FindToolWindow() noexcept(true);
			public:
				void SetTextToStatusBar(const std::uint32_t index, const std::string& text) noexcept(true);
				void SetTextToStatusBar(const std::uint32_t index, const std::wstring& text) noexcept(true);
				[[nodiscard]] std::string GetTextToStatusBarA(const std::uint32_t index) noexcept(true);
				[[nodiscard]] std::wstring GetTextToStatusBarW(const std::uint32_t index) noexcept(true);
				static void ProcessMessages() noexcept(true);
				static MessageBox::Result MessageDlg(const std::string& message, const std::string& caption,
					const MessageBox::Buttons buttons, const MessageBox::Icon icon) noexcept(true);
				static MessageBox::Result MessageDlg(const std::wstring& message, const std::wstring& caption,
					const MessageBox::Buttons buttons, const MessageBox::Icon icon) noexcept(true);
				static MessageBox::Result MessageWarningDlg(const std::string& message) noexcept(true);
				static MessageBox::Result MessageWarningDlg(const std::wstring& message) noexcept(true);
			public:
				CUIMainWindow() noexcept(true) : CUICustomWindow() {}
				CUIMainWindow(const HWND hWnd) noexcept(true) : CUICustomWindow(hWnd) {}
				CUIMainWindow(const CUIMainWindow &base) : CUICustomWindow(base) {}
			public:
				CUIMenu MainMenu;
				CUIToolWindow Toolbar;
				CUIToolWindow Statusbar;
			};

			class CKPE_COMMON_API CUICustomDialog : public CUICustomWindow
			{
			private:
				std::uint32_t m_ResId;
				DLGPROC m_DlgProc;
				CUICustomWindow* m_Parent;
			public:
				void Create() noexcept(true);
				void FreeRelease() noexcept(true);
				[[nodiscard]] inline std::uint32_t GetResourceID() const noexcept(true) { return m_ResId; }
				[[nodiscard]] inline DLGPROC GetDialogProc() const noexcept(true) { return m_DlgProc; }
			public:
				CUICustomDialog(CUICustomWindow* parent, const std::uint32_t res_id, DLGPROC dlg_proc) noexcept(true);
				virtual ~CUICustomDialog() noexcept(true);
			public:
				CKPE_READ_PROPERTY(GetResourceID) std::uint32_t ResourceID;
				CKPE_READ_PROPERTY(GetDialogProc) DLGPROC DialogProc;
			};

			class CKPE_COMMON_API CUIBaseControl : public CUIBaseWindow
			{
			public:
				CUIBaseControl() noexcept(true) : CUIBaseWindow() {}
				CUIBaseControl(const HWND hWnd) noexcept(true) : CUIBaseWindow(hWnd) {}
				CUIBaseControl(const CUIBaseControl &base) : CUIBaseWindow(base) {}

				[[nodiscard]] std::uint16_t GetID() const noexcept(true);

				CKPE_READ_PROPERTY(GetID) std::uint16_t ID;
			};
		}
	}
}
