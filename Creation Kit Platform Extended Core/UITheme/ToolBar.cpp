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
#include "..\MainWindow.h"
#include "VarCommon.h"
#include "ToolBar.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ToolBar
			{
				namespace Render
				{
					VOID FIXAPI DrawBackground(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Default));
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(rc_temp.Left, rc_temp.Top);
						canvas.LineTo(rc_temp.Right, rc_temp.Top);
						canvas.MoveTo(rc_temp.Left, rc_temp.Bottom - 1);
						canvas.LineTo(rc_temp.Right, rc_temp.Bottom - 1);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp.Left, rc_temp.Top + 1);
						canvas.LineTo(rc_temp.Right, rc_temp.Top + 1);
					}

					VOID FIXAPI DrawButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End));
					}

					VOID FIXAPI DrawButton_Checked(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						Core::Classes::UI::CRECT rc_temp_v[2];

						rc_temp_v[0] = rc_temp;
						rc_temp_v[0].Inflate(-1, -1);
						rc_temp_v[1] = rc_temp_v[0];
						rc_temp_v[1].Inflate(-1, -1);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.Fill(rc_temp_v[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp_v[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider);
						canvas.MoveTo(rc_temp_v[1].Left, rc_temp_v[1].Top);
						canvas.LineTo(rc_temp_v[1].Right, rc_temp_v[1].Top);
					}

					VOID FIXAPI DrawButton_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						Core::Classes::UI::CRECT rc_temp_v[2];

						rc_temp_v[0] = rc_temp;
						rc_temp_v[0].Inflate(-1, -1);
						rc_temp_v[1] = rc_temp_v[0];
						rc_temp_v[1].Inflate(-1, -1);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.Fill(rc_temp_v[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp_v[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider);
						canvas.MoveTo(rc_temp_v[1].Left, rc_temp_v[1].Top);
						canvas.LineTo(rc_temp_v[1].Right, rc_temp_v[1].Top);
					}

					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					VOID FIXAPI DrawButton_Icon(Graphics::CUICanvas& canvas, LPCRECT pRect, HIMAGELIST hImageList, INT nIndex)
					{
						Graphics::CRECT rc_temp = *pRect;

						ImageList_Draw(hImageList, nIndex, canvas.Handle, rc_temp.Left + ((rc_temp.Width - 16) >> 1),
							rc_temp.Top + ((rc_temp.Height - 16) >> 1), ILD_NORMAL | ILD_TRANSPARENT);
					}
				}

				VOID FIXAPI Initialize(HWND hWindow)
				{
					SendMessageA(hWindow, CCM_SETVERSION, (WPARAM)6, 0);
					SendMessageA(hWindow, TB_SETSTYLE, 0, TBSTYLE_FLAT | CCS_TOP | TBSTYLE_TOOLTIPS |
						//wrapable style doesn't work with separators
						//use nodivider to remove the two stupid pixel lines on top of the toolbar
						CCS_ADJUSTABLE | CCS_NODIVIDER | TBSTYLE_ALTDRAG); // | TBSTYLE_WRAPABLE);// );
					SendMessageA(hWindow, TB_SETBUTTONSIZE, 0, MAKELPARAM(21, 25));

					TBMETRICS metrics = { 0 };
					metrics.cbSize = sizeof(TBMETRICS);
					metrics.dwMask = TBMF_BUTTONSPACING;
					SendMessageA(hWindow, TB_GETMETRICS, 0, (LPARAM)&metrics);
					// Dialogs are designed for 0 spacing
					metrics.cxButtonSpacing = 0;
					SendMessageA(hWindow, TB_SETMETRICS, 0, (LPARAM)&metrics);

					SendMessageA(hWindow, TB_AUTOSIZE, 0, 0);
				}

				LRESULT FIXAPI OnCustomDraw(HWND hWindow, LPNMTBCUSTOMDRAW lpToolBar)
				{
					Graphics::CUICanvas Canvas(lpToolBar->nmcd.hdc);

					switch (lpToolBar->nmcd.dwDrawStage)
					{
					case CDDS_ITEMPREPAINT:
					{
						HWND hWndTB = lpToolBar->nmcd.hdr.hwndFrom;
						HIMAGELIST hImageList = (HIMAGELIST)SendMessageA(hWndTB, TB_GETIMAGELIST, 0, 0);

						TBBUTTON tbButton = { 0 };
						INT nCount = SendMessageA(hWndTB, TB_BUTTONCOUNT, 0, 0);
						for (INT i = 0; i < nCount; i++)
						{
							SendMessageA(hWndTB, TB_GETBUTTON, (WPARAM)i, (LPARAM)& tbButton);

							if (tbButton.idCommand == lpToolBar->nmcd.dwItemSpec)
								break;
						}

						if ((lpToolBar->nmcd.uItemState & CDIS_CHECKED) == CDIS_CHECKED)
							Render::DrawButton_Checked(Canvas, &lpToolBar->nmcd.rc);
						else if ((lpToolBar->nmcd.uItemState & CDIS_HOT) == CDIS_HOT)
							Render::DrawButton_Hot(Canvas, &lpToolBar->nmcd.rc);
						else if ((lpToolBar->nmcd.uItemState & CDIS_SELECTED) == CDIS_SELECTED)
							Render::DrawButton_Pressed(Canvas, &lpToolBar->nmcd.rc);
						else if (((lpToolBar->nmcd.uItemState & CDIS_DISABLED) == CDIS_DISABLED) ||
							((lpToolBar->nmcd.uItemState & CDIS_GRAYED) == CDIS_GRAYED))
							Render::DrawButton_Disabled(Canvas, &lpToolBar->nmcd.rc);
						else
							Render::DrawButton_Normal(Canvas, &lpToolBar->nmcd.rc);

						Render::DrawButton_Icon(Canvas, &lpToolBar->nmcd.rc, hImageList, tbButton.iBitmap);

						return CDRF_SKIPDEFAULT;
					}
					case CDDS_PREPAINT:
					{
						Core::Classes::UI::CUIToolWindow Toolbar = lpToolBar->nmcd.hdr.hwndFrom;
						auto Rect = Toolbar.ClientRect();
						Render::DrawBackground(Canvas, (LPCRECT)&Rect);
						return CDRF_NOTIFYITEMDRAW;
					}
					default:
						return CDRF_DODEFAULT;
					}
				}
			}
		}
	}
}