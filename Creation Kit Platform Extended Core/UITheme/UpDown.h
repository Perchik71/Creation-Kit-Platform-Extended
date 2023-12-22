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
		namespace UpDown
		{
			namespace Graphics = ::Core::Classes::UI;

			namespace Render
			{
				VOID DrawUp_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
					COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor);
				VOID DrawUp_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawUp_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawUp_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawUp_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawDown_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
					COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor);
				VOID DrawDown_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawDown_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawDown_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawDown_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);

				VOID DrawLeft_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
					COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, 
					COLORREF cHihglighter, BOOL drawHihglighter);
				VOID DrawLeft_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawLeft_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawLeft_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawLeft_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawRight_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
					COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, 
					COLORREF cHihglighter, BOOL drawHihglighter);
				VOID DrawRight_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawRight_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawRight_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID  DrawRight_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
			}

			VOID Initialize(HWND hWindow);
			LRESULT CALLBACK UpDownSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
				UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
		}
	}
}