// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESObjectREFR.h>
#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>
#include <CKPE.ImageList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class ProgressWindow : public Common::PatchBaseWindow
			{
				Common::UI::CUIBaseControl ProgressLabel;
				Common::UI::CUIBaseControl Progress;
				bool isOpen{ false };

				ProgressWindow(const ProgressWindow&) = delete;
				ProgressWindow& operator=(const ProgressWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ProgressWindow();

				inline static ISingleton<ProgressWindow> Singleton;
				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

				static void sub1(std::uint32_t nPartId, LPCSTR lpcstrText) noexcept(true);
				static void step() noexcept(true);

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}