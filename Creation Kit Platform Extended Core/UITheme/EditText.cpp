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

#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "EditText.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace EditText
			{
				namespace Render
				{
					VOID FIXAPI DrawEditText_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clBorderColor,
						COLORREF clColor, COLORREF clColorDividerStart, COLORREF clColorDividerEnd)
					{
						Core::Classes::UI::CRECT rc_temp(*pRect);
						rc_temp.Inflate(-1, -1);

						if (clColorDividerStart == clColorDividerEnd)
							canvas.Frame(*pRect, clColorDividerStart);
						else
							canvas.GradientFrame(*pRect, clColorDividerStart, clColorDividerEnd, Core::Classes::UI::gdVert);
						canvas.Fill(rc_temp, clColor);
						canvas.Frame(rc_temp, clBorderColor);
					}

					VOID FIXAPI DrawEditText_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start), 
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End));
					}

					VOID FIXAPI DrawEditText_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_End));
					}
					VOID FIXAPI DrawEditText_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
					}

					VOID FIXAPI DrawEditText_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End));
					}
				}

				INT_PTR FIXAPI OnCtlColorEdit(HDC hDC)
				{
					SetTextColor(hDC, GetThemeSysColor(ThemeColor_Text_4));
					SetBkColor(hDC, GetThemeSysColor(ThemeColor_Edit_Color));
					return reinterpret_cast<INT_PTR>(hThemeEditBrush);
				}
			}
		}
	}
}