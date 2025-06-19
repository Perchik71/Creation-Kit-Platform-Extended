// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API AboutWindow
		{
		public:
			static INT_PTR CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
			static INT_PTR CALLBACK WndProc2(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		};
	}
}