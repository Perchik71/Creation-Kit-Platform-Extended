// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.UIBaseWindow.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace TimeOfDay
			{
				typedef struct UITimeOfDayComponentsTag
				{
					HWND hMainWnd;
					CUIToolWindow hWndToolBar;
					CUIBaseControl hWndLabel, hWndTrackBar, hWndEdit;
				} UITimeOfDayComponents, *LPUITimeOfDayComponents, *PUITimeOfDayComponents;

				CKPE_COMMON_API HWND Initialization(HWND hWnd, HWND hMainWnd) noexcept(true);

				CKPE_COMMON_API UITimeOfDayComponents* GetOldUITimeOfDayComponents() noexcept(true);
				CKPE_COMMON_API UITimeOfDayComponents* GetNewUITimeOfDayComponents() noexcept(true);

				CKPE_COMMON_API LRESULT TimeOfDayClassWndProc(HWND hWnd, UINT uMsg, 
					WPARAM wParam, LPARAM lParam) noexcept(true);
			}
		}
	}
}