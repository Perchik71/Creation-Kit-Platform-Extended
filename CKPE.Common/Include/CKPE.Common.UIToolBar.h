// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Graphics.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ToolBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBackground(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Checked(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Icon(Canvas& canvas, LPCRECT pRect, HIMAGELIST hImageList, 
						std::int32_t nIndex) noexcept(true);
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMTBCUSTOMDRAW lpToolBar) noexcept(true);
			}
		}
	}
}