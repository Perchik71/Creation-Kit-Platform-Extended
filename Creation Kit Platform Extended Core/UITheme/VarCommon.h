//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
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

#include "Editor API/UI/UIGraphics.h"
#include "Colored.h"

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		extern ::Core::Classes::UI::CUIFont* ThemeFont;

		extern HBRUSH hThemeDefaultBrush;
		extern HBRUSH hThemeText3Brush;
		extern HBRUSH hThemeEditBrush;
		extern HBRUSH hThemeDividerBrush;
		extern HBRUSH hThemeText4Brush;
		extern HBRUSH hThemeBorderWindowBrush;

		Theme GetTheme();
		void SetTheme(Theme theme);

		bool IsDarkThemeSystemSupported();
		bool IsCustomThemeExists();
		bool OpenCustomTheme();
		bool NeedDarkCheck();
		COLORREF GetMaskColorToolbarForCustomTheme();
		String GetFileNameToolbarForCustomTheme();
		COLORREF GetMaskColorToolbarNavMeshForCustomTheme();
		String GetFileNameToolbarNavMeshForCustomTheme();
		COLORREF GetMaskColorIconsForCustomTheme();
		String GetFileNameIconsForCustomTheme();

		inline COLORREF GetThemeSysColor(const ThemeColor color);

		DWORD Comctl32GetSysColor(INT nIndex);
		HBRUSH Comctl32GetSysColorBrush(INT nIndex);
		HBRUSH Comctl32GetSysColorBrushEx(INT nIndex, BOOL reCreate = TRUE);
		HBITMAP LoadImageFromResource(HINSTANCE hInst, DWORD dwResId, LPCSTR ResType);

		bool IsDarkTheme();
	}
}