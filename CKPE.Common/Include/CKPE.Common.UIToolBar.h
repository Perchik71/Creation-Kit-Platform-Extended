// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Graphics.h>
#include <CKPE.Common.Common.h>

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
					CKPE_COMMON_API void DrawButton_Normal(THandle hRenderer) noexcept(true);
					CKPE_COMMON_API void DrawButton_Hot(THandle hRenderer, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Checked(THandle hRenderer, LPCRECT pRect, bool bDisabled) noexcept(true);
					CKPE_COMMON_API void DrawButton_Pressed(THandle hRenderer, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Icon(THandle hRenderer, LPCRECT pRect, HIMAGELIST hImageList,
						std::int32_t nIndex, bool bDisabled = false, std::int32_t nState = 0) noexcept(true);
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMTBCUSTOMDRAW lpToolBar) noexcept(true);
			}
		}
	}
}