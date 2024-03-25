// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "NavMeshWindow.h"
#include "Editor API/SSE/BGSRenderWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			NavMeshWindow* GlobalNavMeshWindowPtr = nullptr;

			bool NavMeshWindow::HasOption() const
			{
				return false;
			}

			bool NavMeshWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* NavMeshWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* NavMeshWindow::GetName() const
			{
				return "NavMesh Window";
			}

			bool NavMeshWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> NavMeshWindow::GetDependencies() const
			{
				return {};
			}

			bool NavMeshWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_SKYRIM_SE_LAST;
			}

			bool NavMeshWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), &HKWndProc);
	
					return true;
				}

				return false;
			}

			bool NavMeshWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			NavMeshWindow::NavMeshWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalNavMeshWindowPtr);
				GlobalNavMeshWindowPtr = this;
			}

			LRESULT CALLBACK NavMeshWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG) 
				{
					GlobalNavMeshWindowPtr->m_hWnd = Hwnd;
					return CallWindowProc(GlobalNavMeshWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
				}
				else if ((Message == WM_COMMAND) && (LOWORD(wParam) == 40770))
				{
					if (MessageBoxA(0, "Do you really want to produce a balance to optimize navmesh?", "Confirmation",
						MB_YESNO | MB_ICONQUESTION) != IDYES)
						return S_OK;
				}

				return CallWindowProc(GlobalNavMeshWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}