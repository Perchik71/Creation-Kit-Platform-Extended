// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			class ProgressWindow : public Common::PatchBaseWindow
			{
				Common::UI::CUIBaseControl ProgressLabel;
				Common::UI::CUIBaseControl Progress;
				bool isOpen;

				static void update_progressbar(LPCSTR lpcstrText) noexcept(true);
				static HWND sub1(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent,
					DLGPROC lpDialogFunc, LPARAM dwInitParam) noexcept(true);
				static void sub2(uint32_t nPartId, LPCSTR lpcstrText) noexcept(true);

				ProgressWindow(const ProgressWindow&) = delete;
				ProgressWindow& operator=(const ProgressWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ProgressWindow();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				inline static ISingleton<ProgressWindow> Singleton;
				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true);
			};
		}
	}
}