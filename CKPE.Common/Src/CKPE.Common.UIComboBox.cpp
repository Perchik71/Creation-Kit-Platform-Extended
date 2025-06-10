// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIComboBox.h>
#include <vssym32.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ComboBox
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawArrow_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor cColor,
						TColor cShadowColor)
					{
						CRECT rc_temp(*pRect);
						rc_temp.Inflate(-((rc_temp.Width - 4) >> 1), -((rc_temp.Height - 4) >> 1));

						std::vector<POINT> p[2];

						rc_temp.Width = (rc_temp.Width & 1) ? rc_temp.Width - 1 : rc_temp.Width;

						p[0].resize(3);
						p[0][0] = { rc_temp.Left, rc_temp.Top };
						p[0][1] = { rc_temp.Right, rc_temp.Top };
						p[0][2] = { rc_temp.Left + (rc_temp.Width >> 1), rc_temp.Top + (rc_temp.Height >> 1) };
						p[1] = p[0];
						p[1][0].y++;
						p[1][1].y++;
						p[1][2].y++;

						canvas.Pen.Color = cShadowColor;
						canvas.Brush.Color = cShadowColor;
						canvas.Polygon(p[0]);
						canvas.Pen.Color = cColor;
						canvas.Brush.Color = cColor;
						canvas.Polygon(p[1]);
					}

					CKPE_COMMON_API void DrawArrow_Normal(Canvas& canvas, LPCRECT pRect)
					{
						DrawArrow_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawArrow_Disabled(Canvas& canvas, LPCRECT pRect)
					{
						DrawArrow_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled));
					}
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags, std::int32_t iStateId)
					{
						flags |= DT_END_ELLIPSIS;
						if (iStateId == CBB_DISABLED)
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_1);
						else
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}