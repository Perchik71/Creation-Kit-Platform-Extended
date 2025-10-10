// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ProgressTaskBar.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <Patches/CKPE.Fallout4.Patch.MainWindow.h>
#include <Patches/CKPE.Fallout4.Patch.ProgressWindow.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			Common::ProgressTaskBar* ProgressTaskBarPtr = nullptr;
			LPDWORD dwProgressLoadCurrent, dwProgressLoadMax;
			static EditorAPI::BSString sProgressLoadText;

			std::uintptr_t pointer_ProgressWindow_sub = 0;
			long value_ProgressWindow_pos = 0;

			constexpr std::uint32_t UI_PROGRESS_ID = 31007;
			constexpr std::uint32_t UI_PROGRESS_LABEL_ID = 2217;
			constexpr std::uint32_t UI_PROGRESS_MAXVALUE = 399;

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool ProgressWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				SafeWrite::WriteNop(__CKPE_OFFSET(5), 2);
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub1);

				// Hook Loading Files...Initializing...
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub2);
				// Hook Loading Files...Initializing References...
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&sub2);
				// Hook Validating forms...
				Detours::DetourCall(__CKPE_OFFSET(6), (std::uintptr_t)&sub2);

				// Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
				//
				dwProgressLoadCurrent = (LPDWORD)__CKPE_OFFSET(7);
				dwProgressLoadMax = (LPDWORD)__CKPE_OFFSET(8);
				auto rva = __CKPE_OFFSET(9);
				SafeWrite::Write(rva, { 0x48, 0x8D, 0x4D, 0x78 });

				if (verPatch == 1)
				{
					SafeWrite::WriteNop(rva + 0x4, 0x30);
					Detours::DetourCall(rva + 0x4, (std::uintptr_t)&update_progressbar);
					SafeWrite::Write(rva + 0x34, { 0xEB });
				}
				else
				{
					SafeWrite::WriteNop(rva + 0x4, 0x34);
					Detours::DetourCall(rva + 0x4, (std::uintptr_t)&update_progressbar);
					SafeWrite::Write(rva + 0x38, { 0xEB });

					// Idk what kind of gifted UI/UX specialist is sitting at Bethesda, 
					// but this is the most shitty solution.
					// 
					// bUseVersionControl=0 by the way...
					// 
					// The output of a message to the user, every time you load something, 
					// should only be in the form of an error, and postpone the load of something.
					SafeWrite::Write(__CKPE_OFFSET(10), { 0xE9, 0xFC, 0x01, 0x00, 0x00, 0x90 });
				}

				pointer_ProgressWindow_sub = __CKPE_OFFSET(2);

				return true;
			}

			INT_PTR CALLBACK ProgressWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
				case WM_INITDIALOG:
				{
					ProgressWindow::Singleton->isOpen = true;
					ProgressWindow::Singleton->m_hWnd = Hwnd;
					ProgressWindow::Singleton->ProgressLabel = GetDlgItem(Hwnd, UI_PROGRESS_LABEL_ID);
					ProgressWindow::Singleton->Progress = GetDlgItem(Hwnd, UI_PROGRESS_ID);

					ProgressWindow::Singleton->ProgressLabel.SetCaption("Loading Files...");
					ProgressWindow::Singleton->Progress.Style |= PBS_SMOOTH;
					ProgressWindow::Singleton->Progress.Perform(PBM_SETRANGE, 0, MAKELPARAM(0, UI_PROGRESS_MAXVALUE));
					ProgressWindow::Singleton->Progress.Perform(PBM_SETSTEP, 1, 0);
					ProgressWindow::Singleton->Progress.Perform(PBM_SETPOS, 0, 0);
					value_ProgressWindow_pos = 0;

					ProgressTaskBarPtr = new Common::ProgressTaskBar(MainWindow::Singleton->Handle, UI_PROGRESS_MAXVALUE + 1);
					if (ProgressTaskBarPtr)
					{
						ProgressTaskBarPtr->Begin();
						// default more often than not, we see uncertain progress
						ProgressTaskBarPtr->SetMarquee(true);
					}

					ShowWindow(Hwnd, SW_SHOW);
					UpdateWindow(Hwnd);
				}
				break;
				case WM_DESTROY:
				{
					if (ProgressTaskBarPtr)
					{
						delete ProgressTaskBarPtr;
						ProgressTaskBarPtr = nullptr;
					}

					ProgressWindow::Singleton->isOpen = false;
					ProgressWindow::Singleton->m_hWnd = nullptr;
					ProgressWindow::Singleton->ProgressLabel = nullptr;
					ProgressWindow::Singleton->Progress = nullptr;
				}
				break;
				}

				return CallWindowProc(ProgressWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			HWND ProgressWindow::sub1(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent,
				DLGPROC lpDialogFunc, LPARAM dwInitParam) noexcept(true)
			{
				return (HWND)Common::EditorUI::Hook::HKCreateDialogParamA(hInstance, MAKEINTRESOURCE(3238), hWndParent,
					(std::uintptr_t)HKWndProc, dwInitParam);
			}

			void ProgressWindow::update_progressbar(LPCSTR lpcstrText) noexcept(true)
			{
				// For some reason, 95 does not go further
				// NEWS: Fallout4.esm contains ~80k incorrect forms.
				//static DWORD dwProgressLoadMax95 = (DWORD)(*dwProgressLoadMax * 95.0f) / 100.0f;
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

				ProgressWindow::Singleton->ProgressLabel.SetCaption(sProgressLoadText.c_str());
				fast_call<void>(pointer_ProgressWindow_sub, 3, sProgressLoadText.c_str());

				ProgressWindow::Singleton->Progress.Perform(PBM_STEPIT, 0, 0);
				ProgressWindow::Singleton->Progress.Refresh();
				if (ProgressTaskBarPtr) ProgressTaskBarPtr->Step();
			}

			void ProgressWindow::sub2(uint32_t nPartId, LPCSTR lpcstrText) noexcept(true)
			{
				if (ProgressWindow::Singleton->isOpen)
				{
					ProgressWindow::Singleton->ProgressLabel.SetCaption(lpcstrText);
					ProgressWindow::Singleton->Progress.Perform(PBM_SETPOS, UI_PROGRESS_MAXVALUE, 0);
					ProgressWindow::Singleton->Progress.Refresh();

					if (ProgressTaskBarPtr) ProgressTaskBarPtr->SetMarquee(true);
				}

				return fast_call<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}
		}
	}
}