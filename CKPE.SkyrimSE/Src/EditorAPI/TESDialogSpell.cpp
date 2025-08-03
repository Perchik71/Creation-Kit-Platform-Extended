// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <EditorAPI/TESDialogSpell.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
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