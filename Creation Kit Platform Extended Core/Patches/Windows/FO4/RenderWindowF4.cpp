// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include "RenderWindowF4.h"
#include "MainWindowF4.h"
#include "Editor API/FO4/TESF4.h"
#include "Editor API/FO4/BGSRenderWindow.h"
#include "UITheme/VarCommon.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI::Fallout4;

			RenderWindow* GlobalRenderWindowPtr = nullptr;
			RenderWindow::Area rcSafeDrawArea;

			uintptr_t pointer_RenderWindow_Mov_data[11];

			bool RenderWindow::HasOption() const
			{
				return false;
			}

			bool RenderWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RenderWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* RenderWindow::GetName() const
			{
				return "Render Window";
			}

			bool RenderWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> RenderWindow::GetDependencies() const
			{
				return {};
			}

			bool RenderWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool RenderWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						voltek::detours_function_class_jump(_RELDATA_ADDR(0), (uintptr_t)&HKWndProc);
					_TempDrawArea = (Area*)_RELDATA_ADDR(1);
					TESUnknown::Instance = (TESUnknown*)_RELDATA_ADDR(2);
					BGSRenderWindow::Singleton = _RELDATA_ADDR(3);
				
					for (uint32_t i = 4; i < lpRelocationDatabaseItem->Count() && i < 14; i++)
						pointer_RenderWindow_Mov_data[i - 4] = _RELDATA_ADDR(i);
					
					BGSRenderWindow::Settings::Movement::FlagsSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[0];
					BGSRenderWindow::Settings::Movement::SnapGridValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[1];
					BGSRenderWindow::Settings::Movement::SnapAngleValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[2];
					BGSRenderWindow::Settings::Movement::ArrowSnapValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[3];
					BGSRenderWindow::Settings::Movement::ObjectRotateValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[4];
					BGSRenderWindow::Settings::Movement::ObjectMoveValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[5];
					BGSRenderWindow::Settings::Movement::CameraRotateValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[6];
					BGSRenderWindow::Settings::Movement::CameraZoomValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[7];
					BGSRenderWindow::Settings::Movement::CameraZoomOrthoValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[8];
					BGSRenderWindow::Settings::Movement::CameraPanValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[9];
					BGSRenderWindow::Settings::Movement::LandspaceMultValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[10];

					return true;
				}

				return false;
			}

			bool RenderWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			RenderWindow::RenderWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalRenderWindowPtr);
				GlobalRenderWindowPtr = this;
			}

			LRESULT CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						GlobalRegistratorWindowPtr->RegisterMajor(Hwnd, "RenderWindow");
						GlobalRenderWindowPtr->m_hWnd = Hwnd;
						return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
					}
					// Don't let us reduce the window too much
					case WM_GETMINMAXINFO:
					{
						if (lParam)
						{
							LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
							lpMMI->ptMinTrackSize.x = 96;	// 96 min tex size
							lpMMI->ptMinTrackSize.y = 96;
						}
						return S_OK;
					}
					case WM_ACTIVATE:
					{
						// Fix bug loss of window size

						if (LOWORD(wParam) == WA_INACTIVE)
							rcSafeDrawArea = *_TempDrawArea;
						else
							*_TempDrawArea = rcSafeDrawArea;
					}
					break;
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}