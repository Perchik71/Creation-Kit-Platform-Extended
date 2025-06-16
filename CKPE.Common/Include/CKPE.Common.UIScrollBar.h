// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Graphics.h>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ScrollBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBackgroundHorz(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawBackgroundVert(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSliderHorz_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSliderHorz_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSliderVert_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSliderVert_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}
			}
		}
	}
}