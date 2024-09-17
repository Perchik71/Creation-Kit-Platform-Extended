// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "AboutWindowSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			AboutWindow* GlobalAboutWindowPtr = nullptr;

			static uintptr_t HKQT5_HkAboutDlgEvent()
			{
				return (uintptr_t)&AboutWindow::QT5Show;
			}

			bool AboutWindow::HasOption() const
			{
				return false;
			}

			bool AboutWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AboutWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* AboutWindow::GetName() const
			{
				return "About Window";
			}

			bool AboutWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> AboutWindow::GetDependencies() const
			{
				return {};
			}

			bool AboutWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_STARFIELD_LAST;
			}

			bool AboutWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&HKQT5_HkAboutDlgEvent);

					return true;
				}

				return false;
			}

			bool AboutWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			AboutWindow::AboutWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalAboutWindowPtr);
				GlobalAboutWindowPtr = this;
			}

			INT_PTR CALLBACK AboutWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				return CallWindowProc(GlobalAboutWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void AboutWindow::QT5Show()
			{
				EditorAPI::EditorUI::HKDialogBoxParamA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(0x64), NULL, HKWndProc, 0);
			}
		}
	}
}