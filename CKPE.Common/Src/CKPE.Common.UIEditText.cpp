// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIEditText.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace EditText
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawEditText_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor clBorderColor,
						TColor clColor, TColor clColorDividerStart, TColor clColorDividerEnd) noexcept(true)
					{
						CRECT rc_temp(*pRect);
						rc_temp.Inflate(-1, -1);

						if (clColorDividerStart == clColorDividerEnd)
							canvas.Frame(*pRect, clColorDividerStart);
						else
							canvas.Frame(*pRect, clColorDividerEnd);
						canvas.Fill(rc_temp, clColor);
						canvas.Frame(rc_temp, clBorderColor);
					}

					CKPE_COMMON_API void DrawEditText_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End));
					}

					CKPE_COMMON_API void DrawEditText_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_End));
					}
					CKPE_COMMON_API void DrawEditText_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
					}

					CKPE_COMMON_API void DrawEditText_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawEditText_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End));
					}
				}

				CKPE_COMMON_API std::ptrdiff_t OnCtlColorEdit(THandle hDC) noexcept(true)
				{
					SetTextColor((HDC)hDC, GetThemeSysColor(ThemeColor_Text_4));
					SetBkColor((HDC)hDC, GetThemeSysColor(ThemeColor_Edit_Color));
					return reinterpret_cast<std::ptrdiff_t>(ThemeData::GetSingleton()->hThemeEditBrush);
				}
			}
		}
	}
}