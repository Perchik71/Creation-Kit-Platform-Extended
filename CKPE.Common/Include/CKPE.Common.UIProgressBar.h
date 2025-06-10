// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <CKPE.Graphics.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ProgressBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBar(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawFill(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT CALLBACK ProgressBarSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, 
					LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true);
			}
		}
	}
}