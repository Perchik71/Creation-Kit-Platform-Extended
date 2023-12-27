// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CellViewWindow.h"

#define UI_CELL_VIEW_ADD_CELL_ITEM					2579
#define UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM			2583
#define UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX			2580	
#define UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX	2582	

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CellViewWindow* GlobalCellViewWindowPtr = nullptr;
			uintptr_t pointer_CellViewWindow_data = 0;
			uintptr_t pointer_CellViewWindow_sub1 = 0;
			uintptr_t pointer_CellViewWindow_sub2 = 0;

			bool CellViewWindow::HasOption() const
			{
				return false;
			}

			bool CellViewWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CellViewWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* CellViewWindow::GetName() const
			{
				return "Cell View Window";
			}

			bool CellViewWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> CellViewWindow::GetDependencies() const
			{
				return {};
			}

			bool CellViewWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_SKYRIM_SE_LAST;
			}

			bool CellViewWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), &HKWndProc);
					pointer_CellViewWindow_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&sub1); // Allow forms to be filtered in EditorUI_CellViewProc
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&sub1); // ^
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub2); // ^

					pointer_CellViewWindow_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));
					pointer_CellViewWindow_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));

					return true;
				}

				return false;
			}

			bool CellViewWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			CellViewWindow::CellViewWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalCellViewWindowPtr);
				GlobalCellViewWindowPtr = this;
			}

			void CellViewWindow::sub1(HWND ListViewHandle, TESForm* Form, bool UseImage, int ItemIndex)
			{
				bool allowInsert = true;
				SendMessageA(GetParent(ListViewHandle), UI_CELL_VIEW_ADD_CELL_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return;

				((void(__fastcall*)(HWND, TESForm*, bool, int))pointer_CellViewWindow_sub1)(ListViewHandle, Form, UseImage, ItemIndex);
			}

			int CellViewWindow::sub2(HWND** ListViewHandle, TESForm** Form, __int64 a3)
			{
				bool allowInsert = true;
				SendMessageA(GetParent(**ListViewHandle), UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM, (WPARAM)*Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 1;

				return ((int(__fastcall*)(HWND*, TESForm**))pointer_CellViewWindow_sub2)(*ListViewHandle, Form);
			}

			LRESULT CALLBACK CellViewWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					GlobalCellViewWindowPtr->m_hWnd = Hwnd;

					// Eliminate the flicker when changing cells
					ListView_SetExtendedListViewStyleEx(GetDlgItem(Hwnd, 1155), LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					ListView_SetExtendedListViewStyleEx(GetDlgItem(Hwnd, 1156), LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

					ShowWindow(GetDlgItem(Hwnd, 1007), SW_HIDE);
				}
				else if (Message == WM_SIZE)
				{
					auto labelRect = reinterpret_cast<RECT*>(pointer_CellViewWindow_data);
					auto handle = GetDlgItem(Hwnd, 1164);

					// Fix the "World Space" label positioning on window resize
					RECT label;
					GetClientRect(handle, &label);

					RECT rect;
					GetClientRect(GetDlgItem(Hwnd, 2083), &rect);

					int ddMid = rect.left + ((rect.right - rect.left) / 2);
					int labelMid = (label.right - label.left) / 2;

					SetWindowPos(handle, nullptr, ddMid - (labelMid / 2), labelRect->top, 0, 0, SWP_NOSIZE);

					// Force the dropdown to extend the full length of the column
					labelRect->right = 0;

					auto result = CallWindowProc(GlobalCellViewWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);

					Classes::CUIBaseControl editSearchObjs = GetDlgItem(Hwnd, 2581);
					Classes::CUIBaseControl listObjs = GetDlgItem(Hwnd, 1156);
					Classes::CUIBaseControl listCheckActiveObjs = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX);

					auto left = listObjs.Left;
					auto width = listObjs.Width;
					listCheckActiveObjs.Left = left;
					listCheckActiveObjs.Width = width;
					editSearchObjs.Left = left;
					editSearchObjs.Width = width;

					return result;
				}
				else if (Message == WM_COMMAND)
				{
					const uint32_t param = LOWORD(wParam);

					if (param == UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, "ActiveCellsOnly", reinterpret_cast<HANDLE>(enableFilter));

						// Fake the dropdown list being activated
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2083, 1), 0);
						return 1;
					}
					else if (param == UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, "ActiveObjectsOnly", reinterpret_cast<HANDLE>(enableFilter));

						// Fake a filter text box change
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
						return 1;
					}
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_ITEM)
				{
					auto form = reinterpret_cast<const TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);

					*allowInsert = true;

					// Skip the entry if "Show only active cells" is checked
					if (static_cast<bool>(GetPropA(Hwnd, "ActiveCellsOnly")))
					{
						if (form && !form->GetActive())
							*allowInsert = false;
					}

					return 1;
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM)
				{
					auto form = reinterpret_cast<const TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);

					*allowInsert = true;

					// Skip the entry if "Show only active objects" is checked
					if (static_cast<bool>(GetPropA(Hwnd, "ActiveObjectsOnly")))
					{
						if (form && !form->GetActive())
							*allowInsert = false;
					}

					return 1;
				}
				// Don't let us reduce the window too much
				else if (Message == WM_GETMINMAXINFO) 
				{
					if (lParam) 
					{
						LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
						lpMMI->ptMinTrackSize.x = 738;
						lpMMI->ptMinTrackSize.y = 315;
					}

					return S_OK;
				}

				return CallWindowProc(GlobalCellViewWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}