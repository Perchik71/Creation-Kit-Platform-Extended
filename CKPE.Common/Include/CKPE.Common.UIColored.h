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
			enum Theme : std::uint32_t
			{
				Theme_Custom = 0,
				Theme_Light,
				Theme_Gray,
				Theme_DarkGray,
				Theme_Dark,
				Theme_NightBlue
			};

			enum ThemeColor : std::uint32_t
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

			CKPE_COMMON_API TColor* GetThemeColors(const Theme theme) noexcept(true);
			CKPE_COMMON_API TColor GetThemeSysColor(const Theme theme, const ThemeColor color) noexcept(true);
		}
	}
}