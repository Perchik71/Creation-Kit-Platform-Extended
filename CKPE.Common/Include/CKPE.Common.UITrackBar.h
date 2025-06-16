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
			namespace TrackBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawTrack(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSlider_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSlider_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSlider_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSlider_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawSlider_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}
			}
		}
	}
}