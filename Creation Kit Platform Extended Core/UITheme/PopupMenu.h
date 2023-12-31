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

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		namespace PopupMenu
		{
			namespace Graphics = ::Core::Classes::UI;

			namespace Render
			{
				VOID DrawBackground_NonClientArray(Graphics::CUICanvas& canvas);
				VOID DrawItem_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawItem_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawItem_Divider(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawItem_Checkbox(Graphics::CUICanvas& canvas, LPCRECT pRect, BOOL bSelected, BOOL bDisabled);
				VOID DrawItem_Arrow(Graphics::CUICanvas& canvas, LPCRECT pRect, BOOL bSelected);
			}

			BOOL IsSystemPopupMenu(HWND hWindow, HMENU hMenu);
			BOOL IsSystemPopupMenuBlindly(HWND hWindow);

			VOID Initialize(HWND hWindow);
			LRESULT CALLBACK PopupMenuSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
				UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

			namespace Event
			{
				VOID OnInitPopupMenu(HWND hWindow, HMENU hMenu);
				VOID OnDrawNoClientPopupMenu(HWND hWindow, HDC hDC);
				VOID OnMeasureItem(HWND hWindow, LPMEASUREITEMSTRUCT lpMeasureItem);
				VOID OnDrawItem(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem);
			}
		}
	}
}