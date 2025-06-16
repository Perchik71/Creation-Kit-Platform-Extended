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
			namespace ComboBox
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawArrow_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor cColor,
						TColor cShadowColor);
					CKPE_COMMON_API void DrawArrow_Normal(Canvas& canvas, LPCRECT pRect);
					CKPE_COMMON_API void DrawArrow_Disabled(Canvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags, std::int32_t iStateId);
				}
			}
		}
	}
}