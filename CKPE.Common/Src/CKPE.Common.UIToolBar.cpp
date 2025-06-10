// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIToolBar.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ToolBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBackground(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;

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

					CKPE_COMMON_API void DrawButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End));
					}

					CKPE_COMMON_API void DrawButton_Checked(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						CRECT rc_temp_v[2];

						rc_temp_v[0] = rc_temp;
						rc_temp_v[0].Inflate(-1, -1);
						rc_temp_v[1] = rc_temp_v[0];
						rc_temp_v[1].Inflate(-1, -1);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.Fill(rc_temp_v[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp_v[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider);
						canvas.MoveTo(rc_temp_v[1].Left, rc_temp_v[1].Top);
						canvas.LineTo(rc_temp_v[1].Right, rc_temp_v[1].Top);
					}

					CKPE_COMMON_API void DrawButton_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						CRECT rc_temp_v[2];

						rc_temp_v[0] = rc_temp;
						rc_temp_v[0].Inflate(-1, -1);
						rc_temp_v[1] = rc_temp_v[0];
						rc_temp_v[1].Inflate(-1, -1);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.Fill(rc_temp_v[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp_v[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider);
						canvas.MoveTo(rc_temp_v[1].Left, rc_temp_v[1].Top);
						canvas.LineTo(rc_temp_v[1].Right, rc_temp_v[1].Top);
					}

					CKPE_COMMON_API void DrawButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;
						rc_temp.Inflate(0, -2);

						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawButton_Icon(Canvas& canvas, LPCRECT pRect, HIMAGELIST hImageList, 
						std::int32_t nIndex) noexcept(true)
					{
						CRECT rc_temp = *pRect;

						ImageList_Draw(hImageList, nIndex, (HDC)canvas.Handle, rc_temp.Left + ((rc_temp.Width - 16) >> 1),
							rc_temp.Top + ((rc_temp.Height - 16) >> 1), ILD_NORMAL | ILD_TRANSPARENT);
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true)
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

				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMTBCUSTOMDRAW lpToolBar) noexcept(true)
				{
					Canvas Canvas(lpToolBar->nmcd.hdc);

					switch (lpToolBar->nmcd.dwDrawStage)
					{
					case CDDS_ITEMPREPAINT:
					{
						HWND hWndTB = lpToolBar->nmcd.hdr.hwndFrom;
						HIMAGELIST hImageList = (HIMAGELIST)SendMessageA(hWndTB, TB_GETIMAGELIST, 0, 0);

						TBBUTTON tbButton = { 0 };
						LRESULT nCount = SendMessageA(hWndTB, TB_BUTTONCOUNT, 0, 0);
						for (LRESULT i = 0; i < nCount; i++)
						{
							SendMessageA(hWndTB, TB_GETBUTTON, (WPARAM)i, (LPARAM)&tbButton);

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
						CUIToolWindow Toolbar = lpToolBar->nmcd.hdr.hwndFrom;
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