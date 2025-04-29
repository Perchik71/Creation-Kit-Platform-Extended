// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/BSString.h"
#include "Core/RegistratorWindow.h"
#include "CellViewWindowSF.h"

#define UI_CELL_VIEW_ADD_CELL_ITEM					2579
#define UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM			2583
#define UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX			2580	
#define UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX	2582	
#define UI_CELL_VIEW_FILTER_CELL					7433	
#define UI_CELL_VIEW_FILTER_WORLDSPACE				7376	
#define UI_CELL_VIEW_GO_BUTTON						3681	

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			CellViewWindow* GlobalCellViewWindowPtr = nullptr;
			uintptr_t pointer_CellViewWindow_sub1 = 0;
			uintptr_t pointer_CellViewWindow_sub2 = 0;
			uintptr_t pointer_CellViewWindow_sub3 = 0;

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
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool CellViewWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					*(uintptr_t*)&_oldWndProc = voltek::detours_function_class_jump(_RELDATA_ADDR(0), &HKWndProc);

					{
						ScopeRelocator text;

						// Some kind of mockery, create a dialog, then create a "splitter" window in it. 
						// Was it really hard to do it in resources? I cut out all this stuff.
						// Delete the separator, delete everything that changes the windows in the dialog.
						lpRelocator->Patch(_RELDATA_RAV(1), { 0xC3, 0x90 });
						// No create splitter.
						lpRelocator->Patch(_RELDATA_RAV(2), { 0xEB });
						// Remove change windows size.
						lpRelocator->Patch(_RELDATA_RAV(3), { 0xC3, 0x90 });
					}

					// Allow forms to be filtered in CellViewProc
					lpRelocator->DetourCall(_RELDATA_RAV(4), &CellViewWindow::sub1);
					lpRelocator->DetourCall(_RELDATA_RAV(5), &CellViewWindow::sub1);
					pointer_CellViewWindow_sub1 = _RELDATA_ADDR(6);
					// For Recent Cells
					lpRelocator->DetourCall(_RELDATA_RAV(10), &CellViewWindow::sub3);
					lpRelocator->DetourCall(_RELDATA_RAV(11), &CellViewWindow::sub3);
					pointer_CellViewWindow_sub3 = _RELDATA_ADDR(9);
					// Allow objects to be filtered in CellViewProc
					lpRelocator->DetourCall(_RELDATA_RAV(7), &CellViewWindow::sub2);
					pointer_CellViewWindow_sub2 = _RELDATA_ADDR(8);

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
				GlobalCellViewWindowPtr->Perform(UI_CELL_VIEW_ADD_CELL_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return;

				fastCall<void>(pointer_CellViewWindow_sub1, ListViewHandle, Form, UseImage, ItemIndex);
			}

			void CellViewWindow::sub2(HWND ListViewHandle, TESForm* Form)
			{
				{
					bool allowInsert = true;
					GlobalCellViewWindowPtr->Perform(UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

					if (!allowInsert)
						return;
				}

				fastCall<void>(pointer_CellViewWindow_sub2, ListViewHandle, Form);
			}

			bool CellViewWindow::sub3(TESForm* Form)
			{
				bool skipsInsert = fastCall<bool>(pointer_CellViewWindow_sub3, Form);
				if (!skipsInsert)
				{
					bool allowInsert = true;
					GlobalCellViewWindowPtr->Perform(UI_CELL_VIEW_ADD_CELL_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);
					if (!allowInsert)
						return true;
				}

				return false;
			}

			void CellViewWindow::ResizeWnd(UINT width, UINT height)
			{
				UINT uHalf = (width - 12) >> 1;
				Classes::CRECT Bounds;

				// CELL MAIN

				Bounds.Left = 4;
				Bounds.Top = 110;
				Bounds.Width = uHalf;
				Bounds.Height = height - (Bounds.Top + 4);
				m_CellListView.BoundsRect = Bounds;

				Bounds.Left = 4;
				Bounds.Top = 4;
				Bounds.Height = 18;
				m_WorldSpaceLabel.BoundsRect = Bounds;
				m_WorldSpaceLabel.Refresh();

				Bounds.Left = 4;
				Bounds.Top = 22;
				Bounds.Height = 95;
				Bounds.Width = 148;
				m_WorldSpaceComboBox.BoundsRect = Bounds;

				Bounds.Left = 192;
				Bounds.Top = 22;
				Bounds.Height = 21;
				Bounds.Right = uHalf + 4;
				m_FilterWorldspaceEdit.BoundsRect = Bounds;

				Bounds.Left = 60;
				Bounds.Top = 85;
				Bounds.Height = 21;
				Bounds.Right = uHalf + 4;
				m_FilterCellEdit.BoundsRect = Bounds;

				Bounds.Left = 6;
				Bounds.Top = 89;
				Bounds.Height = 17;
				Bounds.Right = 56;
				m_FilterCellLabel.BoundsRect = Bounds;

				Bounds.Left = 6;
				Bounds.Top = 57;
				Bounds.Height = 12;
				Bounds.Width = 10;
				m_XLabel.BoundsRect = Bounds;

				Bounds.Left = 64;
				m_YLabel.BoundsRect = Bounds;

				Bounds.Left = 18;
				Bounds.Top = 53;
				Bounds.Width = 40;
				Bounds.Height = 21;
				m_XEdit.BoundsRect = Bounds;

				Bounds.Left = 76;
				m_YEdit.BoundsRect = Bounds;

				Bounds.Top = 47;
				Bounds.Left = 120;
				Bounds.Width = 32;
				Bounds.Height = 32;
				m_GoButton.BoundsRect = Bounds;

				Bounds.Left = 156;
				Bounds.Height = 16;
				Bounds.Width = 85;
				m_LoadedAtTop.BoundsRect = Bounds;

				Bounds.Top += 17;
				Bounds.Width = 145;
				m_ActiveCellsOnly.BoundsRect = Bounds;

				// CELL OBJECTS

				Bounds.Top = 122;
				Bounds.Left = uHalf + 8;
				Bounds.Width = uHalf;
				Bounds.Height = height - (Bounds.Top + 4);
				m_ObjectListView.BoundsRect = Bounds;

				Bounds.Top = 22;
				Bounds.Height = 21;
				Bounds.Width = 70;
				m_UnloadButton.BoundsRect = Bounds;

				Bounds.Top = 50;
				Bounds.Height = 16;
				Bounds.Width = 100;
				m_FilteredOnly.BoundsRect = Bounds;

				Bounds.Left += 74;
				Bounds.Top = 22;
				Bounds.Height = 21;
				Bounds.Right = width - 4;
				m_IdEdit.BoundsRect = Bounds;

				Bounds.Left = uHalf + 8;
				Bounds.Width = 120;
				Bounds.Top = 67;
				m_SelectedOnly.BoundsRect = Bounds;

				Bounds.Top = 84;
				Bounds.Height = 34;
				m_SpecificFormTypes.BoundsRect = Bounds;

				Bounds.Top = 50;
				Bounds.Height = 21;
				Bounds.Left += 140;
				Bounds.Width = 120;
				m_VisibleOnly.BoundsRect = Bounds;

				Bounds.Top = 67;
				m_ActiveLayerOnly.BoundsRect = Bounds;

				Bounds.Top = 91;
				m_SelectTypesButton.BoundsRect = Bounds;

				Bounds.Top = 50;
				Bounds.Left += 140;
				Bounds.Width = 120;
				m_ShowNavmesh.BoundsRect = Bounds;
				
				Bounds.Top = 67;
				Bounds.Width = 160;
				m_ActiveObjectsOnly.BoundsRect = Bounds;
			}

			LRESULT CALLBACK CellViewWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					GlobalRegistratorWindowPtr->RegisterMajor(Hwnd, "CellViewWindow");
					GlobalCellViewWindowPtr->m_hWnd = Hwnd;
					GlobalCellViewWindowPtr->m_CellListView = GetDlgItem(Hwnd, 1155);
					GlobalCellViewWindowPtr->m_ObjectListView = GetDlgItem(Hwnd, 1156);
					GlobalCellViewWindowPtr->m_FilterCellEdit = GetDlgItem(Hwnd, UI_CELL_VIEW_FILTER_CELL);
					GlobalCellViewWindowPtr->m_XEdit = GetDlgItem(Hwnd, 5283);
					GlobalCellViewWindowPtr->m_YEdit = GetDlgItem(Hwnd, 5099);
					GlobalCellViewWindowPtr->m_XLabel = GetDlgItem(Hwnd, 5281);
					GlobalCellViewWindowPtr->m_YLabel = GetDlgItem(Hwnd, 5282);
					GlobalCellViewWindowPtr->m_GoButton = GetDlgItem(Hwnd, UI_CELL_VIEW_GO_BUTTON);
					GlobalCellViewWindowPtr->m_WorldSpaceLabel = GetDlgItem(Hwnd, 1164);
					GlobalCellViewWindowPtr->m_LoadedAtTop = GetDlgItem(Hwnd, 5662);
					GlobalCellViewWindowPtr->m_ActiveCellsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX);
					GlobalCellViewWindowPtr->m_WorldSpaceComboBox = GetDlgItem(Hwnd, 2083);
					GlobalCellViewWindowPtr->m_FilterWorldspaceEdit = GetDlgItem(Hwnd, UI_CELL_VIEW_FILTER_WORLDSPACE);
					GlobalCellViewWindowPtr->m_UnloadButton = GetDlgItem(Hwnd, 1008);
					GlobalCellViewWindowPtr->m_IdEdit = GetDlgItem(Hwnd, 2581);
					GlobalCellViewWindowPtr->m_FilterCellLabel = GetDlgItem(Hwnd, 1222);
					GlobalCellViewWindowPtr->m_FilteredOnly = GetDlgItem(Hwnd, 5664);
					GlobalCellViewWindowPtr->m_VisibleOnly = GetDlgItem(Hwnd, 5666);
					GlobalCellViewWindowPtr->m_SelectedOnly = GetDlgItem(Hwnd, 5665);
					GlobalCellViewWindowPtr->m_ActiveLayerOnly = GetDlgItem(Hwnd, 5667);
					GlobalCellViewWindowPtr->m_ShowNavmesh = GetDlgItem(Hwnd, 148);
					GlobalCellViewWindowPtr->m_SpecificFormTypes = GetDlgItem(Hwnd, 5668);
					GlobalCellViewWindowPtr->m_ActiveObjectsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX);
					GlobalCellViewWindowPtr->m_SelectTypesButton = GetDlgItem(Hwnd, 5959);

					// Eliminate the flicker when changing cells
					ListView_SetExtendedListViewStyleEx(GlobalCellViewWindowPtr->m_CellListView.Handle, 
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					ListView_SetExtendedListViewStyleEx(GlobalCellViewWindowPtr->m_ObjectListView.Handle,
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
				}
				else if (Message == WM_SIZE)
				{
					GlobalCellViewWindowPtr->ResizeWnd(LOWORD(lParam), HIWORD(lParam));
					return S_OK;
				}
				else if (Message == WM_COMMAND)
				{
					const uint32_t param = LOWORD(wParam);

					if (param == UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY, reinterpret_cast<HANDLE>(enableFilter));

						// Fake the dropdown list being activated
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2083, 1), 0);
						return 1;
					}
					else if (param == UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY, reinterpret_cast<HANDLE>(enableFilter));

						// Fake a filter text box change
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
						return 1;
					}
					else if (param == UI_CELL_VIEW_GO_BUTTON)
					{						
						if ((GlobalCellViewWindowPtr->m_XEdit.Caption.length() <= 1) ||
							(GlobalCellViewWindowPtr->m_YEdit.Caption.length() <= 1))
							return 1;
					}
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_ITEM)
				{
					auto form = reinterpret_cast<const TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);

					*allowInsert = true;

					// Skip the entry if "Show only active cells" is checked
					if (static_cast<bool>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY)))
					{
						if (form && !form->Active)
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
					if (static_cast<bool>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY)))
					{
						if (form && !form->Active)
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
						lpMMI->ptMinTrackSize.x = 369;
						lpMMI->ptMinTrackSize.y = 177;
					}

					return S_OK;
				}

				return CallWindowProc(GlobalCellViewWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}