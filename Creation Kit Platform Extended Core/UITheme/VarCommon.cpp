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

#include "VarCommon.h"
#include "Core/Engine.h"

#define CUSTOM_THEME "CreationKitPlatformExtendedCustomTheme.ini"

#define min std::min
#define max std::max

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		static Map<std::string_view, ThemeColor> CustomColorInfo =
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

		bool hasTheme = false, needDarkCheckbox = false;
		::Core::Classes::UI::CUIFont* ThemeFont = nullptr;
		COLORREF szCustomTheme[66] = { 0 };

		COLORREF CustomTheme_MaskColor_Toolbar;
		String CustomTheme_FileName_Toolbar;
		COLORREF CustomTheme_MaskColor_Toolbar_NavMesh;
		String CustomTheme_FileName_Toolbar_NavMesh;
		COLORREF CustomTheme_MaskColor_Icons;
		String CustomTheme_FileName_Icons;

		HBRUSH hThemeDefaultBrush = NULL;
		HBRUSH hThemeText3Brush = NULL;
		HBRUSH hThemeEditBrush = NULL;
		HBRUSH hThemeDividerBrush = NULL;
		HBRUSH hThemeText4Brush = NULL;
		HBRUSH hThemeBorderWindowBrush = NULL;
		HBRUSH hThemeHighlightBrush = NULL;
		HBRUSH hThemeHighlightTextBrush = NULL;
		COLORREF* szCurrentScheme = NULL;

		static Theme generalCurentTheme = Theme_Dark;

		Theme GetTheme()
		{
			return generalCurentTheme;
		}

		void SetTheme(Theme theme)
		{
			hasTheme = true;
			generalCurentTheme = theme;

			if (theme == Theme_Custom)
				szCurrentScheme = const_cast<COLORREF*>(szCustomTheme);
			else
				szCurrentScheme = GetThemeColors(theme);

			Assert(szCurrentScheme);

			DeleteObject(hThemeDefaultBrush);
			DeleteObject(hThemeText3Brush);
			DeleteObject(hThemeEditBrush);
			DeleteObject(hThemeDividerBrush);
			DeleteObject(hThemeText4Brush);
			DeleteObject(hThemeBorderWindowBrush);
			DeleteObject(hThemeHighlightBrush);
			DeleteObject(hThemeHighlightTextBrush);

			hThemeDefaultBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor_Default));
			hThemeText3Brush = CreateSolidBrush(GetThemeSysColor(ThemeColor_Text_3));
			hThemeEditBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor_Edit_Color));
			hThemeDividerBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor_Divider_Color));
			hThemeText4Brush = CreateSolidBrush(GetThemeSysColor(ThemeColor_Text_4));
			hThemeBorderWindowBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor_Border_Window));
			hThemeHighlightBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor_SelectedItem_Back));
			hThemeHighlightTextBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor_SelectedItem_Text));
		}

		bool IsDarkThemeSystemSupported()
		{
			auto OsVer = Core::GlobalEnginePtr->GetSystemVersion();
			return (OsVer.MajorVersion == 10 && OsVer.MinorVersion == 0 && 17763 <= OsVer.BuildNubmer);
		}

		bool IsCustomThemeExists()
		{
			return Utils::FileExists((Utils::GetApplicationPath() + CUSTOM_THEME).c_str());
		}

		void ScanColorFromString(const String& Src, const char* Name, uint32_t* r, uint32_t* g, uint32_t* b)
		{
			__try
			{
				sscanf(Src.c_str(), "%u,%u,%u", r, g, b);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				_ERROR("The contents of the \"%s\" color option could not be read or recognized.", Name);
			}
		}

		bool OpenCustomTheme()
		{
			if (!IsCustomThemeExists())
				return false;

			SmartPointerInfo<Core::INIConfig> ThemeIni = new Core::INIConfig((Utils::GetApplicationPath() + CUSTOM_THEME).c_str());
			if (ThemeIni.Empty())
				return false;

			String Text;
			uint32_t r = 0, g = 0, b = 0;
			for (auto it = CustomColorInfo.begin(); it != CustomColorInfo.end(); it++)
			{
				Text = ThemeIni->ReadString("Colors", it->first.data(), "0,0,0");
				Text.erase(remove_if(Text.begin(), Text.end(), isspace), Text.end());
				ScanColorFromString(Text, it->first.data(), &r, &g, &b);
				szCustomTheme[(int)(it->second) - 1] = RGB(r, g, b);
				r = 0; g = 0; b = 0;
			}

			Text = ThemeIni->ReadString("Assets", "rMaskToolbar", "0,0,0");
			ScanColorFromString(Text, "rMaskToolbar", &r, &g, &b);
			CustomTheme_MaskColor_Toolbar = RGB(r, g, b);
			CustomTheme_FileName_Toolbar = ThemeIni->ReadString("Assets", "sImageForToolbar", "");

			Text = ThemeIni->ReadString("Assets", "rMaskToolbarNavMesh", "0,0,0");
			ScanColorFromString(Text, "rMaskToolbar", &r, &g, &b);
			CustomTheme_MaskColor_Toolbar_NavMesh = RGB(r, g, b);
			CustomTheme_FileName_Toolbar_NavMesh = ThemeIni->ReadString("Assets", "sImageForToolbarNavMesh", "");

			Text = ThemeIni->ReadString("Assets", "rMaskIcons", "0,0,0");
			ScanColorFromString(Text, "rMaskIcons", &r, &g, &b);
			CustomTheme_MaskColor_Icons = RGB(r, g, b);
			CustomTheme_FileName_Icons = ThemeIni->ReadString("Assets", "sImageForIcons", "");

			needDarkCheckbox = ThemeIni->ReadBool("Assets", "bNeedDarkCheckboxInData", false);

			if (PathIsRelative(CustomTheme_FileName_Toolbar.c_str()))
				CustomTheme_FileName_Toolbar = Utils::GetApplicationPath() + CustomTheme_FileName_Toolbar;
			if (PathIsRelative(CustomTheme_FileName_Icons.c_str()))
				CustomTheme_FileName_Icons = Utils::GetApplicationPath() + CustomTheme_FileName_Icons;

			SetTheme(Theme_Custom);
			return true;
		}

		bool NeedDarkCheck()
		{
			return needDarkCheckbox;
		}

		COLORREF GetMaskColorToolbarForCustomTheme()
		{
			return CustomTheme_MaskColor_Toolbar;
		}

		String GetFileNameToolbarForCustomTheme()
		{
			return CustomTheme_FileName_Toolbar;
		}

		COLORREF GetMaskColorToolbarNavMeshForCustomTheme()
		{
			return CustomTheme_MaskColor_Toolbar_NavMesh;
		}

		String GetFileNameToolbarNavMeshForCustomTheme()
		{
			return CustomTheme_FileName_Toolbar_NavMesh;
		}

		COLORREF GetMaskColorIconsForCustomTheme()
		{
			return CustomTheme_MaskColor_Icons;
		}

		String GetFileNameIconsForCustomTheme()
		{
			return CustomTheme_FileName_Icons;
		}

		COLORREF GetThemeSysColor(const ThemeColor color)
		{
			if (szCurrentScheme)
				return szCurrentScheme[color - 1];
			else
				return 0;
		}

		DWORD Comctl32GetSysColor(INT nIndex)
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

		HBRUSH Comctl32GetSysColorBrushEx(INT nIndex, BOOL reCreate)
		{
			if (!reCreate)
				return Comctl32GetSysColorBrush(nIndex);

			return CreateSolidBrush(Comctl32GetSysColor(nIndex));
		}

		HBRUSH Comctl32GetSysColorBrush(INT nIndex)
		{
			switch (nIndex)
			{
			case COLOR_BTNFACE: return hThemeDefaultBrush;
			case COLOR_BTNTEXT: return hThemeText3Brush;
			case COLOR_WINDOWFRAME: return hThemeDividerBrush;
			case COLOR_WINDOWTEXT: return hThemeText4Brush;
			case COLOR_ACTIVEBORDER: return hThemeBorderWindowBrush;
			case COLOR_INACTIVEBORDER: return hThemeDividerBrush;
			case COLOR_HIGHLIGHT: return hThemeHighlightBrush;
			case COLOR_HIGHLIGHTTEXT: return hThemeHighlightTextBrush;
			default:
				return ::GetSysColorBrush(nIndex);
			}
		}

		HBITMAP LoadImageFromResource(HINSTANCE hInst, DWORD dwResId, LPCSTR ResType)
		{
			// https://code911.top/howto/c-gdi-how-to-get-and-load-image-from-resource

			HBITMAP Result;

			IStream* pStream = nullptr;
			Gdiplus::Bitmap* pBmp = nullptr;
			HGLOBAL hGlobal = nullptr;
			// get the handle to the resource
			HRSRC hrsrc = FindResourceA(hInst, MAKEINTRESOURCEA(dwResId), ResType);     
			if (hrsrc)
			{
				DWORD dwResourceSize = SizeofResource(hInst, hrsrc);
				if (dwResourceSize > 0)
				{
					HGLOBAL hGlobalResource = LoadResource(hInst, hrsrc); // load it
					if (hGlobalResource)
					{
						void* imagebytes = LockResource(hGlobalResource); // get a pointer to the file bytes
						// copy image bytes into a real hglobal memory handle
						hGlobal = ::GlobalAlloc(GHND, dwResourceSize);
						if (hGlobal)
						{
							void* pBuffer = ::GlobalLock(hGlobal);
							if (pBuffer)
							{
								memcpy(pBuffer, imagebytes, dwResourceSize);
								HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
								if (SUCCEEDED(hr))
								{
									// pStream now owns the global handle and will invoke GlobalFree on release
									hGlobal = nullptr;
									pBmp = new Gdiplus::Bitmap(pStream);
								}
							}
						}
					}
				}
			}
			if (pStream)
			{
				pStream->Release();
				pStream = nullptr;
			}
			if (hGlobal)
			{
				GlobalFree(hGlobal);
				hGlobal = nullptr;
			}

			if (!pBmp)
				return 0;
			else
			{
				pBmp->GetHBITMAP(0, &Result);
				delete pBmp;
			}
				
			return Result;
		}

		bool IsDarkTheme()
		{
			return hasTheme;
		}
	}
}