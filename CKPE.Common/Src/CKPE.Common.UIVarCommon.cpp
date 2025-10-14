// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma warning(disable : 6031)

#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Application.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Module.h>
#include <windows.h>
#include <shlwapi.h>
#include <map>

#define min std::min
#define max std::max

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			static std::map<std::string_view, ThemeColor> CustomColorInfo =
			{
				{ "rDefault", ThemeColor_Default },
				{ "rListView_BkColor", ThemeColor_ListView_Color },
				{ "rTreeView_BkColor", ThemeColor_TreeView_Color },
				{ "rEdit_BkColor", ThemeColor_Edit_Color },
				{ "rEdit_BkColor_Disabled", ThemeColor_Edit_Color_Disabled },
				{ "rText_1", ThemeColor_Text_1 },
				{ "rText_2", ThemeColor_Text_2 },
				{ "rText_3", ThemeColor_Text_3 },
				{ "rText_4", ThemeColor_Text_4 },
				{ "rMDIWindow", ThemeColor_MDIWindow },
				{ "rDefault_Gradient_Start", ThemeColor_Default_Gradient_Start },
				{ "rDefault_Gradient_End", ThemeColor_Default_Gradient_End },
				{ "rDivider_Highlighter_Disabled_Gradient_Start", ThemeColor_Divider_Highlighter_Disabled_Gradient_Start },
				{ "rDivider_Highlighter_Disabled_Gradient_End,", ThemeColor_Divider_Highlighter_Disabled_Gradient_End },
				{ "rDivider_Color", ThemeColor_Divider_Color },
				{ "rDivider_Color_Disabled", ThemeColor_Divider_Color_Disabled },
				{ "rDivider_Highlighter", ThemeColor_Divider_Highlighter },
				{ "rDivider_Highlighter_Hot", ThemeColor_Divider_Highlighter_Hot },
				{ "rDivider_Color_Ver2", ThemeColor_Divider_Color_Ver2 },
				{ "rDivider_Color_Disabled_Ver2", ThemeColor_Divider_Color_Disabled_Ver2 },
				{ "rDivider_Highlighter_Ver2", ThemeColor_Divider_Highlighter_Ver2 },
				{ "rDivider_Highlighter_Disabled_Ver2", ThemeColor_Divider_Highlighter_Disabled_Ver2 },
				{ "rDivider_Highlighter_Gradient_Start", ThemeColor_Divider_Highlighter_Gradient_Start },
				{ "rDivider_Highlighter_Gradient_End", ThemeColor_Divider_Highlighter_Gradient_End },
				{ "rDivider_Highlighter_Hot_Gradient_Start", ThemeColor_Divider_Highlighter_Hot_Gradient_Start },
				{ "rDivider_Highlighter_Hot_Gradient_End", ThemeColor_Divider_Highlighter_Hot_Gradient_End },
				{ "rDivider_Highlighter_Pressed", ThemeColor_Divider_Highlighter_Pressed },
				{ "rButton_Pressed_Gradient_Start", ThemeColor_Button_Pressed_Gradient_Start },
				{ "rButton_Pressed_Gradient_End", ThemeColor_Button_Pressed_Gradient_End },
				{ "rButton_Hot_Gradient_Start", ThemeColor_Button_Hot_Gradient_Start },
				{ "rButton_Hot_Gradient_End", ThemeColor_Button_Hot_Gradient_End },
				{ "rButton_Pressed_Divider", ThemeColor_Button_Pressed_Divider },
				{ "rButton_Light_Disabled_Divider", ThemeColor_Button_Light_Disabled_Divider },
				{ "rButton_Disabled_Gradient_Start", ThemeColor_Button_Disabled_Gradient_Start },
				{ "rButton_Disabled_Gradient_End", ThemeColor_Button_Disabled_Gradient_End },
				{ "rCheckBox_Gradient_Start", ThemeColor_CheckBox_Gradient_Start },
				{ "rCheckBox_Gradient_End", ThemeColor_CheckBox_Gradient_End },
				{ "rScrollBar_Gradient_Start", ThemeColor_ScrollBar_Gradient_Start },
				{ "rScrollBar_Gradient_End", ThemeColor_ScrollBar_Gradient_End },
				{ "rScrollBar_Thumb_Gradient_Start", ThemeColor_ScrollBar_Thumb_Gradient_Start },
				{ "rScrollBar_Thumb_Gradient_End", ThemeColor_ScrollBar_Thumb_Gradient_End },
				{ "rScrollBar_Thumb_Gradient_Hot_Start", ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start },
				{ "rScrollBar_Thumb_Gradient_Hot_End", ThemeColor_ScrollBar_Thumb_Gradient_Hot_End },
				{ "rScrollBar_Thumb_Highlighter", ThemeColor_ScrollBar_Thumb_Highlighter },
				{ "rScrollBar_Thumb_Highlighter_Hot", ThemeColor_ScrollBar_Thumb_Highlighter_Hot },
				{ "rShape", ThemeColor_Shape },
				{ "rShape_Hot", ThemeColor_Shape_Hot },
				{ "rShape_Pressed", ThemeColor_Shape_Pressed },
				{ "rShape_Disabled", ThemeColor_Shape_Disabled },
				{ "rShape_Shadow", ThemeColor_Shape_Shadow },
				{ "rShape_Shadow_Disabled", ThemeColor_Shape_Shadow_Disabled },
				{ "rProgress_Fill_Gradient_Start", ThemeColor_Progress_Fill_Gradient_Start },
				{ "rProgress_Fill_Gradient_End", ThemeColor_Progress_Fill_Gradient_End },
				{ "rProgress_Fill_Highlighter", ThemeColor_Progress_Fill_Highlighter },
				{ "rProgress_Fill_Highlighter_Up", ThemeColor_Progress_Fill_Highlighter_Up },
				{ "rBorder_Window", ThemeColor_Border_Window },
				{ "rStatusBar_Back", ThemeColor_StatusBar_Back },
				{ "rStatusBar_Text", ThemeColor_StatusBar_Text },
				{ "rCaption_Text", ThemeColor_Caption_Text },
				{ "rHeader_Normal_Gradient_Start", ThemeColor_Header_Normal_Gradient_Start },
				{ "rHeader_Normal_Gradient_End", ThemeColor_Header_Normal_Gradient_End },
				{ "rHeader_Hot_Gradient_Start", ThemeColor_Header_Hot_Gradient_Start },
				{ "rHeader_Hot_Gradient_End", ThemeColor_Header_Hot_Gradient_End },
				{ "rListView_Owner_Selected", ThemeColor_ListView_Owner_Selected },
				{ "rSelectedItem_Back", ThemeColor_SelectedItem_Back },
				{ "rSelectedItem_Text", ThemeColor_SelectedItem_Text },
			};

			TColor* szCurrentScheme = nullptr;
			bool hasTheme = false, needDarkCheckbox = false;
			TColor szCustomTheme[66] = { 0 };

			TColor CustomTheme_MaskColor_Toolbar;
			std::wstring CustomTheme_FileName_Toolbar;
			TColor CustomTheme_MaskColor_Toolbar_NavMesh;
			std::wstring CustomTheme_FileName_Toolbar_NavMesh;
			TColor CustomTheme_MaskColor_Icons;
			std::wstring CustomTheme_FileName_Icons;

			static ThemeData _sdatatheme{ 0 };

			ThemeData* ThemeData::GetSingleton() noexcept(true)
			{
				return &_sdatatheme;
			}

			static Theme generalCurentTheme = Theme_Dark;

			CKPE_COMMON_API Theme GetTheme() noexcept(true)
			{
				return generalCurentTheme;
			}

			CKPE_COMMON_API void SetTheme(Theme theme) noexcept(true)
			{
				hasTheme = true;
				generalCurentTheme = theme;

				if (theme == Theme_Custom)
					szCurrentScheme = const_cast<TColor*>(szCustomTheme);
				else
					szCurrentScheme = GetThemeColors(theme);

				DeleteObject((HGDIOBJ)_sdatatheme.hThemeDefaultBrush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeText3Brush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeEditBrush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeDividerBrush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeText4Brush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeBorderWindowBrush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeHighlightBrush);
				DeleteObject((HGDIOBJ)_sdatatheme.hThemeHighlightTextBrush);

				_sdatatheme.hThemeDefaultBrush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_Default));
				_sdatatheme.hThemeText3Brush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_Text_3));
				_sdatatheme.hThemeEditBrush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_Edit_Color));
				_sdatatheme.hThemeDividerBrush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_Divider_Color));
				_sdatatheme.hThemeText4Brush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_Text_4));
				_sdatatheme.hThemeBorderWindowBrush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_Border_Window));
				_sdatatheme.hThemeHighlightBrush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_SelectedItem_Back));
				_sdatatheme.hThemeHighlightTextBrush = (THandle)::CreateSolidBrush(GetThemeSysColor(ThemeColor_SelectedItem_Text));
			}

			CKPE_COMMON_API bool IsDarkThemeSystemSupported() noexcept(true)
			{
				auto OsVer = HardwareInfo::OS::GetVersion();		
				return ((GET_EXE_VERSION_EX_MAJOR(OsVer) >= 10) || 
					((GET_EXE_VERSION_EX_MAJOR(OsVer) == 10) && (17763 <= GET_EXE_VERSION_EX_BUILD(OsVer))));
			}

			CKPE_COMMON_API bool IsCustomThemeExists() noexcept(true)
			{
				return Interface::GetSingleton()->HasCustomThemeSetting();
			}

			CKPE_COMMON_API bool OpenCustomTheme() noexcept(true)
			{
				if (!IsCustomThemeExists())
					return false;

				auto Interface = Interface::GetSingleton();
				auto ThemeToml = Interface->GetCustomThemeSettings();

				uint32_t r = 0, g = 0, b = 0;
				TOMLSettingCollection::color_value color_zero, color;
				for (auto it = CustomColorInfo.begin(); it != CustomColorInfo.end(); it++)
				{
					color = ThemeToml->ReadRgbColor("Colors", it->first.data(), color_zero);
					szCustomTheme[(int)(it->second) - 1] = RGB(color.r, color.g, color.b);
				}

				color = ThemeToml->ReadRgbColor("Assets", "rMaskToolbar", color_zero);
				CustomTheme_MaskColor_Toolbar = RGB(color.r, color.g, color.b);
				CustomTheme_FileName_Toolbar = ThemeToml->ReadUnicodeString("Assets", "sImageForToolbar", L"");

				color = ThemeToml->ReadRgbColor("Assets", "rMaskToolbarNavMesh", color_zero);
				CustomTheme_MaskColor_Toolbar_NavMesh = RGB(color.r, color.g, color.b);
				CustomTheme_FileName_Toolbar_NavMesh = ThemeToml->ReadUnicodeString("Assets", "sImageForToolbarNavMesh", L"");

				color = ThemeToml->ReadRgbColor("Assets", "rMaskIcons", color_zero);
				CustomTheme_MaskColor_Icons = RGB(color.r, color.g, color.b);
				CustomTheme_FileName_Icons = ThemeToml->ReadUnicodeString("Assets", "sImageForIcons", L"");

				needDarkCheckbox = ThemeToml->ReadBool("Assets", "bNeedDarkCheckboxInData", false);
				
				if (PathIsRelativeW(CustomTheme_FileName_Toolbar.c_str()))
					CustomTheme_FileName_Toolbar = Interface->GetApplication()->GetPath() + CustomTheme_FileName_Toolbar;
				if (PathIsRelativeW(CustomTheme_FileName_Icons.c_str()))
					CustomTheme_FileName_Icons = Interface->GetApplication()->GetPath() + CustomTheme_FileName_Icons;

				SetTheme(Theme_Custom);
				return true;
			}

			CKPE_COMMON_API bool NeedDarkCheck() noexcept(true)
			{
				return needDarkCheckbox;
			}

			CKPE_COMMON_API TColor GetMaskColorToolbarForCustomTheme() noexcept(true)
			{
				return CustomTheme_MaskColor_Toolbar;
			}

			CKPE_COMMON_API std::wstring GetFileNameToolbarForCustomTheme() noexcept(true)
			{
				return CustomTheme_FileName_Toolbar;
			}

			CKPE_COMMON_API TColor GetMaskColorToolbarNavMeshForCustomTheme() noexcept(true)
			{
				return CustomTheme_MaskColor_Toolbar_NavMesh;
			}

			CKPE_COMMON_API std::wstring GetFileNameToolbarNavMeshForCustomTheme() noexcept(true)
			{
				return CustomTheme_FileName_Toolbar_NavMesh;
			}

			CKPE_COMMON_API TColor GetMaskColorIconsForCustomTheme() noexcept(true)
			{
				return CustomTheme_MaskColor_Icons;
			}

			CKPE_COMMON_API std::wstring GetFileNameIconsForCustomTheme() noexcept(true)
			{
				return CustomTheme_FileName_Icons;
			}

			CKPE_COMMON_API TColor GetThemeSysColor(const ThemeColor color) noexcept(true)
			{
				if (szCurrentScheme)
					return szCurrentScheme[color - 1];
				else
					return 0;
			}

			CKPE_COMMON_API std::uint32_t Comctl32GetSysColor(std::int32_t nIndex) noexcept(true)
			{
				switch (nIndex)
				{
				case COLOR_BTNFACE: return GetThemeSysColor(ThemeColor_Default);
				case COLOR_BTNTEXT: return GetThemeSysColor(ThemeColor_Text_3);
				case COLOR_WINDOW: return GetThemeSysColor(ThemeColor_Edit_Color);
				case COLOR_WINDOWFRAME: return GetThemeSysColor(ThemeColor_Divider_Color);
				case COLOR_WINDOWTEXT: return GetThemeSysColor(ThemeColor_Text_4);
				case COLOR_ACTIVEBORDER: return GetThemeSysColor(ThemeColor_Border_Window);
				case COLOR_INACTIVEBORDER: return GetThemeSysColor(ThemeColor_Divider_Color);
				case COLOR_HIGHLIGHT: return GetThemeSysColor(ThemeColor_SelectedItem_Back);
				case COLOR_HIGHLIGHTTEXT: return GetThemeSysColor(ThemeColor_SelectedItem_Text);
				default:
					return ::GetSysColor(nIndex);
				}
			}

			CKPE_COMMON_API THandle Comctl32GetSysColorBrushEx(std::int32_t nIndex, bool reCreate) noexcept(true)
			{
				if (!reCreate)
					return Comctl32GetSysColorBrush(nIndex);

				return ::CreateSolidBrush(Comctl32GetSysColor(nIndex));
			}

			CKPE_COMMON_API THandle Comctl32GetSysColorBrush(std::int32_t nIndex) noexcept(true)
			{
				switch (nIndex)
				{
				case COLOR_BTNFACE: return ThemeData::GetSingleton()->hThemeDefaultBrush;
				case COLOR_BTNTEXT: return ThemeData::GetSingleton()->hThemeText3Brush;
				//case COLOR_WINDOW: return ThemeData::GetSingleton()->hThemeEditBrush;
				case COLOR_WINDOWFRAME: return ThemeData::GetSingleton()->hThemeDividerBrush;
				case COLOR_WINDOWTEXT: return ThemeData::GetSingleton()->hThemeText4Brush;
				case COLOR_ACTIVEBORDER: return ThemeData::GetSingleton()->hThemeBorderWindowBrush;
				case COLOR_INACTIVEBORDER: return ThemeData::GetSingleton()->hThemeDividerBrush;
				case COLOR_HIGHLIGHT: return ThemeData::GetSingleton()->hThemeHighlightBrush;
				case COLOR_HIGHLIGHTTEXT: return ThemeData::GetSingleton()->hThemeHighlightTextBrush;
				default:
					return ::GetSysColorBrush(nIndex);
				}
			}

			CKPE_COMMON_API THandle Comctl32GetSysColorBrush2(std::int32_t nIndex) noexcept(true)
			{
				switch (nIndex)
				{
				case COLOR_BTNFACE: return ThemeData::GetSingleton()->hThemeDefaultBrush;
				case COLOR_BTNTEXT: return ThemeData::GetSingleton()->hThemeText3Brush;
				case COLOR_WINDOW: return ThemeData::GetSingleton()->hThemeEditBrush;
				case COLOR_WINDOWFRAME: return ThemeData::GetSingleton()->hThemeDividerBrush;
				case COLOR_WINDOWTEXT: return ThemeData::GetSingleton()->hThemeText4Brush;
				case COLOR_ACTIVEBORDER: return ThemeData::GetSingleton()->hThemeBorderWindowBrush;
				case COLOR_INACTIVEBORDER: return ThemeData::GetSingleton()->hThemeDividerBrush;
				case COLOR_HIGHLIGHT: return ThemeData::GetSingleton()->hThemeHighlightBrush;
				case COLOR_HIGHLIGHTTEXT: return ThemeData::GetSingleton()->hThemeHighlightTextBrush;
				default:
					return ::GetSysColorBrush(nIndex);
				}
			}

			CKPE_COMMON_API bool IsDarkTheme() noexcept(true)
			{
				return hasTheme;
			}
		}
	}
}