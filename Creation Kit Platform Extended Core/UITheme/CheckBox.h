//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once

#include "Editor API/UI/UIGraphics.h"

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		namespace CheckBox
		{
			namespace Graphics = ::Core::Classes::UI;

			namespace Render
			{
				VOID DrawCheck_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clShadow, COLORREF clColor);
				VOID DrawCheck_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawCheck_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawCheck_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawCheck_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);

				VOID DrawIdeterminate_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clColor);
				VOID DrawIdeterminate_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawIdeterminate_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawIdeterminate_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
				VOID DrawIdeterminate_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
			}

			namespace Event
			{
				VOID OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId);
			}
		}
	}
}