// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			extern DLGPROC OldSpellDlgProc;

			HWND GetWindow(VOID);
			INT_PTR CALLBACK SpellDlgProc(HWND hDialogHwnd, UINT uMessage, 
				WPARAM wParam, LPARAM lParam);
		}
	}
}