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
			namespace Header
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBack_Stylesheet(Canvas& canvas, LPCRECT pRect,
						TColor clGradientColorStart, TColor clGradientColorEnd, TColor cLighter) noexcept(true);
					CKPE_COMMON_API void DrawBack_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawBack_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawBack_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawBack_Selected(Canvas& canvas, LPCRECT pRect) noexcept(true);

					CKPE_COMMON_API void DrawDiv(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}
			}
		}
	}
}