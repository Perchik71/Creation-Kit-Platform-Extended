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
			namespace CustomBox
			{
				enum AllowBox : std::uint32_t
				{
					abNormal = 0,
					abColor,
				};

				namespace Render
				{
					CKPE_COMMON_API void DrawBorder(Canvas& canvas, CRECT& rc);
				}

				CKPE_COMMON_API void Initialize(HWND hWindow, AllowBox eAllowBox);

				CKPE_COMMON_API LRESULT CALLBACK NormalBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
				CKPE_COMMON_API LRESULT CALLBACK ColorBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}