// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
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
				using namespace Common;

				auto rel = Relocation(ID{ 634156, 1725866 });
				rel.WriteFill<0x1C>(NOP, 0x2);
				rel.WriteCall<0x3E>(&sub1);

				// Hook Loading Files...Initializing...
				Relocation(ID{ 452879, 1429824 }, Offset{ 0x47E, 0x3EE }).WriteCall(&sub2);
				// Hook Loading Files...Initializing References...
				Relocation(ID{ 654059, 1995336 }, Offset{ 0x16, 0x12 }).WriteCall(&sub2);
				// Hook Validating forms...
				Relocation(ID{ 653598, 1484721 }, Offset{ 0x20, 0x27 }).WriteCall(&sub2);

				// Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
				//
				dwProgressLoadCurrent = reinterpret_cast<LPDWORD>(ID{ 1110197, 1493376 }.Address());
				dwProgressLoadMax = reinterpret_cast<LPDWORD>(ID{ 365092, 1990122 }.Address());

				auto rel2 = Relocation(ID{ 264710, 1495600 }, Offset{ 0x23C, 0x264 });
				rel2.Write({ 0x48, 0x8D, 0x4D, 0x78 });

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					rel2.WriteFill<0x4>(NOP, 0x30);
					rel2.WriteCall<0x4>(&update_progressbar);
					rel2.Write<0x34>({ JMP });

					// Idk what kind of gifted UI/UX specialist is sitting at Bethesda,
					// but this is the most shitty solution.
					//
					// bUseVersionControl=0 by the way...
					//
					// The output of a message to the user, every time you load something,
					// should only be in the form of an error, and postpone the load of something.
					Relocation(ID(653593), 0x2F1).Write({ 0xE9, 0x2B, 0x02, 0x00, 0x00, 0x90 });
				}
				else
				{
					rel2.WriteFill<0x4>(NOP, 0x34);
					rel2.WriteCall<0x4>(&update_progressbar);
					rel2.Write<0x38>({ JMP });

					// Idk what kind of gifted UI/UX specialist is sitting at Bethesda,
					// but this is the most shitty solution.
					//
					// bUseVersionControl=0 by the way...
					//
					// The output of a message to the user, every time you load something,
					// should only be in the form of an error, and postpone the load of something.
					Relocation(ID(1994158), 0x2EE).Write({ 0xE9, 0xFC, 0x01, 0x00, 0x00, 0x90 });
				}

				pointer_ProgressWindow_sub = Relocation(ID(142976)).Address();

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