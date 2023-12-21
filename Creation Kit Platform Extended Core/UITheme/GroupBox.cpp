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
#include "GroupBox.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace GroupBox
			{
				namespace Render
				{
					VOID FIXAPI DrawGroupBox_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clColor, COLORREF clColorShadow)
					{
						Core::Classes::UI::CRECT rc_temp(*pRect);

						rc_temp.Bottom--;
						canvas.Frame(rc_temp, clColor);
						canvas.Pen.Color = clColorShadow;
						canvas.MoveTo(rc_temp.Left + 1, rc_temp.Top + 1);
						canvas.LineTo(rc_temp.Right - 1, rc_temp.Top + 1);
						canvas.MoveTo(rc_temp.Left, rc_temp.Bottom);
						canvas.LineTo(rc_temp.Right, rc_temp.Bottom);
					}

					VOID FIXAPI DrawGroupBox_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawGroupBox_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Ver2), 
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Ver2));
					}

					VOID FIXAPI DrawGroupBox_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawGroupBox_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled_Ver2),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Ver2));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}