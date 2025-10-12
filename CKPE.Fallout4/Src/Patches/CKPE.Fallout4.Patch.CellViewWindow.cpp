// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.MainWindow.h>
#include <Patches/CKPE.Fallout4.Patch.CellViewWindow.h>

#include <commctrl.h>
#include <shlwapi.h>

#define UI_CELL_VIEW_ADD_CELL_ITEM					2579
#define UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM			2583
#define UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX			2580	
#define UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX	2582	
#define UI_CELL_VIEW_FILTER_CELL					2584	
#define UI_CELL_VIEW_GO_BUTTON						3681	

#define UI_CELL_VIEW_FILTER_CELL_SIZE				1024

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_CellViewWindow_sub1 = 0;
			std::uintptr_t pointer_CellViewWindow_sub2 = 0;
			char* str_CellViewWindow_Filter = nullptr;
			char* str_CellViewWindow_FilterUser = nullptr;

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
				return true;
			}

			std::vector<std::string> CellViewWindow::GetDependencies() const noexcept(true)
			{
				return { "Main Window", "Render Window" };
			}

			bool CellViewWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool CellViewWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				// Allow forms to be filtered in CellViewProc
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&CellViewWindow::sub1);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&CellViewWindow::sub1);
				pointer_CellViewWindow_sub1 = __CKPE_OFFSET(4);

				if ((verPatch == 2))
				{
					// Allow objects to be filtered in CellViewProc
					Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&CellViewWindow::sub2_ver2);
					pointer_CellViewWindow_sub2 = __CKPE_OFFSET(5);
				}
				else
				{
					// Allow objects to be filtered in CellViewProc
					Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&CellViewWindow::sub2);
					pointer_CellViewWindow_sub2 = __CKPE_OFFSET(5);
				}

				return true;
			}

			void CellViewWindow::sub1(HWND ListViewHandle, EditorAPI::Forms::TESForm* Form, 
				bool UseImage, std::int32_t ItemIndex) noexcept(true)
			{
				bool allowInsert = true;
				SendMessageA(GetParent(ListViewHandle), UI_CELL_VIEW_ADD_CELL_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return;

				fast_call<void>(pointer_CellViewWindow_sub1, ListViewHandle, Form, UseImage, ItemIndex);
			}

			std::int32_t CellViewWindow::sub2(HWND** ListViewHandle, EditorAPI::Forms::TESForm** Form, 
				std::int64_t a3) noexcept(true)
			{
				bool allowInsert = true;
				SendMessageA(GetParent(**ListViewHandle), UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM, (WPARAM)*Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 1;

				return fast_call<std::int32_t>(pointer_CellViewWindow_sub2, *ListViewHandle, Form);
			}

			void CellViewWindow::sub2_ver2(HWND ListViewHandle, EditorAPI::Forms::TESForm* Form) noexcept(true)
			{
				bool allowInsert = true;
				SendMessageA(ListViewHandle, UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return;

				fast_call<void>(pointer_CellViewWindow_sub2, ListViewHandle, Form);
			}

			void CellViewWindow::ResizeWnd(std::uint32_t width, std::uint32_t height) noexcept(true)
			{
				std::uint32_t uHalf = (width - 12) >> 1;
				CKPE::CRECT Bounds;

				Bounds.Left = 4;
				Bounds.Top = 90;
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

				Bounds.Left = 156;
				Bounds.Top = 22;
				Bounds.Height = 21;
				Bounds.Right = uHalf + 4;
				m_FilterCellEdit.BoundsRect = Bounds;

				Bounds.Left = 6;
				Bounds.Top = 60;
				Bounds.Height = 12;
				Bounds.Width = 10;
				m_XLabel.BoundsRect = Bounds;

				Bounds.Left = 64;
				m_YLabel.BoundsRect = Bounds;

				Bounds.Left = 18;
				Bounds.Top = 56;
				Bounds.Width = 40;
				Bounds.Height = 21;
				m_XEdit.BoundsRect = Bounds;

				Bounds.Left = 76;
				m_YEdit.BoundsRect = Bounds;

				Bounds.Top = 50;
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

				Bounds.Top = 90;
				Bounds.Left = uHalf + 8;
				Bounds.Width = uHalf;
				Bounds.Height = height - (Bounds.Top + 4);
				m_ObjectListView.BoundsRect = Bounds;

				Bounds.Top = 22;
				Bounds.Height = 21;
				m_IdEdit.BoundsRect = Bounds;

				Bounds.Top = 4;
				Bounds.Height = 18;
				m_SelectedObjectLabel.BoundsRect = Bounds;
				m_SelectedObjectLabel.Refresh();

				Bounds.Top = 50;
				Bounds.Height = 16;
				Bounds.Width = 95;
				m_FilteredOnly.BoundsRect = Bounds;

				Bounds.Top = 50;
				Bounds.Height = 16;
				Bounds.Width = 95;
				m_FilteredOnly.BoundsRect = Bounds;

				Bounds.Top = 67;
				m_SelectObjectsOnly.BoundsRect = Bounds;

				Bounds.Top = 50;
				Bounds.Left += 100;
				Bounds.Width = 160;
				m_VisibleObjectsOnly.BoundsRect = Bounds;

				Bounds.Top = 67;
				m_ActiveObjectsOnly.BoundsRect = Bounds;
			}

			void CellViewWindow::UpdateCellList() noexcept(true)
			{
				if (!lock)
					// Fake the dropdown list being activated
					SendMessageA(Handle, WM_COMMAND, MAKEWPARAM(2083, 1), 0);
			}

			void CellViewWindow::UpdateObjectList() noexcept(true)
			{
				if (!lock)
					// Fake a filter text box change
					SendMessageA(Handle, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
			}

			LRESULT CALLBACK CellViewWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				if (Message == WM_INITDIALOG)
				{
					str_CellViewWindow_Filter = (char*)EditorAPI::NiAPI::NiMemoryManager::Alloc(nullptr, UI_CELL_VIEW_FILTER_CELL);
					CKPE_ASSERT_MSG(str_CellViewWindow_Filter, "Failed to allocate memory for the cells filter");
					str_CellViewWindow_FilterUser = (char*)EditorAPI::NiAPI::NiMemoryManager::Alloc(nullptr, UI_CELL_VIEW_FILTER_CELL);
					CKPE_ASSERT_MSG(str_CellViewWindow_FilterUser, "Failed to allocate memory for the text of the custom filter");

					auto __This = CellViewWindow::Singleton.GetSingleton();

					__This->m_hWnd = Hwnd;

					__This->m_WorldSpaceLabel = GetDlgItem(Hwnd, 1164);
					__This->m_WorldSpaceComboBox = GetDlgItem(Hwnd, 2083);
					__This->m_XEdit = GetDlgItem(Hwnd, 5283);
					__This->m_YEdit = GetDlgItem(Hwnd, 5099);
					__This->m_XLabel = GetDlgItem(Hwnd, 5281);
					__This->m_YLabel = GetDlgItem(Hwnd, 5282);
					__This->m_GoButton = GetDlgItem(Hwnd, UI_CELL_VIEW_GO_BUTTON);
					__This->m_LoadedAtTop = GetDlgItem(Hwnd, 5662);
					__This->m_FilteredOnly = GetDlgItem(Hwnd, 5664);
					__This->m_VisibleObjectsOnly = GetDlgItem(Hwnd, 5666);
					__This->m_SelectObjectsOnly = GetDlgItem(Hwnd, 5665);
					__This->m_IdEdit = GetDlgItem(Hwnd, 2581);
					__This->m_SelectedObjectLabel = GetDlgItem(Hwnd, 1163);
					__This->m_ActiveCellsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX);
					__This->m_ActiveObjectsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX);
					__This->m_CellListView = GetDlgItem(Hwnd, 1155);
					__This->m_ObjectListView = GetDlgItem(Hwnd, 1156);
					__This->m_FilterCellEdit = GetDlgItem(Hwnd, UI_CELL_VIEW_FILTER_CELL);

					__This->m_WorldSpaceLabel.Style |= SS_CENTER;

					// Eliminate the flicker when changing cells
					ListView_SetExtendedListViewStyleEx(__This->m_CellListView.Handle,
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					ListView_SetExtendedListViewStyleEx(__This->m_ObjectListView.Handle,
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
				}
				else if (Message == WM_SIZE)
				{
					CellViewWindow::Singleton->ResizeWnd(LOWORD(lParam), HIWORD(lParam));
					return S_OK;
				}
				else if (Message == WM_COMMAND)
				{
					const std::uint32_t param = LOWORD(wParam);

					if (param == UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY, reinterpret_cast<HANDLE>(enableFilter));
						CellViewWindow::Singleton->UpdateCellList();
						return 1;
					}
					else if (param == UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY, reinterpret_cast<HANDLE>(enableFilter));
						CellViewWindow::Singleton->UpdateObjectList();
						return 1;
					}
					else if ((param == UI_CELL_VIEW_FILTER_CELL) && (HIWORD(wParam) == EN_CHANGE))
					{
						auto hFilter = CellViewWindow::Singleton->m_FilterCellEdit.Handle;
						auto iLen = std::min(GetWindowTextLengthA(hFilter), UI_CELL_VIEW_FILTER_CELL_SIZE - 1);

						SetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_FILTER_CELLS_LEN, reinterpret_cast<HANDLE>(iLen));

						if (iLen)
							GetWindowTextA(hFilter, str_CellViewWindow_FilterUser, iLen + 1);

						CellViewWindow::Singleton->UpdateCellList();
						return 1;
					}
					else if (param == UI_CELL_VIEW_GO_BUTTON)
					{
						if ((CellViewWindow::Singleton->m_XEdit.GetCaption().length() <= 1) ||
							(CellViewWindow::Singleton->m_YEdit.GetCaption().length() <= 1))
							return 1;
					}
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_ITEM)
				{
					auto form = reinterpret_cast<const EditorAPI::Forms::TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);
					*allowInsert = true;

					if (!form)
						return 1;

					// Skip the entry if "Show only active cells" is checked
					if (static_cast<bool>(GetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY)))
						*allowInsert = form->Active;

					// Skip if a filter is installed and the form does not meet the requirements
					if (*allowInsert && 
						reinterpret_cast<std::int32_t>(GetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_FILTER_CELLS_LEN)) > 2)
					{
						auto editorID = form->EditorID;
						if (editorID)
						{
							sprintf_s(str_CellViewWindow_Filter, UI_CELL_VIEW_FILTER_CELL, "%s %08X %s",
								editorID, form->FormID, form->FullName);

							*allowInsert = StrStrIA(str_CellViewWindow_Filter, str_CellViewWindow_FilterUser) != 0;
						}
					}

					return 1;
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM)
				{
					auto form = reinterpret_cast<const EditorAPI::Forms::TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);
					*allowInsert = true;

					if (!form)
						return 1;

					// Skip the entry if "Show only active objects" is checked
					if (static_cast<bool>(GetPropA(Hwnd, Common::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY)))
						*allowInsert = form->Active;

					return 1;
				}
				else if (Message == WM_CLOSE)
				{
					//
					// hide the window and uncheck the checkbox in the main window menu
					//
					CellViewWindow::Singleton->Visible = false;
					MainWindow::Singleton->MainMenu.GetSubMenuItem(2).GetItem(Common::EditorUI::UI_EDITOR_TOGGLECELLVIEW).Checked = false;

					return S_OK;
				}
				// Don't let us reduce the window too much
				else if (Message == WM_GETMINMAXINFO)
				{
					if (lParam)
					{
						LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
						lpMMI->ptMinTrackSize.x = 369;
						lpMMI->ptMinTrackSize.y = 157;
					}

					return S_OK;
				}

				return CallWindowProc(CellViewWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}