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

#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace UpDown
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawUp_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor);
					VOID FIXAPI DrawUp_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawUp_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawUp_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawUp_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor);
					VOID FIXAPI DrawDown_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);

					VOID FIXAPI DrawLeft_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, 
						COLORREF cHihglighter, BOOL drawHihglighter);
					VOID FIXAPI DrawLeft_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawLeft_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawLeft_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawLeft_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, 
						COLORREF cHihglighter, BOOL drawHihglighter);
					VOID FIXAPI DrawRight_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				VOID FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK UpDownSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}