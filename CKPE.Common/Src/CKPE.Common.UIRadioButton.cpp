// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIRadioButton.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace RadioButton
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawPushButtonR_Stylesheet(Canvas& canvas, LPCRECT pRect,
						TColor clGradientColorStart, TColor clGradientColorEnd,
						TColor clGradientHighlighterColorStart, TColor clGradientHighlighterColorEnd,
						TColor clDividerColor, TColor clDividerHighlighterColor) noexcept(true)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor::ThemeColor_Default));
						canvas.Pen.Style = psSolid;
						canvas.Pen.Color = clDividerHighlighterColor;
						CreateGradientBrush(canvas.Brush, clGradientColorStart, clGradientColorEnd,
							pRect->bottom - pRect->top, gdVert);
						canvas.Ellipse(*pRect);
					}

					CKPE_COMMON_API void DrawPushButtonR_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawPushButtonR_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawPushButtonR_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawPushButtonR_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawPushButtonR_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawPushButtonR_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed), GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawPushButtonR_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawPushButtonR_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Button_Light_Disabled_Divider),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
					}

					CKPE_COMMON_API void DrawRadioButton_Stylesheet(Canvas& canvas, LPCRECT pRect, 
						TColor clColor) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Inflate(-2, -2);
						rc_temp[0].Width++;
						rc_temp[0].Height++;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Pen.Style = psClear;
						CreateGradientBrush(canvas.Brush, GetThemeSysColor(ThemeColor::ThemeColor_CheckBox_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_CheckBox_Gradient_End),
							pRect->bottom - pRect->top, gdVert);
						canvas.Ellipse(rc_temp[0]);
						canvas.EllipseFill(rc_temp[1], clColor);
					}

					CKPE_COMMON_API void DrawRadioButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRadioButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape));
					}

					CKPE_COMMON_API void DrawRadioButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRadioButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot));
					}

					CKPE_COMMON_API void DrawRadioButton_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRadioButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed));
					}

					CKPE_COMMON_API void DrawRadioButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRadioButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled));
					}
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, DWORD& flags, std::int32_t iStateId) noexcept(true)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Text_3);
					}
				}
			}
		}
	}
}