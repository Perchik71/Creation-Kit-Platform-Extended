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
#include "..\UIThemeMode.h"
#include "VarCommon.h"

#define RECTWIDTH(r) (r.right - r.left)
#define RECTHEIGHT(r) (r.bottom - r.top)

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace CustomCaption
			{
				constexpr static LONG TOPEXTENDWIDTH = 27;
				constexpr static LONG BOTTOMEXTENDWIDTH = 20;
				constexpr static LONG LEFTEXTENDWIDTH = 8;
				constexpr static LONG RIGHTEXTENDWIDTH = 8;

				// Paint the title on the custom frame.
				void PaintCustomCaption(HWND hWnd, HDC hdc)
				{
					RECT rcClient;
					GetClientRect(hWnd, &rcClient);

					HTHEME hTheme = OpenThemeData(NULL, L"CompositedWindow::Window");
					if (hTheme)
					{
						HDC hdcPaint = CreateCompatibleDC(hdc);
						if (hdcPaint)
						{
							int cx = RECTWIDTH(rcClient);
							int cy = RECTHEIGHT(rcClient);

							// Define the BITMAPINFO structure used to draw text.
							// Note that biHeight is negative. This is done because
							// DrawThemeTextEx() needs the bitmap to be in top-to-bottom
							// order.
							BITMAPINFO dib = { 0 };
							dib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
							dib.bmiHeader.biWidth = cx;
							dib.bmiHeader.biHeight = -cy;
							dib.bmiHeader.biPlanes = 1;
							dib.bmiHeader.biBitCount = 32;
							dib.bmiHeader.biCompression = BI_RGB;

							HBITMAP hbm = CreateDIBSection(hdc, &dib, DIB_RGB_COLORS, NULL, NULL, 0);
							if (hbm)
							{
								HBITMAP hbmOld = (HBITMAP)SelectObject(hdcPaint, hbm);

								// Setup the theme drawing options.
								DTTOPTS DttOpts = { sizeof(DTTOPTS) };
								DttOpts.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE;
								DttOpts.iGlowSize = 15;

								// Select a font.
								LOGFONTW lgFont;
								HFONT hFontOld = NULL;
								if (SUCCEEDED(GetThemeSysFont(hTheme, TMT_CAPTIONFONT, &lgFont)))
								{
									HFONT hFont = CreateFontIndirectW(&lgFont);
									hFontOld = (HFONT)SelectObject(hdcPaint, hFont);
								}

								// Draw the title.
								RECT rcPaint = rcClient;
								rcPaint.top += 8;
								rcPaint.right -= 125;
								rcPaint.left += 8;
								rcPaint.bottom = 50;
								DrawThemeTextEx(hTheme,
									hdcPaint,
									0, 0,
									L"RENDER WINDOW",
									-1,
									DT_LEFT | DT_WORD_ELLIPSIS,
									&rcPaint,
									&DttOpts);

								// Blit text to the frame.
								BitBlt(hdc, 0, 0, cx, cy, hdcPaint, 0, 0, SRCCOPY);

								SelectObject(hdcPaint, hbmOld);
								if (hFontOld)
								{
									SelectObject(hdcPaint, hFontOld);
								}
								DeleteObject(hbm);
							}
							DeleteDC(hdcPaint);
						}
						CloseThemeData(hTheme);
					}
				}

				// Hit test the frame for resizing and moving.
				LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
				{
					// Get the point coordinates for the hit test.
					POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

					// Get the window rectangle.
					RECT rcWindow;
					GetWindowRect(hWnd, &rcWindow);

					// Get the frame rectangle, adjusted for the style without a caption.
					RECT rcFrame = { 0 };
					AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

					// Determine if the hit test is for resizing. Default middle (1,1).
					USHORT uRow = 1;
					USHORT uCol = 1;
					BOOL fOnResizeBorder = FALSE;

					// Determine if the point is at the top or bottom of the window.
					if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + TOPEXTENDWIDTH)
					{
						fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
						uRow = 0;
					}
					else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - BOTTOMEXTENDWIDTH)
						uRow = 2;

					// Determine if the point is at the left or right of the window.
					if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + LEFTEXTENDWIDTH)
						uCol = 0; // left side
					else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - RIGHTEXTENDWIDTH)
						uCol = 2; // right side

					// Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
					LRESULT hitTests[3][3] =
					{
						{ HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
						{ HTLEFT,       HTNOWHERE,     HTRIGHT },
						{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
					};

					return hitTests[uRow][uCol];
				}

				INT_PTR CALLBACK CustomCaptionDlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData, BOOL& bCallDWP)
				{
					LRESULT lRet = 0;
					HRESULT hr = S_OK;

					bCallDWP = !DwmDefWindowProc(DialogHwnd, Message, wParam, lParam, &lRet);

					// Handle window creation.
					if (Message == WM_CREATE)
					{
						RECT rcClient;
						GetWindowRect(DialogHwnd, &rcClient);

						// Inform application of the frame change.
						SetWindowPos(DialogHwnd,
							NULL,
							rcClient.left, rcClient.top,
							RECTWIDTH(rcClient), RECTHEIGHT(rcClient),
							SWP_FRAMECHANGED);

						bCallDWP = TRUE;
						lRet = 0;
					}
					// Handle window activation.
					else if (Message == WM_ACTIVATE)
					{
						// Extend the frame into the client area.
						MARGINS margins;

						margins.cxLeftWidth = LEFTEXTENDWIDTH;
						margins.cxRightWidth = RIGHTEXTENDWIDTH;
						margins.cyBottomHeight = BOTTOMEXTENDWIDTH;
						margins.cyTopHeight = TOPEXTENDWIDTH;    

						DwmExtendFrameIntoClientArea(DialogHwnd, &margins);

						bCallDWP = TRUE;
						lRet = 0;
					}
					else if (Message == WM_PAINT)
					{
						HDC hdc;
						{
						//	PAINTSTRUCT ps;
							hdc = GetWindowDC(DialogHwnd);// BeginPaint(DialogHwnd, &ps);
							PaintCustomCaption(DialogHwnd, hdc);
							//EndPaint(DialogHwnd, &ps);
						}

						bCallDWP = TRUE;
						lRet = 0;
					}
					// Handle the non-client size message.
					else if ((Message == WM_NCCALCSIZE) && (wParam == TRUE))
					{
						// Calculate new NCCALCSIZE_PARAMS based on custom NCA inset.
						NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

						pncsp->rgrc[0].left = pncsp->rgrc[0].left + LEFTEXTENDWIDTH;
						pncsp->rgrc[0].top = pncsp->rgrc[0].top + TOPEXTENDWIDTH;
						pncsp->rgrc[0].right = pncsp->rgrc[0].right - RIGHTEXTENDWIDTH;
						pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - BOTTOMEXTENDWIDTH;

						lRet = 0;

						// No need to pass the message on to the DefWindowProc.
						bCallDWP = FALSE;
					}
					// Handle hit testing in the NCA if not handled by DwmDefWindowProc.
					else if ((Message == WM_NCHITTEST) && (lRet == 0))
					{
						lRet = HitTestNCA(DialogHwnd, wParam, lParam);

						if (lRet != HTNOWHERE)
							bCallDWP = FALSE;
					}

					return lRet;
				}

				INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					BOOL bCallDWP = TRUE;
					BOOL bDwmEnabled = FALSE;
					LRESULT lRet = 0;
					HRESULT hr = S_OK;

					// Winproc worker for custom frame issues.
					hr = DwmIsCompositionEnabled(&bDwmEnabled);
					if (SUCCEEDED(hr))
						lRet = CustomCaptionDlgProc(DialogHwnd, Message, wParam, lParam, uIdSubclass, dwRefData, bCallDWP);

					// Winproc worker for the rest of the application.
					//if (bCallDWP)
					//	lRet = UITheme::DialogSubclass(DialogHwnd, Message, wParam, lParam, uIdSubclass, dwRefData);

					return lRet;
				}
			}
		}
	}
}