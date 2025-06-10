// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIScrollBar.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ScrollBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBackgroundHorz(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Height /= 2;

						canvas.GradientFill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start), gdVert);

						rc_temp[1] = *pRect;
						rc_temp[1].Top = rc_temp[0].Bottom;

						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End), gdVert);

						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(rc_temp[0].Left, rc_temp[0].Top);
						canvas.LineTo(rc_temp[0].Right, rc_temp[0].Top);
					}

					CKPE_COMMON_API void DrawBackgroundVert(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Width /= 2;

						canvas.GradientFill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start), gdHorz);

						rc_temp[1] = *pRect;
						rc_temp[1].Left = rc_temp[0].Right;

						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End), gdHorz);

						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(rc_temp[0].Left, rc_temp[0].Top);
						canvas.LineTo(rc_temp[0].Left, rc_temp[0].Bottom);
					}

					CKPE_COMMON_API void DrawSliderHorz_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					CKPE_COMMON_API void DrawSliderHorz_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter_Hot);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					CKPE_COMMON_API void DrawSliderVert_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_End), gdHorz);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Left, rc_temp[1].Bottom);
					}

					CKPE_COMMON_API void DrawSliderVert_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_End), gdHorz);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter_Hot);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Left, rc_temp[1].Bottom);
					}

					CKPE_COMMON_API void DrawButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.Fill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
						canvas.Fill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}
				}
			}
		}
	}
}