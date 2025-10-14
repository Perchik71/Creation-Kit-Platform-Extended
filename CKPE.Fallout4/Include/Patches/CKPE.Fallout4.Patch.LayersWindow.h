// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class LayersWindow : public Common::PatchBaseWindow
			{
				Common::UI::CUIBaseControl PushbuttonNewLayer;
				Common::UI::CUIBaseControl PushbuttonAddSelection;
				Common::UI::CUIBaseControl EdittextFilter;
				Common::UI::CUIBaseControl PushbuttonPlus;
				Common::UI::CUIBaseControl BodyForData;

				static LRESULT sub(HWND TVHwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true);
				static bool MoveWindowBody(HWND hWnd, std::int32_t X, std::int32_t Y, std::int32_t nWidth,
					std::int32_t nHeight, bool bRepaint) noexcept(true);
				static bool MoveWindowHeader(HWND hWnd, std::int32_t X, std::int32_t Y, std::int32_t nWidth,
					std::int32_t nHeight, bool bRepaint) noexcept(true);

				LayersWindow(const LayersWindow&) = delete;
				LayersWindow& operator=(const LayersWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				LayersWindow();

				inline static ISingleton<LayersWindow> Singleton;
				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}