// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIHeader.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace Header
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBack_Stylesheet(Canvas& canvas, LPCRECT pRect,
						TColor clGradientColorStart, TColor clGradientColorEnd, TColor cLighter) noexcept(true)
					{
						if (clGradientColorStart == clGradientColorEnd)
							canvas.Fill(*pRect, clGradientColorStart);
						else
							canvas.GradientFill(*pRect, clGradientColorStart, clGradientColorEnd, gdVert);

						canvas.Pen.Style = psSolid;
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(pRect->left, pRect->bottom - 1);
						canvas.LineTo(pRect->right, pRect->bottom - 1);

						canvas.Pen.Color = cLighter;
						canvas.MoveTo(pRect->left, pRect->top);
						canvas.LineTo(pRect->right, pRect->top);
					}

					CKPE_COMMON_API void DrawBack_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawBack_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawBack_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawBack_Selected(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End));
					}

					CKPE_COMMON_API void DrawDiv(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						if (pRect->left == 0)
							return;

						canvas.Pen.Style = psSolid;
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(pRect->left, pRect->top);
						canvas.LineTo(pRect->left, pRect->bottom);
					}
				}
			}
		}
	}
}