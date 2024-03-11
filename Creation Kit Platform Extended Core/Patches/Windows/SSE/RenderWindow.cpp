// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RenderWindow.h"
#include "MainWindow.h"
#include "Editor API/SSE/BGSRenderWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			bool GlobalRenderWindowInMainWindow = false;
			RenderWindow* GlobalRenderWindowPtr = nullptr;
			LONG GlobalToolbarHeight = 0;
			LONG GlobalStatusbarHeight = 0;

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
				// In version 1.6.1130, this is fixed, however, the camera is being installed
				return eEditorCurrentVersion <= EDITOR_SKYRIM_SE_LAST;
			}

			bool RenderWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), &RenderWindow::HKWndProc);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), &RenderWindow::setFlagLoadCell);
					EditorAPI::SkyrimSpectialEdition::BGSRenderWindow::Singleton = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

					return true;
				}

				return false;
			}

			bool RenderWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			RenderWindow::RenderWindow() : BaseWindow(), Classes::CUIBaseWindow(),
				_BlockInputMessage(true)
			{
				Assert(!GlobalRenderWindowPtr);
				GlobalRenderWindowPtr = this;
			}

			void RenderWindow::UpdateWindowSize(LONG w, LONG h)
			{
				if (GlobalRenderWindowInMainWindow)
				{
					::Core::Classes::UI::CRECT NewRect =
					{
						1,
						GlobalToolbarHeight,
						w - 1,
						h - (GlobalStatusbarHeight - 2)
					};

					GlobalRenderWindowPtr->SetBoundsRect(NewRect);
				}
			}

			LRESULT CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG) 
				{
					GlobalRenderWindowPtr->m_hWnd = Hwnd;
					GlobalRenderWindowPtr->_BlockInputMessage = true;
					auto Ret = CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);

					if (GlobalMainWindowPtr && _READ_OPTION_BOOL("CreationKit", "bRenderWindowInMainWindow", false))
					{
						GlobalRenderWindowInMainWindow = true;
						GlobalRenderWindowPtr->SetParent(*GlobalMainWindowPtr);
						GlobalRenderWindowPtr->Style = WS_POPUP | WS_VISIBLE | WS_OVERLAPPED;
						GlobalToolbarHeight = GlobalMainWindowPtr->Toolbar.Height;
						GlobalStatusbarHeight = GlobalMainWindowPtr->Toolbar.Height;
						auto ClientArea = GlobalMainWindowPtr->ClientRect();
						UpdateWindowSize(ClientArea.Width, ClientArea.Height);
					}

					return Ret;
				}
				////// Fix WHITE area (Eats my eyes at startup) (only 1.6.1130)
				else if (Message == WM_PAINT)
				{
					PAINTSTRUCT ps;
					HDC dc = BeginPaint(Hwnd, &ps);

					::Core::Classes::UI::CUICanvas Canvas = dc;
					Canvas.Fill(RGB(0x6A, 0x6A, 0x6A));
					
					EndPaint(Hwnd, &ps);
					return S_OK;
				}
				else if (Message == WM_ERASEBKGND)
				{
					// An application should return nonzero if it erases the background; otherwise, it should return zero.
					return S_FALSE;
				}
				///////
				else if (GlobalRenderWindowPtr->_BlockInputMessage) 
				{
					switch (Message) 
					{
					case WM_KEYUP:
					case WM_KEYDOWN:
					case WM_SYSCHAR:
					case WM_SYSKEYUP:
					case WM_SYSKEYDOWN:
					case WM_MOUSEMOVE:
					case WM_MOUSEWHEEL:
					case WM_LBUTTONUP:
					case WM_LBUTTONDOWN:
					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONUP:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONDBLCLK:
					case WM_MBUTTONUP:
					case WM_MBUTTONDOWN:
					case WM_MBUTTONDBLCLK:
						// block messages
						return 0;
					default:
						break;
					}
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void RenderWindow::setFlagLoadCell()
			{
				GlobalRenderWindowPtr->_BlockInputMessage = false;
			}
		}
	}
}