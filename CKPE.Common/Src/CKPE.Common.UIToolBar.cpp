// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.GDIPlus.h>
#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIToolBar.h>

#include <format>

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

					CKPE_COMMON_API void DrawButton_Normal(THandle hRenderer) noexcept(true)
					{
						auto color = GetThemeSysColor(ThemeColor::ThemeColor_Default);
						GDIPlusRenderer::SetClearColor(hRenderer, GET_R_VALUE(color), GET_G_VALUE(color), GET_B_VALUE(color), 255);
						GDIPlusRenderer::Clear(hRenderer);
					}

					CKPE_COMMON_API void DrawButton_Hot(THandle hRenderer, LPCRECT pRect) noexcept(true)
					{
						auto color = GetThemeSysColor(ThemeColor::ThemeColor_Default);
						GDIPlusRenderer::SetClearColor(hRenderer, GET_R_VALUE(color), GET_G_VALUE(color), GET_B_VALUE(color), 255);
						GDIPlusRenderer::Clear(hRenderer);

						auto colorBodyStart = GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start);
						auto colorBodyEnd = GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End);
						auto colorHighlighterStart = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start);
						auto colorHighlighterEnd = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End);
						auto alpha = static_cast<std::uint8_t>(160);

						auto w = pRect->right - pRect->left;
						auto h = pRect->bottom - pRect->top;
						auto r = w > 24 ? 4 : 2;
						auto sw = w > 24 ? 2 : 1;
						auto sh = h > 24 ? 2 : 1;

						RECT rc{ sw, sh, w - (sw << 1), h - (sh << 1) };

						GDIPlusRendererHelpers::DrawRoundedSquare(hRenderer, (LPRECT)std::addressof(rc), r,
							colorBodyStart, colorBodyEnd,				// тело: верх/низ градиента
							{ 0, 0, 0, alpha },							// тёмное кольцо внутри
							color,										// цвет фона — тот же, что в Clear!
							colorHighlighterEnd,						// внешняя подсветка сверху (светлее фона)
							GDIPlusColor(colorHighlighterStart) / 2,	// внешняя подсветка снизу (темнее фона)
							1, 1);
					}

					CKPE_COMMON_API void DrawButton_Checked(THandle hRenderer, LPCRECT pRect, bool bDisabled) noexcept(true)
					{
						auto color = GetThemeSysColor(ThemeColor::ThemeColor_Default);
						GDIPlusRenderer::SetClearColor(hRenderer, GET_R_VALUE(color), GET_G_VALUE(color), GET_B_VALUE(color), 255);
						GDIPlusRenderer::Clear(hRenderer);

						auto colorBodyStart = GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start);
						auto colorBodyEnd = GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End);
						auto colorHighlighterStart = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start);
						auto colorHighlighterEnd = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End);
						auto alpha = static_cast<std::uint8_t>(160);

						if (bDisabled)
						{
							colorBodyStart = GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start);
							colorBodyEnd = GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End);
							colorHighlighterStart = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start);
							colorHighlighterEnd = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End);
							alpha = static_cast<std::uint8_t>(120);
						}

						auto w = pRect->right - pRect->left;
						auto h = pRect->bottom - pRect->top;
						auto r = w > 24 ? 4 : 2;
						auto sw = w > 24 ? 2 : 1;
						auto sh = h > 24 ? 2 : 1;

						RECT rc{ sw, sh, w - (sw << 1), h - (sh << 1) };

						GDIPlusRendererHelpers::DrawRoundedSquare(hRenderer, (LPRECT)std::addressof(rc), r,
							colorBodyStart, colorBodyEnd,				// тело: верх/низ градиента
							{ 0, 0, 0, alpha },							// тёмное кольцо внутри
							color,										// цвет фона — тот же, что в Clear!
							GDIPlusColor(colorHighlighterStart) / 2,	// внешняя подсветка сверху (темнее фона)
							colorHighlighterEnd,						// внешняя подсветка снизу (светлее фона)
							1, 1);
					}

					CKPE_COMMON_API void DrawButton_Pressed(THandle hRenderer, LPCRECT pRect) noexcept(true)
					{
						DrawButton_Checked(hRenderer, pRect, true);
					}

					CKPE_COMMON_API void DrawButton_Icon(THandle hRenderer, LPCRECT pRect, HIMAGELIST hImageList,
						std::int32_t nIndex, bool bDisabled, std::int32_t nState) noexcept(true)
					{
						CRECT rc_temp = *pRect;

						std::uint32_t additional = 0;
						if (nState == 1)
							additional = ILD_FOCUS;
						else if (nState == 2)
							additional = ILD_SELECTED;

						if (bDisabled)
							additional |= ILD_BLEND25;

						IMAGEINFO imageListInfo;
						if (!ImageList_GetImageInfo(hImageList, nIndex, std::addressof(imageListInfo)))
							return;

						BITMAP bmInfo;
						if (!GetObjectA(imageListInfo.hbmImage, sizeof(BITMAP), (LPSTR)std::addressof(bmInfo)))
							return;

						std::int32_t w, h;
						if (!ImageList_GetIconSize(hImageList, std::addressof(w), std::addressof(h)))
							return;

						if (bmInfo.bmBitsPixel == 32)
						{
							auto hIcon = ImageList_GetIcon(hImageList, nIndex, ILD_TRANSPARENT);
							if (!hIcon)
								return;

							ICONINFO iconInfo = { 0 };
							if (!GetIconInfo(hIcon, std::addressof(iconInfo)))
								return;

							GDIPlusRenderer::DrawImage(hRenderer, iconInfo.hbmColor, ((rc_temp.Width - w) >> 1),
								((rc_temp.Height - h) >> 1), w, h, bDisabled ? .5f : 1.f);

							if (iconInfo.hbmColor)
							{
								DeleteObject(iconInfo.hbmColor);
								iconInfo.hbmColor = nullptr;
							}

							if (iconInfo.hbmMask)
							{
								DeleteObject(iconInfo.hbmMask);
								iconInfo.hbmMask = nullptr;
							}

							DestroyIcon(hIcon);
						}
						else
							ImageList_DrawEx(hImageList, nIndex, (HDC)GDIPlusRenderer::GetCompatibleDC(hRenderer), ((rc_temp.Width - w) >> 1),
								((rc_temp.Height - h) >> 1), w, h, CLR_NONE, CLR_DEFAULT, ILD_TRANSPARENT | additional);
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true)
				{
					SendMessageA(hWindow, CCM_SETVERSION, (WPARAM)6, 0);

					auto style = SendMessageA(hWindow, TB_GETSTYLE, 0, 0);
					std::uint32_t place = CCS_TOP;
					if ((style & CCS_VERT) == CCS_VERT)
					{
						if ((style & CCS_BOTTOM) == CCS_TOP)
							place = CCS_LEFT;
						else
							place = CCS_RIGHT;
					}
					else
					{
						if ((style & CCS_BOTTOM) == CCS_BOTTOM)
							place = CCS_BOTTOM;
					}

					bool noresize = (style & CCS_NORESIZE) == CCS_NORESIZE;
					if (noresize)
						place |= CCS_NORESIZE;
					if ((style & TBSTYLE_WRAPABLE) == TBSTYLE_WRAPABLE)
						place |= TBSTYLE_WRAPABLE;

					SendMessageA(hWindow, TB_SETSTYLE, 0, TBSTYLE_FLAT | place | TBSTYLE_TOOLTIPS |
						//wrapable style doesn't work with separators
						//use nodivider to remove the two stupid pixel lines on top of the toolbar
						CCS_ADJUSTABLE | CCS_NODIVIDER | TBSTYLE_ALTDRAG); // | TBSTYLE_WRAPABLE);// );

					TBMETRICS metrics = { 0 };
					metrics.cbSize = sizeof(TBMETRICS);
					metrics.dwMask = TBMF_BUTTONSPACING;
					SendMessageA(hWindow, TB_GETMETRICS, 0, (LPARAM)&metrics);
					// Dialogs are designed for 0 spacing
					metrics.cxButtonSpacing = 0;
					SendMessageA(hWindow, TB_SETMETRICS, 0, (LPARAM)&metrics);
				}

				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMTBCUSTOMDRAW lpToolBar) noexcept(true)
				{
					Canvas Canvas(lpToolBar->nmcd.hdc);

					switch (lpToolBar->nmcd.dwDrawStage)
					{
					case CDDS_ITEMPREPAINT:
					{
						auto hWndTB = lpToolBar->nmcd.hdr.hwndFrom;
						auto hImageList = (HIMAGELIST)SendMessageA(hWndTB, TB_GETIMAGELIST, 0, 0);

						TBBUTTON tbButton = { 0 };
						LRESULT nCount = SendMessageA(hWndTB, TB_BUTTONCOUNT, 0, 0);
						for (LRESULT i = 0; i < nCount; i++)
						{
							SendMessageA(hWndTB, TB_GETBUTTON, (WPARAM)i, (LPARAM)&tbButton);

							if (tbButton.idCommand == lpToolBar->nmcd.dwItemSpec)
								break;
						}

						bool disabled = ((lpToolBar->nmcd.uItemState & CDIS_DISABLED) == CDIS_DISABLED) ||
							((lpToolBar->nmcd.uItemState & CDIS_GRAYED) == CDIS_GRAYED);
						
						auto style = SendMessageA(hWndTB, TB_GETSTYLE, 0, 0);
						RECT rc = (style & CCS_VERT) == CCS_VERT ? lpToolBar->nmcd.rc : 
							RECT{ lpToolBar->nmcd.rc.left, lpToolBar->nmcd.rc.top + 2, lpToolBar->nmcd.rc.right, lpToolBar->nmcd.rc.bottom - 2 };

						auto renderer = GDIPlusRenderer::DrawBegin(Canvas.Handle, std::addressof(rc));
						if (renderer)
						{
							if ((lpToolBar->nmcd.uItemState & CDIS_CHECKED) == CDIS_CHECKED)
								Render::DrawButton_Checked(renderer, std::addressof(rc), disabled);
							else if ((lpToolBar->nmcd.uItemState & CDIS_SELECTED) == CDIS_SELECTED)
								Render::DrawButton_Pressed(renderer, std::addressof(rc));
							else if ((lpToolBar->nmcd.uItemState & CDIS_HOT) == CDIS_HOT)
								Render::DrawButton_Hot(renderer, std::addressof(rc));
							else
								Render::DrawButton_Normal(renderer);

							Render::DrawButton_Icon(renderer, std::addressof(rc), hImageList, tbButton.iBitmap, disabled);

							GDIPlusRenderer::DrawEnd(renderer);
						}

						return CDRF_SKIPDEFAULT;
					}
					case CDDS_PREPAINT:
					{
						RECT rc;
						auto hWndTB = lpToolBar->nmcd.hdr.hwndFrom;

						// Необходима вся область, так как вызывает множественные "артефакты"
						if (GetClientRect(hWndTB, std::addressof(rc)))
							Render::DrawBackground(Canvas, (LPCRECT)std::addressof(rc));

						return CDRF_NOTIFYITEMDRAW;
					}
					// ЗАПРЕТ НА СТИРАНИЕ!!! Решает проблему фликов
					case CDDS_PREERASE:
					case CDDS_POSTERASE:
						return CDRF_SKIPDEFAULT;
					default:
						return CDRF_DODEFAULT;
					}
				}
			}
		}
	}
}