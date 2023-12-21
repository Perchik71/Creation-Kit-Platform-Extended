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

#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			extern Core::Classes::UI::CUIFont* ThemeFont;

			extern HBRUSH hThemeDefaultBrush;
			extern HBRUSH hThemeText3Brush;
			extern HBRUSH hThemeEditBrush;
			extern HBRUSH hThemeDividerBrush;
			extern HBRUSH hThemeText4Brush;
			extern HBRUSH hThemeBorderWindowBrush;

			enum Theme
			{
				Theme_Light = 1,
				Theme_Gray,
				Theme_DarkGray,
				Theme_Dark,
				Theme_NightBlue
			};

			Theme FIXAPI GetTheme(VOID);
			VOID FIXAPI SetTheme(Theme theme);

			enum ThemeColor
			{
				ThemeColor_Default = 1,
				ThemeColor_ListView_Color,
				ThemeColor_TreeView_Color,
				ThemeColor_Edit_Color,
				ThemeColor_Edit_Color_Disabled,
				ThemeColor_Text_1,
				ThemeColor_Text_2,
				ThemeColor_Text_3,
				ThemeColor_Text_4,
				ThemeColor_MDIWindow,
				ThemeColor_Default_Gradient_Start,
				ThemeColor_Default_Gradient_End,
				ThemeColor_Divider_Highlighter_Disabled_Gradient_Start,
				ThemeColor_Divider_Highlighter_Disabled_Gradient_End,
				ThemeColor_Divider_Color,
				ThemeColor_Divider_Color_Disabled,
				ThemeColor_Divider_Highlighter,
				ThemeColor_Divider_Highlighter_Hot,
				ThemeColor_Divider_Color_Ver2,
				ThemeColor_Divider_Color_Disabled_Ver2,
				ThemeColor_Divider_Highlighter_Ver2,
				ThemeColor_Divider_Highlighter_Disabled_Ver2,
				ThemeColor_Divider_Highlighter_Gradient_Start,
				ThemeColor_Divider_Highlighter_Gradient_End,
				ThemeColor_Divider_Highlighter_Hot_Gradient_Start,
				ThemeColor_Divider_Highlighter_Hot_Gradient_End,
				ThemeColor_Divider_Highlighter_Pressed,
				ThemeColor_Button_Pressed_Gradient_Start,
				ThemeColor_Button_Pressed_Gradient_End,
				ThemeColor_Button_Hot_Gradient_Start,
				ThemeColor_Button_Hot_Gradient_End,
				ThemeColor_Button_Pressed_Divider,
				ThemeColor_Button_Light_Disabled_Divider,
				ThemeColor_Button_Disabled_Gradient_Start,
				ThemeColor_Button_Disabled_Gradient_End,
				ThemeColor_CheckBox_Gradient_Start,
				ThemeColor_CheckBox_Gradient_End,
				ThemeColor_ScrollBar_Gradient_Start,
				ThemeColor_ScrollBar_Gradient_End,
				ThemeColor_ScrollBar_Thumb_Gradient_Start,
				ThemeColor_ScrollBar_Thumb_Gradient_End,
				ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start,
				ThemeColor_ScrollBar_Thumb_Gradient_Hot_End,
				ThemeColor_ScrollBar_Thumb_Highlighter,
				ThemeColor_ScrollBar_Thumb_Highlighter_Hot,
				ThemeColor_Shape,
				ThemeColor_Shape_Hot,
				ThemeColor_Shape_Pressed,
				ThemeColor_Shape_Disabled,
				ThemeColor_Shape_Shadow,
				ThemeColor_Shape_Shadow_Disabled,
				ThemeColor_Progress_Fill_Gradient_Start,
				ThemeColor_Progress_Fill_Gradient_End,
				ThemeColor_Progress_Fill_Highlighter,
				ThemeColor_Progress_Fill_Highlighter_Up,
				ThemeColor_Border_Window,
				ThemeColor_StatusBar_Back,
				ThemeColor_StatusBar_Text,
				ThemeColor_Caption_Text,
				ThemeColor_Header_Normal_Gradient_Start,
				ThemeColor_Header_Normal_Gradient_End,
				ThemeColor_Header_Hot_Gradient_Start,
				ThemeColor_Header_Hot_Gradient_End,
				ThemeColor_ListView_Owner_Selected,
				ThemeColor_SelectedItem_Back,
				ThemeColor_SelectedItem_Text
			};

			inline COLORREF FIXAPI GetThemeSysColor(const ThemeColor color);

			DWORD FIXAPI Comctl32GetSysColor(INT nIndex);
			HBRUSH FIXAPI Comctl32GetSysColorBrush(INT nIndex);
			HBRUSH FIXAPI Comctl32GetSysColorBrushEx(INT nIndex, BOOL reCreate = TRUE);
			HBITMAP FIXAPI LoadImageFromResource(HINSTANCE hInst, DWORD dwResId, LPCSTR ResType);

			bool FIXAPI IsDarkTheme();
		}
	}
}