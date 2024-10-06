// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/ProgressTaskBar.h"
#include "Editor API/EditorUI.h"
#include "Patches/UIThemePatch.h"
#include "ProgressWindowSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
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
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool ProgressWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();
				if (verPatch == 1)
				{
					auto rva = (uintptr_t)(_RELDATA_RAV(0));
					lpRelocator->PatchNop(rva, 2);
					lpRelocator->DetourCall(rva + 0x27, (uintptr_t)&sub1);

					// Hook Loading Files %d%% (%s)
					lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&sub3);
					// Hook Loading Files...Initializing...
					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&sub2);
					// Hook Loading Files...Initializing References...
					lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&sub2);
					// Hook Validating forms...
					lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&sub2);

					pointer_ProgressWindow_sub = _RELDATA_ADDR(5);

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
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETRANGE, 0, MAKELPARAM(0, 100));
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETSTEP, 1, 0);
						GlobalProgressWindowPtr->Progress.Perform(PBM_SETPOS, 0, 0);
						value_ProgressWindow_pos = 0;

						ProgressTaskBarPtr = new ProgressTaskBar(Hwnd);
						if (ProgressTaskBarPtr) ProgressTaskBarPtr->Begin();
						
						ShowWindow(Hwnd, SW_SHOW);
						UpdateWindow(Hwnd);
						SetTimer(Hwnd, 0, 50, NULL);
					}
					return 0;
					case WM_TIMER:
					{
						if (wParam == 0)
							GlobalProgressWindowPtr->Progress.Refresh();
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

						KillTimer(Hwnd, 0);
					}
					return 0;
				}

				return DefWindowProc(Hwnd, Message, wParam, lParam);
			}

			HWND ProgressWindow::sub1(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent,
				DLGPROC lpDialogFunc, LPARAM dwInitParam)
			{
				auto hEvent = CreateEventA(NULL, 0, 0, NULL);		
				HWND hWindow = NULL;

				std::thread asyncLogThread([](HWND* window, HANDLE* hEvent, HINSTANCE hInstance, LPCSTR lpTemplateName, 
					HWND hWndParent, LPARAM dwInitParam)
					{
						auto darkmode = _READ_OPTION_BOOL("CreationKit", "bUIDarkTheme", false);
						if (darkmode)
							Patches::UIThemePatch::InitializeCurrentThread();

						*window = EditorAPI::EditorUI::HKCreateDialogParamA(hInstance, MAKEINTRESOURCE(3238), hWndParent,
							HKWndProc, dwInitParam);

						SetEvent(*hEvent);

						if (*window)
						{
							MSG msg;
							while (GetMessageA(&msg, NULL, 0, 0) > 0)
							{
								TranslateMessage(&msg);
								DispatchMessageA(&msg);

								if ((msg.message == WM_DESTROY) && (*window == msg.hwnd))
								{
									_CONSOLE("Exit gg");
									break;
								}
							}

							return true;
						}

						return false;
					}, &hWindow, &hEvent, hInstance, lpTemplateName, hWndParent, dwInitParam);

				asyncLogThread.detach();
				WaitForSingleObject(hEvent, 10000);
				CloseHandle(hEvent);

				_CONSOLE("TETET %u", hWindow);

				return hWindow;

				//return EditorAPI::EditorUI::HKCreateDialogParamA(hInstance, MAKEINTRESOURCE(3238), hWndParent,
				//	HKWndProc, dwInitParam);
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