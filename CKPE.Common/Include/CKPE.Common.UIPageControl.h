// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <CKPE.Graphics.h>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace PageControl
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBorder(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColor) noexcept(true);
					CKPE_COMMON_API void DrawButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_SelectedAndFocused(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags) noexcept(true);
				}

				CKPE_COMMON_API void Initialize(HWND hWnd) noexcept(true);
			}
		}
	}
}