// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <vssym32.h>
#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIUpDown.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace UpDown
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawUpArrow_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColor, 
						COLORREF cShadowColor) noexcept(true)
					{
						CRECT rc_temp(*pRect);
						rc_temp.Inflate(-((rc_temp.Width - 4) >> 1), -((rc_temp.Height - 4) >> 1));

						std::vector<POINT> p[2];
						rc_temp.Offset(0, 1);

						p[0].resize(3);
						p[0][0] = { rc_temp.Left, rc_temp.Top + 2 };
						p[0][1] = { rc_temp.Left + 4, rc_temp.Top + 2 };
						p[0][2] = { rc_temp.Left + 2, rc_temp.Top };
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

					CKPE_COMMON_API void DrawDownArrow_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColor, 
						COLORREF cShadowColor) noexcept(true)
					{
						CRECT rc_temp(*pRect);
						rc_temp.Inflate(-((rc_temp.Width - 4) >> 1), -((rc_temp.Height - 4) >> 1));
						rc_temp.Offset(0, -1);

						std::vector<POINT> p[2];

						p[0].resize(3);
						p[0][0] = { rc_temp.Left, rc_temp.Top };
						p[0][1] = { rc_temp.Left + 4, rc_temp.Top };
						p[0][2] = { rc_temp.Left + 2, rc_temp.Top + 2 };
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

					CKPE_COMMON_API void DrawLeftArrow_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColor, 
						COLORREF cShadowColor) noexcept(true)
					{
						CRECT rc_temp(*pRect);
						rc_temp.Inflate(-((rc_temp.Width - 4) >> 1), -((rc_temp.Height - 4) >> 1));
						rc_temp.Offset(-1, -1);

						std::vector<POINT> p[2];

						p[0].resize(3);
						p[0][0] = { rc_temp.Right, rc_temp.Top };
						p[0][1] = { rc_temp.Right - 2, rc_temp.Top + 2 };
						p[0][2] = { rc_temp.Right, rc_temp.Top + 4 };
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

					CKPE_COMMON_API void DrawRightArrow_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColor,
						COLORREF cShadowColor) noexcept(true)
					{
						CRECT rc_temp(*pRect);
						rc_temp.Inflate(-((rc_temp.Width - 4) >> 1), -((rc_temp.Height - 4) >> 1));
						rc_temp.Offset(0, -1);

						std::vector<POINT> p[2];

						p[0].resize(3);
						p[0][0] = { rc_temp.Left, rc_temp.Top };
						p[0][1] = { rc_temp.Left, rc_temp.Top + 4 };
						p[0][2] = { rc_temp.Left + 2, rc_temp.Top + 2 };
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

					CKPE_COMMON_API void DrawUp_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor)  noexcept(true)
					{
						CRECT rc = *pRect;

						rc.Left++;
						rc.Bottom++;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorEnd, cColorStart, gdVert);

						DrawUpArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					CKPE_COMMON_API void DrawUp_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawUp_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawUp_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawUp_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled));
					}

					CKPE_COMMON_API void DrawDown_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor) noexcept(true)
					{
						CRECT rc = *pRect;

						rc.Left++;
						/*canvas.Frame(rc, cColorBorder);
						rc.Right++;

						if ((rc.Width % 2) == 0)
							rc.Inflate(-1, 0);

						if ((rc.Height % 2) == 0)
							rc.Height--;*/

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorStart, cColorEnd, gdVert);

						DrawDownArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					CKPE_COMMON_API void DrawDown_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawDown_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawDown_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					CKPE_COMMON_API void DrawDown_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled));
					}

					CKPE_COMMON_API void DrawLeft_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor,
						COLORREF cHihglighter, bool drawHihglighter) noexcept(true)
					{
						CRECT rc = *pRect;

						canvas.Frame(rc, cColorBorder);
						rc.Inflate(0, -1);
						rc.Left++;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorStart, cColorEnd, gdVert);

						if (drawHihglighter)
						{
							canvas.Pen.Style = psSolid;
							canvas.Pen.Color = cHihglighter;
							canvas.MoveTo(rc.Left, rc.Top);
							canvas.LineTo(rc.Right, rc.Top);
						}

						DrawLeftArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					CKPE_COMMON_API void DrawLeft_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							TRUE);
					}

					CKPE_COMMON_API void DrawLeft_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot),
							TRUE);
					}

					CKPE_COMMON_API void DrawLeft_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							0,
							FALSE);
					}

					CKPE_COMMON_API void DrawLeft_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Ver2),
							TRUE);
					}

					CKPE_COMMON_API void DrawRight_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, COLORREF cHihglighter, 
						bool drawHihglighter) noexcept(true)
					{
						CRECT rc = *pRect;

						canvas.Frame(rc, cColorBorder);
						rc.Inflate(0, -1);
						rc.Width--;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorStart, cColorEnd, gdVert);

						if (drawHihglighter)
						{
							canvas.Pen.Style = psSolid;
							canvas.Pen.Color = cHihglighter;
							canvas.MoveTo(rc.Left, rc.Top);
							canvas.LineTo(rc.Right, rc.Top);
						}

						DrawRightArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					CKPE_COMMON_API void DrawRight_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							TRUE);
					}

					CKPE_COMMON_API void DrawRight_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot),
							TRUE);
					}

					CKPE_COMMON_API void DrawRight_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							0,
							FALSE);
					}

					CKPE_COMMON_API void DrawRight_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Ver2),
							TRUE);
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true)
				{
					OpenThemeData(hWindow, VSCLASS_SPIN);
					// This function spoils the rendering process
					// SetWindowTheme(hWindow, nullptr, nullptr);
					SetWindowSubclass(hWindow, UpDownSubclass, 0, 0);
				}

				CKPE_COMMON_API LRESULT CALLBACK UpDownSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
				{
					LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

					if ((uMsg == WM_ENABLE) || (uMsg == WM_SETFOCUS))
					{
						InvalidateRect(hWnd, NULL, TRUE);
						UpdateWindow(hWnd);
					}
					else if (uMsg == WM_PAINT)
					{
						// redraw border
						HDC hDC = GetWindowDC(hWnd);

						CRECT rc;
						Canvas canvas = hDC;

						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(-rc.Left, -rc.Top);

						if (IsWindowEnabled(hWnd))
							canvas.Frame(rc, GetThemeSysColor(ThemeColor_Divider_Color));
						else
							canvas.Frame(rc, GetThemeSysColor(ThemeColor_Divider_Color_Disabled));

						ReleaseDC(hWnd, hDC);
					}

					return result;
				}
			}
		}
	}
}