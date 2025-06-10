// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Graphics.h>
#include <CKPE.Common.UIColored.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			struct CKPE_COMMON_API ThemeData
			{
				Font* ThemeFont;

				THandle hThemeDefaultBrush;
				THandle hThemeText3Brush;
				THandle hThemeEditBrush;
				THandle hThemeDividerBrush;
				THandle hThemeText4Brush;
				THandle hThemeBorderWindowBrush;
				THandle hThemeHighlightBrush;
				THandle hThemeHighlightTextBrush;

				static ThemeData* GetSingleton() noexcept(true);
			};

			CKPE_COMMON_API Theme GetTheme() noexcept(true);
			CKPE_COMMON_API void SetTheme(Theme theme) noexcept(true);

			CKPE_COMMON_API bool IsDarkThemeSystemSupported() noexcept(true);
			CKPE_COMMON_API bool IsCustomThemeExists() noexcept(true);
			CKPE_COMMON_API bool OpenCustomTheme() noexcept(true);
			CKPE_COMMON_API bool NeedDarkCheck() noexcept(true);
			CKPE_COMMON_API TColor GetMaskColorToolbarForCustomTheme() noexcept(true);
			CKPE_COMMON_API std::wstring GetFileNameToolbarForCustomTheme() noexcept(true);
			CKPE_COMMON_API TColor GetMaskColorToolbarNavMeshForCustomTheme() noexcept(true);
			CKPE_COMMON_API std::wstring GetFileNameToolbarNavMeshForCustomTheme() noexcept(true);
			CKPE_COMMON_API TColor GetMaskColorIconsForCustomTheme() noexcept(true);
			CKPE_COMMON_API std::wstring GetFileNameIconsForCustomTheme() noexcept(true);

			CKPE_COMMON_API inline TColor GetThemeSysColor(const ThemeColor color) noexcept(true);

			CKPE_COMMON_API std::uint32_t Comctl32GetSysColor(std::int32_t nIndex) noexcept(true);
			CKPE_COMMON_API THandle Comctl32GetSysColorBrush(std::int32_t nIndex) noexcept(true);
			CKPE_COMMON_API THandle Comctl32GetSysColorBrushEx(std::int32_t nIndex, bool reCreate = true) noexcept(true);

			CKPE_COMMON_API bool IsDarkTheme() noexcept(true);
		}
	}
}