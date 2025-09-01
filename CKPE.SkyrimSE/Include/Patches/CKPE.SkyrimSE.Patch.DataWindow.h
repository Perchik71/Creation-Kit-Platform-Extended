// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>
#include <CKPE.ImageList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class DataWindow : public Common::PatchBaseWindow
			{
				constexpr static std::int32_t VisibleGroupId = 0;
				constexpr static std::int32_t FilteredGroupId = 1;

				ImageList m_ImageList;
				Common::UI::CUIBaseControl m_pluginList;

				DataWindow(const DataWindow&) = delete;
				DataWindow& operator=(const DataWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				DataWindow();

				inline static ISingleton<DataWindow> Singleton;
				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void DoListViewDraw(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem,
					const char* lpstrFileName, const char* lpstrTypeName) noexcept(true);

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}