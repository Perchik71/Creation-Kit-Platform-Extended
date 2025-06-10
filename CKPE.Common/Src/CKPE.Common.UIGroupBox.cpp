// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIGroupBox.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace GroupBox
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawGroupBox_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor clColor, 
						TColor clColorShadow) noexcept(true)
					{
						CRECT rc_temp(*pRect);

						rc_temp.Bottom--;
						canvas.Frame(rc_temp, clColor);
						canvas.Pen.Color = clColorShadow;
						canvas.MoveTo(rc_temp.Left + 1, rc_temp.Top + 1);
						canvas.LineTo(rc_temp.Right - 1, rc_temp.Top + 1);
						canvas.MoveTo(rc_temp.Left, rc_temp.Bottom);
						canvas.LineTo(rc_temp.Right, rc_temp.Bottom);
					}

					CKPE_COMMON_API void DrawGroupBox_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawGroupBox_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Ver2),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Ver2));
					}

					CKPE_COMMON_API void DrawGroupBox_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawGroupBox_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled_Ver2),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Ver2));
					}
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, DWORD& flags) noexcept(true)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}