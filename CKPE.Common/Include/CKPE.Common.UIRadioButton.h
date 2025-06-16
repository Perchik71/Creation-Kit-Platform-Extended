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
			namespace RadioButton
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawPushButtonR_Stylesheet(Canvas& canvas, LPCRECT pRect,
						TColor clGradientColorStart, TColor clGradientColorEnd,
						TColor clGradientHighlighterColorStart, TColor clGradientHighlighterColorEnd,
						TColor clDividerColor, TColor clDividerHighlighterColor) noexcept(true);
					CKPE_COMMON_API void DrawPushButtonR_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawPushButtonR_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawPushButtonR_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawPushButtonR_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);

					CKPE_COMMON_API void DrawRadioButton_Stylesheet(Canvas& canvas, LPCRECT pRect, 
						TColor clColor) noexcept(true);
					CKPE_COMMON_API void DrawRadioButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRadioButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRadioButton_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRadioButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags,
						std::int32_t iStateId) noexcept(true);
				}
			}
		}
	}
}