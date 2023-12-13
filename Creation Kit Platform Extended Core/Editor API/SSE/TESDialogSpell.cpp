//////////////////////////////////////////
/*
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
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

#include "TESDialogSpell.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			DLGPROC OldSpellDlgProc;
			static HWND hSpellDialog;
			static HFONT hSpellFont;

			HWND GetWindow(VOID) 
			{
				return hSpellDialog;
			}

			INT_PTR CALLBACK SpellDlgProc(HWND hDialogHwnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
				if (uMessage == WM_INITDIALOG) {
					hSpellDialog = hDialogHwnd;
					auto result = OldSpellDlgProc(hDialogHwnd, uMessage, wParam, lParam);

					HWND hCtrlWnd = GetDlgItem(hDialogHwnd, 2069);
					if (hCtrlWnd) {
						HDC hDC = GetDC(hDialogHwnd);
						auto nHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);

						hSpellFont = CreateFontA(
							nHeight,
							0,
							0,
							0,
							FW_NORMAL,
							FALSE,
							FALSE,
							FALSE,
							DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							CLEARTYPE_NATURAL_QUALITY,
							VARIABLE_PITCH,
							"Microsoft Sans Serif");

						if (hSpellFont)
							SendMessageA(hCtrlWnd, WM_SETFONT, (WPARAM)hSpellFont, TRUE);

						ReleaseDC(hDialogHwnd, hDC);
					}

					return result;
				}
				else if (uMessage == WM_DESTROY) {
					hSpellDialog = NULL;
					DeleteObject(hSpellFont);
				}

				return OldSpellDlgProc(hDialogHwnd, uMessage, wParam, lParam);
			}
		}
	}
}