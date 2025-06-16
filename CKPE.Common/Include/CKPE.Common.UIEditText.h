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
			namespace EditText
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawEditText_Stylesheet(Canvas& canvas, LPCRECT pRect, TColor clBorderColor,
						TColor clColor, TColor clColorDividerStart, TColor clColorDividerEnd) noexcept(true);

					CKPE_COMMON_API void DrawEditText_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawEditText_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawEditText_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawEditText_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}

				CKPE_COMMON_API std::ptrdiff_t OnCtlColorEdit(THandle hDC) noexcept(true);
			}
		}
	}
}