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
#include "CheckBox.h"

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		namespace CheckBox
		{
			namespace Render
			{
				VOID DrawCheck_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clShadow, COLORREF clColor)
				{
					Graphics::CRECT rc_temp = *pRect;

					POINT ps[6] = {
						{ 3 + rc_temp.Left, 5 + rc_temp.Top },
						{ 5 + rc_temp.Left, 7 + rc_temp.Top },
						{ 10 + rc_temp.Left, 2 + rc_temp.Top },
						{ 11 + rc_temp.Left, 3 + rc_temp.Top },
						{ 5 + rc_temp.Left, 9 + rc_temp.Top },
						{ 2 + rc_temp.Left, 6 + rc_temp.Top }
					};

					POINT p[6] = {
						{ 3 + rc_temp.Left, 6 + rc_temp.Top },
						{ 5 + rc_temp.Left, 8 + rc_temp.Top },
						{ 10 + rc_temp.Left, 3 + rc_temp.Top },
						{ 11 + rc_temp.Left, 4 + rc_temp.Top },
						{ 5 + rc_temp.Left, 10 + rc_temp.Top },
						{ 2 + rc_temp.Left, 7 + rc_temp.Top }
					};

					if (GetTheme() != Theme::Theme_NightBlue)
					{
						canvas.Pen.Color = clShadow;
						canvas.Brush.Color = clShadow;
						canvas.Polygon(ps, 6);
					}

					canvas.Pen.Color = clColor;
					canvas.Brush.Color = clColor;
					canvas.Polygon(p, 6);
				}

				VOID DrawCheck_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
						GetThemeSysColor(ThemeColor::ThemeColor_Shape));
				}

				VOID DrawCheck_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
						GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot));
				}

				VOID DrawCheck_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
						GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed));
				}

				VOID DrawCheck_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled),
						GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled));
				}

				VOID DrawIdeterminate_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clColor)
				{
					Graphics::CRECT rc_temp[2];

					rc_temp[0] = *pRect;
					rc_temp[0].Inflate(-3, -3);
					rc_temp[1] = rc_temp[0];
					rc_temp[1].Inflate(-1, -1);

					canvas.GradientFill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_CheckBox_Gradient_Start),
						GetThemeSysColor(ThemeColor::ThemeColor_CheckBox_Gradient_End), Graphics::gdVert);
					canvas.Fill(rc_temp[1], clColor);
				}

				VOID DrawIdeterminate_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape));
				}

				VOID DrawIdeterminate_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot));
				}

				VOID DrawIdeterminate_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed));
				}

				VOID DrawIdeterminate_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
				{
					DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled));
				}
			}

			namespace Event
			{
				VOID OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId)
				{
					flags |= DT_END_ELLIPSIS;
					canvas.ColorText = GetThemeSysColor(ThemeColor_Text_3);
				}
			}
		}
	}
}