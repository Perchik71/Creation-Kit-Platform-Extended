// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UICheckBox.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace CheckBox
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawCheck_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor clShadow, 
						TColor clColor) noexcept(true)
					{
						CRECT rc_temp = *pRect;

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

					CKPE_COMMON_API void DrawCheck_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape));
					}

					CKPE_COMMON_API void DrawCheck_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot));
					}

					CKPE_COMMON_API void DrawCheck_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed));
					}

					CKPE_COMMON_API void DrawCheck_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawCheck_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled));
					}

					CKPE_COMMON_API void DrawIdeterminate_Stylesheet(Canvas& canvas, LPCRECT pRect, 
						TColor clColor) noexcept(true)
					{
						CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Inflate(-3, -3);
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.GradientFill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_CheckBox_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_CheckBox_Gradient_End), gdVert);
						canvas.Fill(rc_temp[1], clColor);
					}

					CKPE_COMMON_API void DrawIdeterminate_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape));
					}

					CKPE_COMMON_API void DrawIdeterminate_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot));
					}

					CKPE_COMMON_API void DrawIdeterminate_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed));
					}

					CKPE_COMMON_API void DrawIdeterminate_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawIdeterminate_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled));
					}
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags,
						std::int32_t iStateId) noexcept(true)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Text_3);
					}
				}
			}
		}
	}
}