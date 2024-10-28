// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/ProgressTaskBar.h"
#include "Editor API/EditorUI.h"
#include "Editor API/BSString.h"
#include "Patches/UIThemePatch.h"
#include "MainWindowF4.h"
#include "ProgressWindowF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			ProgressWindow* GlobalProgressWindowPtr = nullptr;
			ProgressTaskBar* ProgressTaskBarPtr = nullptr;

			uintptr_t pointer_ProgressWindow_sub = 0;
			long value_ProgressWindow_pos = 0;

			constexpr uint32_t UI_PROGRESS_ID = 31007;
			constexpr uint32_t UI_PROGRESS_LABEL_ID = 2217;
			constexpr uint32_t MAX_PROGRESSBAR_VALUE = 399;

			static LPDWORD dwProgressLoadCurrent = nullptr;
			static LPDWORD dwProgressLoadMax = nullptr;
			static EditorAPI::BSString sProgressLoadText;

			bool ProgressWindow::HasOption() const
			{
				return false;
			}

			bool ProgressWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ProgressWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* ProgressWindow::GetName() const
			{
				return "Progress Window";
			}

			bool ProgressWindow::HasDependencies() const
			{
				return true;
			}

			Array<String> ProgressWindow::GetDependencies() const
			{
				return { "Main Window" };
			}

			bool ProgressWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool ProgressWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					lpRelocator->PatchNop(_RELDATA_RAV(5), 2);
					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&sub1);

					// Hook Loading Files %d%% (%s)
					//lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&sub3);
					// Hook Loading Files...Initializing...
					lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&sub2);
					// Hook Loading Files...Initializing References...
					lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&sub2);
					// Hook Validating forms...
					lpRelocator->DetourCall(_RELDATA_RAV(6), (uintptr_t)&sub2);

					// Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
					//
					dwProgressLoadCurrent = (LPDWORD)_RELDATA_ADDR(7);
					dwProgressLoadMax = (LPDWORD)_RELDATA_ADDR(8);
					auto rva = (uintptr_t)(_RELDATA_RAV(9));
					lpRelocator->Patch(rva, { 0x48, 0x8D, 0x4D, 0x78 });

					if (verPatch == 1)
					{
						lpRelocator->PatchNop(rva + 0x4, 0x30);
						lpRelocator->DetourCall(rva + 0x4, (uintptr_t)&update_progressbar);
						lpRelocator->Patch(rva + 0x34, { 0xEB });
					}
					else
					{
						lpRelocator->PatchNop(rva + 0x4, 0x34);
						lpRelocator->DetourCall(rva + 0x4, (uintptr_t)&update_progressbar);
						lpRelocator->Patch(rva + 0x38, { 0xEB });

						// Idk what kind of gifted UI/UX specialist is sitting at Bethesda, 
						// but this is the most shitty solution.
						// 
						// bUseVersionControl=0 by the way...
						// 
						// The output of a message to the user, every time you load something, 
						// should only be in the form of an error, and postpone the load of something.
						lpRelocator->Patch(_RELDATA_RAV(10), { 0xE9, 0xFC, 0x01, 0x00, 0x00, 0x90 });
					}

					pointer_ProgressWindow_sub = _RELDATA_ADDR(2);

					return true;
				}

				return false;
			}

			bool ProgressWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			ProgressWindow::ProgressWindow() : BaseWindow(), Classes::CUIBaseWindow(), isOpen(false)
			{
				Assert(!GlobalProgressWindowPtr);
				GlobalProgressWindowPtr = this;
			}

			LRESULT CALLBACK ProgressWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						GlobalProgressWindowPtr->isOpen = true;
						GlobalProgressWindowPtr->m_hWnd = Hwnd;
						GlobalProgressWindowPtr->ProgressLabel = GetDlgItem(Hwnd, UI_PROGRESS_LABEL_ID);
						GlobalProgressWindowPtr->Progress = GetDlgItem(Hwnd, UI_PROGRESS_ID);
						
						GlobalProgressWindowPtr->ProgressLabel.Caption = "Loading Files...";
						GlobalProgressWindowPtr->Progress.Style |= PBS_SMOOTH;
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETRANGE, 0, MAKELPARAM(0, MAX_PROGRESSBAR_VALUE));
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETSTEP, 1, 0);
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETPOS, 0, 0);
						value_ProgressWindow_pos = 0;

						if (GlobalMainWindowPtr)
						{
							ProgressTaskBarPtr = new ProgressTaskBar(GlobalMainWindowPtr->Handle);
							if (ProgressTaskBarPtr)
							{
								ProgressTaskBarPtr->SetTotal(MAX_PROGRESSBAR_VALUE);
								ProgressTaskBarPtr->Begin();
							}
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

						GlobalProgressWindowPtr->isOpen = false;
						GlobalProgressWindowPtr->m_hWnd = nullptr;
						GlobalProgressWindowPtr->ProgressLabel = nullptr;
						GlobalProgressWindowPtr->Progress = nullptr;
					}
					return 0;
				}

				return DefWindowProc(Hwnd, Message, wParam, lParam);
			}

			void ProgressWindow::update_progressbar(LPCSTR lpcstrText)
			{
				// For some reason, 95 does not go further
				static DWORD dwProgressLoadMax95 = (DWORD)(*dwProgressLoadMax * 95.0f) / 100.0f;
				if (*dwProgressLoadCurrent == dwProgressLoadMax95)
					return;

				static double lastPercent = 0.0f;
				(*dwProgressLoadCurrent)++;

				// Only update every quarter percent, rather than every single form load
				double newPercent = ((double)(*dwProgressLoadCurrent) / (double)(dwProgressLoadMax95)) * 100.0f;
				if (abs(lastPercent - newPercent) <= 0.25f)
					return;

				lastPercent = newPercent;

				sProgressLoadText.Format("Loading Files %d%% (%s)", (int)(lastPercent + 0.5), lpcstrText);

				GlobalProgressWindowPtr->ProgressLabel.Caption = sProgressLoadText.c_str();
				fastCall<void>(pointer_ProgressWindow_sub, 3, sProgressLoadText.c_str());

				GlobalProgressWindowPtr->Progress.Perform(PBM_STEPIT, 0, 0);
				GlobalProgressWindowPtr->Progress.Refresh();
				if (ProgressTaskBarPtr) ProgressTaskBarPtr->Step();
			}

			HWND ProgressWindow::sub1(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent,
				DLGPROC lpDialogFunc, LPARAM dwInitParam)
			{
				return EditorAPI::EditorUI::HKCreateDialogParamA(hInstance, MAKEINTRESOURCE(3238), hWndParent,
					HKWndProc, dwInitParam);
			}

			void ProgressWindow::sub2(uint32_t nPartId, LPCSTR lpcstrText)
			{
				if (GlobalProgressWindowPtr->isOpen)
				{
					GlobalProgressWindowPtr->ProgressLabel.Caption = lpcstrText;
					GlobalProgressWindowPtr->Progress.Perform(PBM_SETPOS, MAX_PROGRESSBAR_VALUE, 0);
					GlobalProgressWindowPtr->Progress.Refresh();

					if (ProgressTaskBarPtr) ProgressTaskBarPtr->SetMarquee(true);
				}

				return fastCall<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}
		}
	}
}