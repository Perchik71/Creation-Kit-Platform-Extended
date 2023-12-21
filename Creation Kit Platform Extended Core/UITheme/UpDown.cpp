//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "UpDown.h"

#include <CommCtrl.h>
#include <Uxtheme.h>
#include <vssym32.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace UpDown
			{
				namespace Render
				{
					VOID FIXAPI DrawUpArrow_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc_temp(*pRect);
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

					VOID FIXAPI DrawDownArrow_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc_temp(*pRect);
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

					VOID FIXAPI DrawLeftArrow_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc_temp(*pRect);
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

					VOID FIXAPI DrawRightArrow_Stylesheet(Graphics::CUICanvas & canvas, LPCRECT pRect, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc_temp(*pRect);
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

					VOID FIXAPI DrawUp_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc = *pRect;

						canvas.Pen.Color = cColorBorder;
						canvas.MoveTo(rc.Left, rc.Top);
						canvas.LineTo(rc.Left, rc.Bottom);

						rc.Left++;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorEnd, cColorStart, Graphics::gdVert);

						DrawUpArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					VOID FIXAPI DrawUp_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End), 
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawUp_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawUp_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawUp_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawUp_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled));
					}

					VOID FIXAPI DrawDown_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor)
					{
						Graphics::CRECT rc = *pRect;

						canvas.Pen.Color = cColorBorder;
						canvas.MoveTo(rc.Left, rc.Top);
						canvas.LineTo(rc.Left, rc.Bottom);

						rc.Left++;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorStart, cColorEnd, Graphics::gdVert);

						DrawDownArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					VOID FIXAPI DrawDown_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawDown_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawDown_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow));
					}

					VOID FIXAPI DrawDown_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawDown_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled));
					}

					VOID FIXAPI DrawLeft_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, COLORREF cHihglighter, BOOL drawHihglighter)
					{
						Graphics::CRECT rc = *pRect;

						canvas.Frame(rc, cColorBorder);
						rc.Inflate(0, -1);
						rc.Left++;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorStart, cColorEnd, Graphics::gdVert);

						if (drawHihglighter)
						{
							canvas.Pen.Style = Graphics::psSolid;
							canvas.Pen.Color = cHihglighter;
							canvas.MoveTo(rc.Left, rc.Top);
							canvas.LineTo(rc.Right, rc.Top);
						}

						DrawLeftArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					VOID FIXAPI DrawLeft_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							TRUE);
					}

					VOID FIXAPI DrawLeft_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot),
							TRUE);
					}

					VOID FIXAPI DrawLeft_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							0,
							FALSE);
					}

					VOID FIXAPI DrawLeft_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawLeft_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow_Disabled),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Ver2),
							TRUE);
					}

					VOID FIXAPI DrawRight_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, COLORREF cHihglighter, BOOL drawHihglighter)
					{
						Graphics::CRECT rc = *pRect;

						canvas.Frame(rc, cColorBorder);
						rc.Inflate(0, -1);
						rc.Width--;

						if (cColorStart == cColorEnd)
							canvas.Fill(rc, cColorStart);
						else
							canvas.GradientFill(rc, cColorStart, cColorEnd, Graphics::gdVert);

						if (drawHihglighter)
						{
							canvas.Pen.Style = Graphics::psSolid;
							canvas.Pen.Color = cHihglighter;
							canvas.MoveTo(rc.Left, rc.Top);
							canvas.LineTo(rc.Right, rc.Top);
						}

						DrawRightArrow_Stylesheet(canvas, (LPRECT)&rc, cColor, cShadowColor);
					}

					VOID FIXAPI DrawRight_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							TRUE);
					}

					VOID FIXAPI DrawRight_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Hot),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot),
							TRUE);
					}

					VOID FIXAPI DrawRight_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawRight_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Shape_Shadow),
							0,
							FALSE);
					}

					VOID FIXAPI DrawRight_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
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

				VOID FIXAPI Initialize(HWND hWindow)
				{
					OpenThemeData(hWindow, VSCLASS_SPIN);
					// This function spoils the rendering process
					// SetWindowTheme(hWindow, nullptr, nullptr);
					SetWindowSubclass(hWindow, UpDownSubclass, 0, 0);
				}

				LRESULT CALLBACK UpDownSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
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

						Core::Classes::UI::CRECT rc;
						Core::Classes::UI::CUICanvas canvas = hDC;

						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(-rc.Left, -rc.Top);
				
						if (IsWindowEnabled(hWnd))
							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						else
							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));

						ReleaseDC(hWnd, hDC);
					}

					return result;
				}
			}
		}
	}
}