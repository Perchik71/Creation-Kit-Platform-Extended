// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Editor API/UI/UIBaseWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class FormInfoOutputWindow : public ::Core::Classes::UI::CUICustomWindow
		{
		public:
			FormInfoOutputWindow();

			INT_PTR OpenModal(const HWND hParentWindow);

			static INT_PTR CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		};
	}
}