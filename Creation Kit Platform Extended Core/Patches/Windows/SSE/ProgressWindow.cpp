// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/ProgressTaskBar.h"
#include "Editor API/EditorUI.h"
#include "Patches/UIThemePatch.h"
#include "MainWindow.h"
#include "ProgressWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
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
				return eEditorCurrentVersion <= EDITOR_SKYRIM_SE_LAST;
			}

			bool ProgressWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc = voltek::detours_function_class_jump(_RELDATA_ADDR(0), &HKWndProc);
					pointer_ProgressWindow_sub = _RELDATA_ADDR(1);

					// Hook Loading Files...Initializing...
					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&sub1);

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

						if (GlobalMainWindowPtr)
						{
							ProgressTaskBarPtr = new ProgressTaskBar(GlobalMainWindowPtr->Handle, 100 * 4);
							if (ProgressTaskBarPtr)
							{
								ProgressTaskBarPtr->Begin();
								// default more often than not, we see uncertain progress
								ProgressTaskBarPtr->SetMarquee(true);
							}
						}
					}
					break;
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
					break;
				}

				return CallWindowProc(GlobalProgressWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void ProgressWindow::sub1(uint32_t nPartId, LPCSTR lpcstrText)
			{
				if (GlobalProgressWindowPtr->isOpen && ProgressTaskBarPtr)
					ProgressTaskBarPtr->SetMarquee(true);

				return fastCall<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}

			void ProgressWindow::step()
			{
				if (GlobalProgressWindowPtr->isOpen && ProgressTaskBarPtr)
				{
					ProgressTaskBarPtr->SetMarquee(false);
					ProgressTaskBarPtr->Step();
				}
			}
		}
	}
}