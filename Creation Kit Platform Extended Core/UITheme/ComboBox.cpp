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
#include "ComboBox.h"
#include <Vssym32.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ComboBox
			{
				namespace Render
				{
					VOID FIXAPI DrawArrow_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc_temp(*pRect);
						rc_temp.Inflate(-((rc_temp.Width - 4) >> 1), -((rc_temp.Height - 4) >> 1));

						std::vector<POINT> p[2];

						rc_temp.Width = (rc_temp.Width & 1) ? rc_temp.Width - 1 : rc_temp.Width;

						p[0].resize(3);
						p[0][0] = { rc_temp.Left, rc_temp.Top };
						p[0][1] = { rc_temp.Right, rc_temp.Top };
						p[0][2] = { rc_temp.Left + (rc_temp.Width >> 1), rc_temp.Top + (rc_temp.Height >> 1) };
						p[1] = p[0];
						p[1][0].y++;
						p[1][1].y++;
						p[1][2].y++;

						canvas.Pen.Color = cShadowColor;
						canvas.Brush.Color = cShadowColor;
						canvas.Polygon(p[0]);
						canvas.Pen.Color = cColor;
						canvas.Brush.Color = cColor;
						canvas.Polygon(p[1]);
					}

					VOID FIXAPI DrawArrow_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawArrow_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawArrow_Disabled(Graphics::CUICanvas & canvas, LPCRECT pRect)
					{
						DrawArrow_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId)
					{
						flags |= DT_END_ELLIPSIS;
						if (iStateId == CBB_DISABLED)
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_1);
						else
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}