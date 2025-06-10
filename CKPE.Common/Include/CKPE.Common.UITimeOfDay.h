// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.UIGraphics.h>
#include "Editor API/UI/UIBaseWindow.h"

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace TimeOfDay
			{
				namespace Graphics = ::Core::Classes::UI;

				typedef struct UITimeOfDayComponentsTag
				{
					Graphics::CUIToolWindow hWndToolBar;
					Graphics::CUIBaseControl hWndLabel, hWndTrackBar, hWndEdit;
				} UITimeOfDayComponents, * LPUITimeOfDayComponents, * PUITimeOfDayComponents;

				HWND Initialization(HWND hWnd);

				extern UITimeOfDayComponents OldUITimeOfDayComponents;
				extern UITimeOfDayComponents NewUITimeOfDayComponents;

				LRESULT CALLBACK TimeOfDayClassWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			}
		}
	}
}