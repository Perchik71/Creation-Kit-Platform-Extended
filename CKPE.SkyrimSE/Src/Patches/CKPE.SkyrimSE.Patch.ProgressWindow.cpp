// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ProgressTaskBar.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.MainWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.ProgressWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			Common::ProgressTaskBar* ProgressTaskBarPtr = nullptr;

			std::uintptr_t pointer_ProgressWindow_sub = 0;
			long value_ProgressWindow_pos = 0;

			constexpr std::uint32_t UI_PROGRESS_ID = 31007;
			constexpr std::uint32_t UI_PROGRESS_LABEL_ID = 2217;

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ProgressWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);
				pointer_ProgressWindow_sub = __CKPE_OFFSET(1);

				// Hook Loading Files...Initializing...
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub1);

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

					ProgressTaskBarPtr = new Common::ProgressTaskBar(MainWindow::Singleton->Handle, 100 * 4);
					if (ProgressTaskBarPtr)
					{
						ProgressTaskBarPtr->Begin();
						// default more often than not, we see uncertain progress
						ProgressTaskBarPtr->SetMarquee(true);
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

					ProgressWindow::Singleton->isOpen = false;
					ProgressWindow::Singleton->m_hWnd = nullptr;
					ProgressWindow::Singleton->ProgressLabel = nullptr;
					ProgressWindow::Singleton->Progress = nullptr;
				}
				break;
				}

				return CallWindowProc(ProgressWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void ProgressWindow::sub1(std::uint32_t nPartId, LPCSTR lpcstrText) noexcept(true)
			{
				if (ProgressWindow::Singleton->isOpen && ProgressTaskBarPtr)
					ProgressTaskBarPtr->SetMarquee(true);

				return fast_call<void>(pointer_ProgressWindow_sub, nPartId, lpcstrText);
			}

			void ProgressWindow::step() noexcept(true)
			{
				if (ProgressWindow::Singleton->isOpen && ProgressTaskBarPtr)
				{
					ProgressTaskBarPtr->SetMarquee(false);
					ProgressTaskBarPtr->Step();
				}
			}
		}
	}
}