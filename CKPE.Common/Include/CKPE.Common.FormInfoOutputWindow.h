// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.UIBaseWindow.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API FormInfoOutputWindow : public Common::UI::CUICustomWindow
		{
		public:
			FormInfoOutputWindow();

			INT_PTR OpenModal(const HWND hParentWindow);

			static INT_PTR CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		};
	}
}