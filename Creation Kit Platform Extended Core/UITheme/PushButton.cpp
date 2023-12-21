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
#include "PushButton.h"
#include <vssym32.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PushButton
			{
				namespace Render
				{
					VOID FIXAPI DrawPushButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clGradientColorStart, COLORREF clGradientColorEnd,
						COLORREF clGradientHighlighterColorStart, COLORREF clGradientHighlighterColorEnd,
						COLORREF clDividerColor, COLORREF clDividerHighlighterColor)
					{
						Core::Classes::UI::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Inflate(-1, -1);
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						if (clGradientHighlighterColorStart == clGradientHighlighterColorEnd)
							canvas.Fill(*pRect, clGradientHighlighterColorStart);
						else
							canvas.GradientFill(*pRect, clGradientHighlighterColorStart, clGradientHighlighterColorEnd, Core::Classes::UI::gdVert);
						canvas.Fill(rc_temp[0], clDividerHighlighterColor);
						canvas.GradientFill(rc_temp[1], clGradientColorStart, clGradientColorEnd, Core::Classes::UI::gdVert);
						canvas.Pen.Color = clDividerColor;
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					VOID FIXAPI DrawPushButton_Stylesheet_Flat(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clColorBody, COLORREF clColorDivider)
					{
						Core::Classes::UI::CRECT rc_temp[2];
						rc_temp[0] = *pRect;
						
						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Default));

						canvas.Pen.Style = Core::Classes::UI::psSolid;
						canvas.Pen.Color = clColorDivider;
						canvas.Brush.Color = clColorBody;

						canvas.RoundRect(rc_temp[0], 2, 2);
					}

					VOID FIXAPI DrawPushButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						if (GetTheme() == Theme::Theme_NightBlue)
							DrawPushButton_Stylesheet_Flat(canvas, pRect,
								GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Ver2));
						else
							DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawPushButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						if (GetTheme() == Theme::Theme_NightBlue)
							DrawPushButton_Stylesheet_Flat(canvas, pRect,
								GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
								GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start));
						else
							DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawPushButton_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						if (GetTheme() == Theme::Theme_NightBlue)
							DrawPushButton_Stylesheet_Flat(canvas, pRect,
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						else
							DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed), GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawPushButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						if (GetTheme() == Theme::Theme_NightBlue)
							DrawPushButton_Stylesheet_Flat(canvas, pRect,
								GetThemeSysColor(ThemeColor::ThemeColor_Default),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Ver2));
						else
							DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Button_Light_Disabled_Divider),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId)
					{
						flags |= DT_END_ELLIPSIS;
						if (iStateId == PBS_DISABLED)
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_1);
						else
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}