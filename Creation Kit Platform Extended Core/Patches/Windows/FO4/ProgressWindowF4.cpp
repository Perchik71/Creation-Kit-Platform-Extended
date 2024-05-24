// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/ProgressTaskBar.h"
#include "Editor API/EditorUI.h"
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
				return false;
			}

			Array<String> ProgressWindow::GetDependencies() const
			{
				return {};
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
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(5), 2);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub1);

					// Hook Loading Files %d%% (%s)
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub3);
					// Hook Loading Files...Initializing...
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&sub2);
					// Hook Loading Files...Initializing References...
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub2);
					// Hook Validating forms...
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(6), (uintptr_t)&sub2);

					if (verPatch == 2)
						// Idk what kind of gifted UI/UX specialist is sitting at Bethesda, 
						// but this is the most shitty solution.
						// 
						// bUseVersionControl=0 by the way...
						// 
						// The output of a message to the user, every time you load something, 
						// should only be in the form of an error, and postpone the load of something.
						lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xEB });

					pointer_ProgressWindow_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

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
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETRANGE, 0, MAKELPARAM(0, 95));
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETSTEP, 1, 0);
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETPOS, 0, 0);
						value_ProgressWindow_pos = 0;

						if (GlobalMainWindowPtr)
						{
							ProgressTaskBarPtr = new ProgressTaskBar(GlobalMainWindowPtr->Handle);
							if (ProgressTaskBarPtr) ProgressTaskBarPtr->Begin();
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
					GlobalProgressWindowPtr->Progress.Perform(PBM_SETPOS, 95, 0);
					GlobalProgressWindowPtr->Progress.Refresh();

					if (ProgressTaskBarPtr) ProgressTaskBarPtr->SetPosition(95);
				}

				return fastCall<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}

			void ProgressWindow::sub3(uint32_t nPartId, LPCSTR lpcstrText)
			{
				if (GlobalProgressWindowPtr->isOpen)
				{
					//GlobalProgressWindowPtr->ProgressLabel.Caption = lpcstrText;

					char* EndPref = nullptr;
					auto step = strtol(lpcstrText + 14, &EndPref, 10);

					if (value_ProgressWindow_pos < step)
					{
						for (auto i = 0; i < (step - value_ProgressWindow_pos); i++)
						{
							GlobalProgressWindowPtr->Progress.Perform(PBM_STEPIT, 0, 0);
							if (ProgressTaskBarPtr) ProgressTaskBarPtr->Step();
						}
			
						value_ProgressWindow_pos = step;
						GlobalProgressWindowPtr->Progress.Refresh();
					}
				}

				return fastCall<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}
		}
	}
}