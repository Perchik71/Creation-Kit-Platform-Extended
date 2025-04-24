// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

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

		class UIThemePatch : public Module
		{
		public:
			UIThemePatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static void InitializeCurrentThread();
			static void InitializeThread(uint32_t u32ThreadId);
			static bool ExcludeSubclassKnownWindows(HWND hWindow, BOOL bRemoved = FALSE);
			static ThemeType GetThemeTypeFromWindow(HWND hWindow);
			static bool RegisterThemeHandle(HWND hWindow, ThemeType eTheme);
			static bool RegisterThemeHandle(HTHEME hTheme, ThemeType eTheme);

			static LRESULT CALLBACK CallWndProcCallback(INT nCode, WPARAM wParam, LPARAM lParam);
			static LRESULT CALLBACK WindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
				UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			static LRESULT CALLBACK DialogSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
				UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

			static void ApplyThemeForWindow(HWND hWnd);
			static DWORD Comctl32GetSysColor(INT nIndex);
			static HBRUSH Comctl32GetSysColorBrush(INT nIndex);
			static HRESULT Comctl32DrawThemeText(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId, 
				LPCWSTR pszText, INT cchText, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);
			static HRESULT Comctl32DrawThemeTextEx(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId,
				LPCWSTR pszText, INT cchText, DWORD dwTextFlags, LPRECT pRect, const DTTOPTS* pOptions);
			static HRESULT Comctl32DrawThemeBackground(HTHEME hTheme, HDC hdc, INT iPartId, INT iStateId, 
				LPCRECT pRect, LPCRECT pClipRect);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			UIThemePatch(const UIThemePatch&) = default;
			UIThemePatch& operator=(const UIThemePatch&) = default;
		};
	}
}