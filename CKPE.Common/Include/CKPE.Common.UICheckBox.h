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
			namespace CheckBox
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawCheck_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor clShadow, 
						TColor clColor) noexcept(true);
					CKPE_COMMON_API void DrawCheck_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawCheck_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawCheck_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawCheck_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);

					CKPE_COMMON_API void DrawIdeterminate_Stylesheet(Canvas& canvas, LPCRECT pRect, 
						TColor clColor) noexcept(true);
					CKPE_COMMON_API void DrawIdeterminate_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawIdeterminate_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawIdeterminate_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawIdeterminate_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
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