// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UITrackBar.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace TrackBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawTrack(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;

						rc_temp.Offset(0, 1);
						rc_temp.Inflate(1, 0);
						canvas.Frame(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End));
						rc_temp = *pRect;
						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						rc_temp.Inflate(-1, -1);
						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Ver2));
					}

					CKPE_COMMON_API void DrawSlider_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					CKPE_COMMON_API void DrawSlider_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					CKPE_COMMON_API void DrawSlider_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
						canvas.Frame(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawSlider_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
						canvas.Frame(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawSlider_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}
				}
			}
		}
	}
}