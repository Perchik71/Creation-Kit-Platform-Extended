// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.CellViewWindow.h>

#include <commctrl.h>
#include <shlwapi.h>

#define UI_CELL_VIEW_ADD_CELL_ITEM					2579
#define UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM			2583
#define UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX			2580	
#define UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX	2582	
#define UI_CELL_VIEW_FILTER_CELL					7433	
#define UI_CELL_VIEW_FILTER_WORLDSPACE				7376	
#define UI_CELL_VIEW_GO_BUTTON						3681	

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			std::uintptr_t pointer_CellViewWindow_sub1 = 0;
			std::uintptr_t pointer_CellViewWindow_sub2 = 0;
			std::uintptr_t pointer_CellViewWindow_sub3 = 0;

			CellViewWindow::CellViewWindow() : Common::PatchBaseWindow()
			{
				SetName("Cell View Window");
				Singleton = this;
			}

			bool CellViewWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CellViewWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CellViewWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CellViewWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CellViewWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool CellViewWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), &HKWndProc);

				// Some kind of mockery, create a dialog, then create a "splitter" window in it. 
				// Was it really hard to do it in resources? I cut out all this stuff.
				// Delete the separator, delete everything that changes the windows in the dialog.
				SafeWrite::Write(__CKPE_OFFSET(1), { 0xC3, 0x90 });
				// No create splitter.
				SafeWrite::Write(__CKPE_OFFSET(2), { 0xEB });
				// Remove change windows size.
				SafeWrite::Write(__CKPE_OFFSET(3), { 0xC3, 0x90 });

				// Allow forms to be filtered in CellViewProc
				//Detours::DetourCall(__CKPE_OFFSET(4), &CellViewWindow::sub1);
				//Detours::DetourCall(__CKPE_OFFSET(5), &CellViewWindow::sub1);
				//pointer_CellViewWindow_sub1 = __CKPE_OFFSET(6);
				// For Recent Cells
				//Detours::DetourCall(__CKPE_OFFSET(10), &CellViewWindow::sub3);
				//Detours::DetourCall(__CKPE_OFFSET(11), &CellViewWindow::sub3);
				//pointer_CellViewWindow_sub3 = __CKPE_OFFSET(9);
				// Allow objects to be filtered in CellViewProc
				//Detours::DetourCall(__CKPE_OFFSET(7), &CellViewWindow::sub2);
				//pointer_CellViewWindow_sub2 = __CKPE_OFFSET(8);

				return true;
			}

			void CellViewWindow::ResizeWnd(UINT width, UINT height) noexcept(true)
			{
				UINT uHalf = (width - 12) >> 1;
				CKPE::CRECT Bounds;

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

			LRESULT CALLBACK CellViewWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				if (Message == WM_INITDIALOG)
				{
					auto __This = CellViewWindow::Singleton.GetSingleton();

					__This->m_hWnd = Hwnd;
					__This->m_CellListView = GetDlgItem(Hwnd, 1155);
					__This->m_ObjectListView = GetDlgItem(Hwnd, 1156);
					__This->m_FilterCellEdit = GetDlgItem(Hwnd, UI_CELL_VIEW_FILTER_CELL);
					__This->m_XEdit = GetDlgItem(Hwnd, 5283);
					__This->m_YEdit = GetDlgItem(Hwnd, 5099);
					__This->m_XLabel = GetDlgItem(Hwnd, 5281);
					__This->m_YLabel = GetDlgItem(Hwnd, 5282);
					__This->m_GoButton = GetDlgItem(Hwnd, UI_CELL_VIEW_GO_BUTTON);
					__This->m_WorldSpaceLabel = GetDlgItem(Hwnd, 1164);
					__This->m_LoadedAtTop = GetDlgItem(Hwnd, 5662);
					__This->m_ActiveCellsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX);
					__This->m_WorldSpaceComboBox = GetDlgItem(Hwnd, 2083);
					__This->m_FilterWorldspaceEdit = GetDlgItem(Hwnd, UI_CELL_VIEW_FILTER_WORLDSPACE);
					__This->m_UnloadButton = GetDlgItem(Hwnd, 1008);
					__This->m_IdEdit = GetDlgItem(Hwnd, 2581);
					__This->m_FilterCellLabel = GetDlgItem(Hwnd, 1222);
					__This->m_FilteredOnly = GetDlgItem(Hwnd, 5664);
					__This->m_VisibleOnly = GetDlgItem(Hwnd, 5666);
					__This->m_SelectedOnly = GetDlgItem(Hwnd, 5665);
					__This->m_ActiveLayerOnly = GetDlgItem(Hwnd, 5667);
					__This->m_ShowNavmesh = GetDlgItem(Hwnd, 148);
					__This->m_SpecificFormTypes = GetDlgItem(Hwnd, 5668);
					__This->m_ActiveObjectsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX);
					__This->m_SelectTypesButton = GetDlgItem(Hwnd, 5959);

					// Eliminate the flicker when changing cells
					ListView_SetExtendedListViewStyleEx(__This->m_CellListView.Handle,
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					ListView_SetExtendedListViewStyleEx(__This->m_ObjectListView.Handle,
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
				}
				else if (Message == WM_SIZE)
				{
					CellViewWindow::Singleton.GetSingleton()->ResizeWnd(LOWORD(lParam), HIWORD(lParam));
					return S_OK;
				}
				else if (Message == WM_COMMAND)
				{
					const uint32_t param = LOWORD(wParam);

					if (param == UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY, reinterpret_cast<HANDLE>(enableFilter));

						// Fake the dropdown list being activated
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2083, 1), 0);
						return 1;
					}
					else if (param == UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY, reinterpret_cast<HANDLE>(enableFilter));

						// Fake a filter text box change
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
						return 1;
					}
					else if (param == UI_CELL_VIEW_GO_BUTTON)
					{
						if ((CellViewWindow::Singleton.GetSingleton()->m_XEdit.GetCaption().length() <= 1) ||
							(CellViewWindow::Singleton.GetSingleton()->m_YEdit.GetCaption().length() <= 1))
							return 1;
					}
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_ITEM)
				{
					//auto form = reinterpret_cast<const TESForm*>(wParam);
					//auto allowInsert = reinterpret_cast<bool*>(lParam);

					//*allowInsert = true;

					//// Skip the entry if "Show only active cells" is checked
					//if (static_cast<bool>(GetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY)))
					//{
					//	if (form && !form->Active)
					//		*allowInsert = false;
					//}

					return 1;
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM)
				{
					//auto form = reinterpret_cast<const TESForm*>(wParam);
					//auto allowInsert = reinterpret_cast<bool*>(lParam);

					//*allowInsert = true;

					//// Skip the entry if "Show only active objects" is checked
					//if (static_cast<bool>(GetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY)))
					//{
					//	if (form && !form->Active)
					//		*allowInsert = false;
					//}

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

				return CallWindowProc(CellViewWindow::Singleton.GetSingleton()->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}