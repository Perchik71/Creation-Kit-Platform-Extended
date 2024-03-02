// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BSString.h"
#include "Editor API/EditorUI.h"
#include "Editor API/SSE/BGSRenderWindow.h"
#include "Patches/Windows/SSE/MainWindow.h"
#include "CellViewWindow.h"

#define UI_CELL_VIEW_ADD_CELL_ITEM					2579
#define UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM			2583
#define UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX			2580	
#define UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX	2582	
#define UI_CELL_VIEW_SELECT_CELL_OBJECTS_CHECKBOX	5665	
#define UI_CELL_VIEW_VISIBLE_CELL_OBJECTS_CHECKBOX	5666	
#define UI_CELL_VIEW_FILTER_CELL					2584	

#define UI_CELL_VIEW_FILTER_CELL_SIZE				1024

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CellViewWindow* GlobalCellViewWindowPtr = nullptr;
			//uintptr_t pointer_CellViewWindow_data = 0;
			uintptr_t pointer_CellViewWindow_sub1 = 0;
			uintptr_t pointer_CellViewWindow_sub2 = 0;
			uintptr_t pointer_CellViewWindow_sub3 = 0;
			char* str_CellViewWindow_Filter = nullptr;
			char* str_CellViewWindow_FilterUser = nullptr;

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
				return true;
			}

			Array<String> CellViewWindow::GetDependencies() const
			{
				return { "Main Window", "Render Window" };
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
					//pointer_CellViewWindow_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&sub1); // Allow forms to be filtered in EditorUI_CellViewProc
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&sub1); // ^
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub2); // ^

					pointer_CellViewWindow_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));
					pointer_CellViewWindow_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));

					// Hook event add/rem from pick
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(7), (uintptr_t)&sub3);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(9), (uintptr_t)&sub3);
					pointer_CellViewWindow_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(8));

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

			__int64 CellViewWindow::sub3(__int64 a1, __int64 a2, __int64 a3)
			{
				auto Ret = fastCall<__int64>(pointer_CellViewWindow_sub3, a1, a2, a3);

				if (GlobalCellViewWindowPtr)
				{
					auto Handle = GlobalCellViewWindowPtr->Handle;
					
					auto SelOnly = static_cast<bool>(GetPropA(Handle, EditorAPI::EditorUI::UI_USER_DATA_SELECT_OBJECT_ONLY));
					if (SelOnly)
						// Fake a filter text box change
						SendMessageA(Handle, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);

					auto VisOnly = static_cast<bool>(GetPropA(Handle, EditorAPI::EditorUI::UI_USER_DATA_VISIBLE_OBJECT_ONLY));
					if (VisOnly)
						// Fake a filter text box change
						SendMessageA(Handle, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
				}

				return Ret;
			}

			void CellViewWindow::ResizeWnd(UINT width, UINT height)
			{
				UINT uHalf = (width - 12) >> 1;
				Classes::CRECT Bounds;

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
				m_VisibleObjectsOnly.BoundsRect = Bounds;

				Bounds.Top = 67;
				m_SelectObjectsOnly.BoundsRect = Bounds;

				Bounds.Left += 100;
				Bounds.Width = 160;
				m_ActiveObjectsOnly.BoundsRect = Bounds;
			}

			LRESULT CALLBACK CellViewWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					str_CellViewWindow_Filter = (char*)GlobalMemoryManagerPtr->MemAlloc(UI_CELL_VIEW_FILTER_CELL);
					AssertMsg(str_CellViewWindow_Filter, "Failed to allocate memory for the cells filter");
					str_CellViewWindow_FilterUser = (char*)GlobalMemoryManagerPtr->MemAlloc(UI_CELL_VIEW_FILTER_CELL);
					AssertMsg(str_CellViewWindow_FilterUser, "Failed to allocate memory for the text of the custom filter");

					GlobalCellViewWindowPtr->m_hWnd = Hwnd;
					GlobalCellViewWindowPtr->m_WorldSpaceLabel = GetDlgItem(Hwnd, 1164);
					GlobalCellViewWindowPtr->m_WorldSpaceComboBox = GetDlgItem(Hwnd, 2083);
					GlobalCellViewWindowPtr->m_XEdit = GetDlgItem(Hwnd, 5283);
					GlobalCellViewWindowPtr->m_YEdit = GetDlgItem(Hwnd, 5099);
					GlobalCellViewWindowPtr->m_XLabel = GetDlgItem(Hwnd, 5281);
					GlobalCellViewWindowPtr->m_YLabel = GetDlgItem(Hwnd, 5282);
					GlobalCellViewWindowPtr->m_GoButton = GetDlgItem(Hwnd, 3681);
					GlobalCellViewWindowPtr->m_LoadedAtTop = GetDlgItem(Hwnd, 5662);
					GlobalCellViewWindowPtr->m_IdEdit = GetDlgItem(Hwnd, 2581);
					GlobalCellViewWindowPtr->m_SelectedObjectLabel = GetDlgItem(Hwnd, 1163);
					GlobalCellViewWindowPtr->m_ActiveCellsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELLS_CHECKBOX);
					GlobalCellViewWindowPtr->m_ActiveObjectsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_ACTIVE_CELL_OBJECTS_CHECKBOX);
					GlobalCellViewWindowPtr->m_SelectObjectsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_SELECT_CELL_OBJECTS_CHECKBOX);
					GlobalCellViewWindowPtr->m_VisibleObjectsOnly = GetDlgItem(Hwnd, UI_CELL_VIEW_VISIBLE_CELL_OBJECTS_CHECKBOX);			
					GlobalCellViewWindowPtr->m_CellListView = GetDlgItem(Hwnd, 1155);
					GlobalCellViewWindowPtr->m_ObjectListView = GetDlgItem(Hwnd, 1156);
					GlobalCellViewWindowPtr->m_FilterCellEdit = GetDlgItem(Hwnd, UI_CELL_VIEW_FILTER_CELL);

					GlobalCellViewWindowPtr->m_WorldSpaceLabel.Style |= SS_CENTER;

					// Eliminate the flicker when changing cells
					ListView_SetExtendedListViewStyleEx(GlobalCellViewWindowPtr->m_CellListView.Handle, 
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					ListView_SetExtendedListViewStyleEx(GlobalCellViewWindowPtr->m_ObjectListView.Handle,
						LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

					SendMessage(GlobalCellViewWindowPtr->m_VisibleObjectsOnly.Handle, BM_SETCHECK, BST_CHECKED, 0);
					SetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_VISIBLE_OBJECT_ONLY, reinterpret_cast<HANDLE>(true));
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
					else if (param == UI_CELL_VIEW_SELECT_CELL_OBJECTS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_SELECT_OBJECT_ONLY, reinterpret_cast<HANDLE>(enableFilter));

						// Fake a filter text box change
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
						return 1;
					}
					else if (param == UI_CELL_VIEW_VISIBLE_CELL_OBJECTS_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_VISIBLE_OBJECT_ONLY, reinterpret_cast<HANDLE>(enableFilter));

						// Fake a filter text box change
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2581, EN_CHANGE), 0);
						return 1;
					}
					else if ((param == UI_CELL_VIEW_FILTER_CELL) && (HIWORD(wParam) == EN_CHANGE))
					{
						auto hFilter = GlobalCellViewWindowPtr->m_FilterCellEdit.Handle;
						auto iLen = std::min(GetWindowTextLengthA(hFilter), UI_CELL_VIEW_FILTER_CELL_SIZE - 1);

						SetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_FILTER_CELLS_LEN, reinterpret_cast<HANDLE>(iLen));

						if (iLen)
							GetWindowTextA(hFilter, str_CellViewWindow_FilterUser, iLen + 1);

						// Fake the dropdown list being activated
						SendMessageA(Hwnd, WM_COMMAND, MAKEWPARAM(2083, 1), 0);
						return 1;
					}
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_ITEM)
				{
					auto form = reinterpret_cast<const TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);
					*allowInsert = true;

					if (!form)
						return 1;

					// Skip the entry if "Show only active cells" is checked
					if (static_cast<bool>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_ACTIVE_CELLS_ONLY)))
						*allowInsert = form->Active;

					// Skip if a filter is installed and the form does not meet the requirements
					if (*allowInsert && reinterpret_cast<int>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_FILTER_CELLS_LEN)) > 2)
					{
						auto editorID = form->GetEditorID_NoVTable();
						if (editorID)
						{
							sprintf_s(str_CellViewWindow_Filter, UI_CELL_VIEW_FILTER_CELL, "%s %08X %s",
								editorID, form->FormID, form->FullName);
						
							*allowInsert = StrStrI(str_CellViewWindow_Filter, str_CellViewWindow_FilterUser) != 0;
						}
					}

					return 1;
				}
				else if (Message == UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM)
				{
					auto form = reinterpret_cast<const TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);
					*allowInsert = true;

					if (!form)
						return 1;

					// Skip the entry if "Show only active objects" is checked
					if (static_cast<bool>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_ACTIVE_OBJECT_ONLY)))
						*allowInsert = form->Active;

					// Skip the entry if "Visible Only" is checked
					if (*allowInsert && static_cast<bool>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_VISIBLE_OBJECT_ONLY)))
					{
						auto Node = ((TESObjectREFR*)(form))->GetFadeNode();
						if (Node && (Node->QAppCulled() || Node->QNotVisible()))
							*allowInsert = false;
					}

					// Skip the entry if "Selected Only" is checked
					if (*allowInsert && static_cast<bool>(GetPropA(Hwnd, EditorAPI::EditorUI::UI_USER_DATA_SELECT_OBJECT_ONLY)))
					{
						auto Renderer = BGSRenderWindow::Singleton.Singleton;
						if (Renderer)
						{
							if (!Renderer->PickHandler->Has((TESObjectREFR*)form))
								*allowInsert = false;
						}
					}

					return 1;
				}
				else if (Message == WM_CLOSE)
				{
					//
					// hide the window and uncheck the checkbox in the main window menu
					//
					GlobalCellViewWindowPtr->Visible = false;
					GlobalMainWindowPtr->MainMenu.GetSubMenuItem(2).GetItem(EditorAPI::EditorUI::UI_EDITOR_TOGGLECELLVIEW).Checked = false;

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
				else if (Message == WM_NOTIFY)
				{
					if (((LPNMHDR)lParam)->code == HDN_ITEMCHANGED)
					{
						// Bethesda удаляет столбцы, после их восстанавливает и считывает файла параметров их ширину.
						// Естественно это приводит к потере ширины столбца всегда при открытии новой ячейки.
						// Сохраним их в файл параметров, как только будут изменены.

						LPNMHEADERA pNMHeader = (LPNMHEADERA)lParam;
						auto ListView = GetParent(pNMHeader->hdr.hwndFrom);
						if (ListView == GlobalCellViewWindowPtr->m_CellListView.Handle)
						{
							auto ColWidth = ListView_GetColumnWidth(ListView, pNMHeader->iItem);

							EditorAPI::BSString OptionName;
							OptionName.Format("Cell View Cell List Column %d width", pNMHeader->iItem);

							WritePrivateProfileStringA("General", OptionName.c_str(),
								EditorAPI::BSString::Transforms::IntToStr(ColWidth).c_str(), 
								(EditorAPI::BSString::Utils::GetApplicationPath() + "CreationKitPrefs.ini").c_str());
						}
					}
				}

				return CallWindowProc(GlobalCellViewWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}