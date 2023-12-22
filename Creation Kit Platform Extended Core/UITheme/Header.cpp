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

#include "Editor API/UI/UIBaseWindow.h"
#include "VarCommon.h"
#include "Header.h"

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		namespace Header
		{
			namespace Render
			{
				VOID DrawBack_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
					COLORREF clGradientColorStart, COLORREF clGradientColorEnd, COLORREF cLighter)
				{
					if (clGradientColorStart == clGradientColorEnd)
						canvas.Fill(*pRect, clGradientColorStart);
					else
						canvas.GradientFill(*pRect, clGradientColorStart, clGradientColorEnd, Graphics::gdVert);

					canvas.Pen.Style = Graphics::psSolid;
					canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
					canvas.MoveTo(pRect->left, pRect->bottom - 1);
					canvas.LineTo(pRect->right, pRect->bottom - 1);

					canvas.Pen.Color = cLighter;
					canvas.MoveTo(pRect->left, pRect->top);
					canvas.LineTo(pRect->right, pRect->top);
				}

				VOID DrawBack_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawBack_Stylesheet(canvas, pRect, 
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_Start),
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End),
						GetThemeSysColor(ThemeColor::ThemeColor_Default));
				}

				VOID DrawBack_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawBack_Stylesheet(canvas, pRect,
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Hot_Gradient_Start),
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Hot_Gradient_End),
						GetThemeSysColor(ThemeColor::ThemeColor_Default));
				}

				VOID DrawBack_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawBack_Stylesheet(canvas, pRect,
						GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
						GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
						GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
				}

				VOID DrawBack_Selected(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawBack_Stylesheet(canvas, pRect,
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End),
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_Start),
						GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End));
				}

				VOID DrawDiv(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					if (pRect->left == 0)
						return;

					canvas.Pen.Style = Graphics::psSolid;
					canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
					canvas.MoveTo(pRect->left, pRect->top);
					canvas.LineTo(pRect->left, pRect->bottom);
				}
			}
		}
	}
}