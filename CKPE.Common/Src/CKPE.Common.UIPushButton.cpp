// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIPushButton.h>
#include <vssym32.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace PushButton
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawPushButton_Stylesheet(Canvas& canvas, LPCRECT pRect,
						TColor clGradientColorStart, TColor clGradientColorEnd,
						TColor clGradientHighlighterColorStart, TColor clGradientHighlighterColorEnd,
						TColor clDividerColor, TColor clDividerHighlighterColor) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Inflate(-1, -1);
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						if (clGradientHighlighterColorStart == clGradientHighlighterColorEnd)
							canvas.Fill(*pRect, clGradientHighlighterColorStart);
						else
							canvas.GradientFill(*pRect, clGradientHighlighterColorStart, clGradientHighlighterColorEnd,
								gdVert);
						canvas.Fill(rc_temp[0], clDividerHighlighterColor);
						canvas.GradientFill(rc_temp[1], clGradientColorStart, clGradientColorEnd, gdVert);
						canvas.Pen.Color = clDividerColor;
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					CKPE_COMMON_API void DrawPushButton_Stylesheet_Flat(Canvas& canvas, LPCRECT pRect,
						TColor clColorBody, TColor clColorDivider) noexcept(true)
					{
						CRECT rc_temp[2];
						rc_temp[0] = *pRect;

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Default));

						canvas.Pen.Style = psSolid;
						canvas.Pen.Color = clColorDivider;
						canvas.Brush.Color = clColorBody;

						canvas.RoundRect(rc_temp[0], 2, 2);
					}

					CKPE_COMMON_API void DrawPushButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
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

					CKPE_COMMON_API void DrawPushButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
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

					CKPE_COMMON_API void DrawPushButton_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
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

					CKPE_COMMON_API void DrawPushButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
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
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, DWORD& flags, INT iStateId) noexcept(true)
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