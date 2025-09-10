// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ProgressTaskBar.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <Patches/CKPE.Starfield.Patch.MainWindow.h>
#include <Patches/CKPE.Starfield.Patch.ProgressWindow.h>

#include <commctrl.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			Common::ProgressTaskBar* ProgressTaskBarPtr = nullptr;

			uintptr_t pointer_ProgressWindow_sub = 0;
			long value_ProgressWindow_pos = 0;

			constexpr uint32_t UI_PROGRESS_ID = 31007;
			constexpr uint32_t UI_PROGRESS_LABEL_ID = 2217;
			constexpr uint32_t MAX_PROGRESSBAR_VALUE = 399;

			static LPDWORD dwProgressLoadCurrent = nullptr;
			static LPDWORD dwProgressLoadMax = nullptr;
			static EditorAPI::BSString sProgressLoadText;

			ProgressWindow::ProgressWindow() : Common::PatchBaseWindow()
			{
				SetName("Progress Window");
				Singleton = this;
			}

			bool ProgressWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ProgressWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ProgressWindow::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> ProgressWindow::GetDependencies() const noexcept(true)
			{
				return { "Main Window" };
			}

			bool ProgressWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool ProgressWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				auto rva = (std::uintptr_t)(__CKPE_OFFSET(0));
				SafeWrite::WriteNop(rva, 2);
				Detours::DetourCall(rva + 0x27, (std::uintptr_t)&sub1);

				// Hook Loading Files...Initializing...
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub2);
				pointer_ProgressWindow_sub = __CKPE_OFFSET(2);

				// Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
				//
				dwProgressLoadCurrent = (LPDWORD)__CKPE_OFFSET(3);
				dwProgressLoadMax = (LPDWORD)__CKPE_OFFSET(4);

				rva = (std::uintptr_t)(__CKPE_OFFSET(5));
				SafeWrite::Write(rva, { 0x48, 0x8D, 0x4F, 0x40 });
				SafeWrite::WriteNop(rva + 0x4, 0x31);
				Detours::DetourCall(rva + 0x4, (std::uintptr_t)&update_progressbar);
				SafeWrite::Write(rva + 0x35, { 0xEB });

				return true;
			}

			LRESULT CALLBACK ProgressWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						auto s = ProgressWindow::Singleton.GetSingleton();

						s->isOpen = true;
						s->m_hWnd = Hwnd;
						s->ProgressLabel = GetDlgItem(Hwnd, UI_PROGRESS_LABEL_ID);
						s->Progress = GetDlgItem(Hwnd, UI_PROGRESS_ID);

						s->ProgressLabel.SetCaption("Loading Files...");
						s->Progress.Style |= PBS_SMOOTH;
						s->Progress.Perform(PBM_SETRANGE, 0, MAKELPARAM(0, MAX_PROGRESSBAR_VALUE));
						s->Progress.Perform(PBM_SETSTEP, 1, 0);
						s->Progress.Perform(PBM_SETPOS, 0, 0);
						value_ProgressWindow_pos = 0;

						ProgressTaskBarPtr = new Common::ProgressTaskBar(MainWindow::Singleton->Handle, MAX_PROGRESSBAR_VALUE);
						if (ProgressTaskBarPtr)
						{
							ProgressTaskBarPtr->SetTotal(MAX_PROGRESSBAR_VALUE);
							ProgressTaskBarPtr->Begin();
							// default more often than not, we see uncertain progress
							//ProgressTaskBarPtr->SetMarquee(true);
						}

						ShowWindow(Hwnd, SW_SHOW);
						UpdateWindow(Hwnd);
					}
					return 0;
					case WM_DESTROY:
					{
						if (ProgressTaskBarPtr)
						{
							delete ProgressTaskBarPtr;
							ProgressTaskBarPtr = nullptr;
						}

						auto s = ProgressWindow::Singleton.GetSingleton();

						s->isOpen = false;
						s->m_hWnd = nullptr;
						s->ProgressLabel = nullptr;
						s->Progress = nullptr;
					}
					return 0;
				}

				return DefWindowProc(Hwnd, Message, wParam, lParam);
			}

			void ProgressWindow::update_progressbar(LPCSTR lpcstrText) noexcept(true)
			{
				if (*dwProgressLoadCurrent == *dwProgressLoadMax)
					return;

				static double lastPercent = 0.0f;
				(*dwProgressLoadCurrent)++;

				// Only update every quarter percent, rather than every single form load
				double newPercent = ((double)(*dwProgressLoadCurrent) / (double)(*dwProgressLoadMax)) * 100.0f;
				if (abs(lastPercent - newPercent) <= 0.25f)
					return;

				lastPercent = newPercent;

				sProgressLoadText.Format("Loading Files %d%% (%s)", (int)(lastPercent + 0.5), lpcstrText);
				auto s = ProgressWindow::Singleton.GetSingleton();

				s->ProgressLabel.SetCaption(sProgressLoadText.c_str());
				fast_call<void>(pointer_ProgressWindow_sub, 3, sProgressLoadText.c_str());

				s->Progress.Perform(PBM_STEPIT, 0, 0);
				if (ProgressTaskBarPtr) ProgressTaskBarPtr->Step();
			}

			HWND ProgressWindow::sub1(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent,
				DLGPROC lpDialogFunc, LPARAM dwInitParam) noexcept(true)
			{
				return (HWND)Common::EditorUI::Hook::HKCreateDialogParamA(hInstance, MAKEINTRESOURCE(3238), hWndParent,
					(std::uintptr_t)HKWndProc, dwInitParam);
			}

			void ProgressWindow::sub2(uint32_t nPartId, LPCSTR lpcstrText) noexcept(true)
			{
				auto s = ProgressWindow::Singleton.GetSingleton();

				if (s->isOpen)
				{
					s->ProgressLabel.SetCaption(lpcstrText);
					s->Progress.Style |= PBS_MARQUEE;
					s->Progress.Perform(PBM_SETMARQUEE, (WPARAM)1, (LPARAM)100);

					if (ProgressTaskBarPtr) ProgressTaskBarPtr->SetMarquee(true);
				}

				return fast_call<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}
		}
	}
}