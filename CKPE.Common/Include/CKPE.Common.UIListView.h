// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <uxtheme.h>
#include <CKPE.Graphics.h>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ListView
			{
				typedef LRESULT(*OnCustomDrawHandler)(HWND hWindow, LPNMLVCUSTOMDRAW lpListView, bool& bReturn);
				typedef void(*OnCustomDrawPluginsHandler)(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem,
					const char* lpstrFileName, const char* lpstrTypeName);

				CKPE_COMMON_API HTHEME Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT CALLBACK ListViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true);

				CKPE_COMMON_API void OnCustomDrawItemPlugins(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem) noexcept(true);
				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMLVCUSTOMDRAW lpListView) noexcept(true);

				CKPE_COMMON_API void InstallCustomDrawHandler(OnCustomDrawHandler handler, 
					OnCustomDrawPluginsHandler plugins_handler) noexcept(true);
			}
		}
	}
}