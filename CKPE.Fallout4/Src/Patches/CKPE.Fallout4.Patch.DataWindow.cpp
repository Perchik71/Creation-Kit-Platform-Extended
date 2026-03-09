// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <windowsx.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Detours.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.UIListView.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.Fallout4.Patch.DataWindow.h>
#include "../CKPE.Common/resource.h"

#include <richedit.h>
#include <commctrl.h>

#undef MessageBox

#define UI_DATA_DIALOG_PLUGINLISTVIEW		1056
#define UI_DATA_DIALOG_FILTERBOX			1003	// See: resource.rc
#define UI_LISTVIEW_PLUGINS					UI_DATA_DIALOG_PLUGINLISTVIEW
#define UI_EDIT_SEARCH_PLUGIN_BY_NAME		52004
#define UI_NEW_LISTVIEW_CONTROL_TO_RESULT	52005

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static const char* GetAuthorPluginName(const EditorAPI::TESFile* file) noexcept(true)
			{
				if (file->IsActiveFileBlacklist(false))
					return "Bethesda Game Studios";
				return file->GetAuthorName().c_str();
			}

			void DataWindow::ListViewResultInitialize(HWND a_handle, std::uint32_t a_column) noexcept(true)
			{
				// add list columns
				LVCOLUMN lvc = { 0 };
				// Initialize the LVCOLUMN structure.
				// The mask specifies that the format, width, text, and
				// subitem members of the structure are valid.
				lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvc.fmt = LVCFMT_LEFT;
				lvc.cchTextMax = 255;
				// width of column in pixels
				lvc.cx = a_column;

				lvc.pszText = (LPSTR)"Filename";
				ListView_InsertColumn(a_handle, 0, &lvc);

				lvc.iSubItem = 1;
				lvc.pszText = (LPSTR)"Status";
				ListView_InsertColumn(a_handle, 1, &lvc);

				ListView_SetExtendedListViewStyle(a_handle, LVS_OWNERDATA | LVS_LIST | LVS_SMALLICON | LVS_SORTDESCENDING);

				// Prevent flickering & adjust width to fit file names
				ListView_SetExtendedListViewStyleEx(a_handle, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
			}

			void DataWindow::ToggleListView(const bool a_showResultListView) noexcept(true)
			{
				auto _This = DataWindow::Singleton.GetSingleton();

				auto hWnd = _This->m_pluginList.Handle;
				auto hWndResult = _This->m_pluginResultList.Handle;

				_This->m_pluginList.Visible = !a_showResultListView;
				_This->m_pluginResultList.Visible = a_showResultListView;

				if (a_showResultListView)
				{
					// The width could be changed, will repeat for list
					ListView_SetColumnWidth(hWndResult, 0, ListView_GetColumnWidth(hWnd, 0));
					ListView_SetColumnWidth(hWndResult, 1, ListView_GetColumnWidth(hWnd, 1));
				}
			}

			bool DataWindow::AppendToListViewResult(const char* sFileName, const char* sType, const bool bCheck) noexcept(true)
			{
				auto _This = DataWindow::Singleton.GetSingleton();

				HWND hWnd = _This->m_pluginResultList.Handle;
				auto iLastIndex = ListView_GetItemCount(hWnd);

				LVITEMA lvi = { 0 };
				lvi.mask = LVIF_TEXT | LVIF_IMAGE;
				lvi.cchTextMax = (INT)(strlen(sFileName) + 1);
				lvi.iItem = iLastIndex;
				lvi.pszText = (LPSTR)sFileName;
				lvi.iSubItem = 0;
				lvi.iImage = (INT)bCheck;

				if (ListView_InsertItem(hWnd, &lvi) == -1)
					return false;

				ListView_SetItemText(hWnd, iLastIndex, 1, (LPSTR)sType);

				return true;
			}

			void DataWindow::UpdateListViewResult() noexcept(true)
			{
				constexpr auto SIZEBUF = 1024;

				auto _This = DataWindow::Singleton.GetSingleton();

				INT idx, idx_safe;
				LVITEMA lvi = { 0 };
				HWND hSrchEdit, hListView, hListViewResult;
				CHAR szStrs[2][SIZEBUF]{};

				hSrchEdit = _This->m_filer.Handle;
				hListView = _This->m_pluginList.Handle;
				hListViewResult = _This->m_pluginResultList.Handle;

				ListView_DeleteAllItems(hListViewResult);

				if (Edit_GetTextLength(hSrchEdit) < 2)
				{
					ToggleListView(false);
					return;
				}

				idx_safe = -1;
				idx = -1;

				while ((idx = Common::EditorUI::ListViewFindItemByString(hListView,
					_This->m_filer.GetCaption().c_str(), idx + 1)) != -1)
				{
					if (idx_safe == -1) idx_safe = idx;
					ListView_GetItemText(hListView, idx, 0, szStrs[0], SIZEBUF);
					ListView_GetItemText(hListView, idx, 1, szStrs[1], SIZEBUF);

					lvi.mask = LVIF_IMAGE;
					lvi.iItem = idx;
					ListView_GetItem(hListView, &lvi);

					AppendToListViewResult(szStrs[0], szStrs[1], lvi.iImage != 0);
				}

				InvalidateRect(hListViewResult, nullptr, true);
				UpdateWindow(hListViewResult);

				if (!ListView_GetItemCount(hListViewResult))
				{
					Common::EditorUI::ListViewSetItemState(hListView, 0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
					ToggleListView(false);
					return;
				}

				Common::EditorUI::ListViewSelectItem(hListView, (idx_safe >= 0) ? idx_safe : 0, false);
				ToggleListView(true);
				Common::EditorUI::ListViewSelectItem(hListViewResult, 0, false);

				SendMessageA(hSrchEdit, WM_SETCURSOR, 0, 0);
			}

			DataWindow::DataWindow() : Common::PatchBaseWindow()
			{
				SetName("Data Window");
				Common::UI::ListView::InstallCustomDrawPluginsHandler(&DoListViewDraw);
				Singleton = this;
			}

			bool DataWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* DataWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool DataWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> DataWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool DataWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool DataWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				if (verPatch == 2)
				{
					auto off = __CKPE_OFFSET(1) + 7;

					SafeWrite::WriteNop(off, 0x1C);
					SafeWrite::Write(off, { 0x48, 0x89, 0xF9 });
					Detours::DetourCall(off + 3, (std::uintptr_t)&GetAuthorPluginName);
					SafeWrite::Write(off + 8, { 0x48, 0x89, 0xD9, 0xBA, 0x01, 0x04, 0x00, 0x00 });
				}

				return true;
			}

			INT_PTR CALLBACK DataWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
				case WM_INITDIALOG:
				{
					auto _This = DataWindow::Singleton.GetSingleton();

					INT_PTR result = CallWindowProc(_This->GetOldWndProc(), Hwnd, Message, wParam, lParam);
					HWND pluginListHandle = GetDlgItem(Hwnd, UI_LISTVIEW_PLUGINS);
					HWND pluginResultListHandle = GetDlgItem(Hwnd, UI_NEW_LISTVIEW_CONTROL_TO_RESULT);

					_This->m_hWnd = Hwnd;
					_This->m_filer = GetDlgItem(Hwnd, UI_DATA_DIALOG_FILTERBOX);
					_This->m_pluginList = pluginListHandle;
					_This->m_pluginResultList = pluginResultListHandle;

					_This->Style = WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU;

					// Subscribe to notifications when the user types in the filter text box
					SendMessageA(GetDlgItem(Hwnd, UI_DATA_DIALOG_FILTERBOX), EM_SETEVENTMASK, 0, ENM_CHANGE);

					// Prevent flickering & adjust width to fit file names
					ListView_SetExtendedListViewStyleEx(pluginListHandle, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

					auto Path = EditorAPI::BSString::Utils::GetApplicationPath();
					auto Column = GetPrivateProfileIntA("General", "Data Column 0 width", 250, (Path + "CreationKitPrefs.ini").c_str());
					ListView_SetColumnWidth(pluginListHandle, 0, Column);

					ListViewResultInitialize(pluginResultListHandle, Column);

					// Bethesda probably doesn't know about the existence of Check. 
					// They have created icons that mimic pictorially for the user.
					// I completely take everything from there, although I'm not happy about it, but this is a ready-made mechanism,
					// and I'm just trying to make a search in it.
					auto hImageList = ListView_GetImageList(pluginListHandle, LVSIL_SMALL);
					ImageList_Destroy(hImageList);

					_This->m_ImageList.ReCreate(16, 16, true, ilct24Bit);

					auto hModule = (HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL();

					if (Common::UI::IsDarkTheme())
					{
						_This->m_pluginList.SetStyle(_This->m_pluginList.GetStyle() | LVS_OWNERDRAWFIXED);
						_This->m_pluginResultList.SetStyle(_This->m_pluginList.GetStyle() | LVS_OWNERDRAWFIXED);

						if ((Common::UI::GetTheme() == Common::UI::Theme_Custom) && Common::UI::NeedDarkCheck())
							goto DarkCheckIcons;

						_This->m_ImageList.AddFromResourceMasked(hModule, IDB_BITMAP6, RGB(32, 32, 32));
						_This->m_ImageList.AddFromResourceMasked(hModule, IDB_BITMAP2, RGB(32, 32, 32));
					}
					else
					{
					DarkCheckIcons:
						_This->m_ImageList.AddFromResourceMasked(hModule, IDB_BITMAP5, RGB(255, 255, 255));
						_This->m_ImageList.AddFromResourceMasked(hModule, IDB_BITMAP1, RGB(255, 255, 255));
					}

					ListView_SetImageList(pluginListHandle, (HWND)(_This->m_ImageList.Handle), LVSIL_SMALL);
					ListView_SetImageList(pluginResultListHandle, (HWND)(_This->m_ImageList.Handle), LVSIL_SMALL);

					_This->m_pluginResultList.Visible = false;

					// fix no checked in list 
					RedrawWindow(pluginListHandle, nullptr, nullptr, RDW_UPDATENOW | RDW_NOCHILDREN);
					RedrawWindow(pluginResultListHandle, nullptr, nullptr, RDW_UPDATENOW | RDW_NOCHILDREN);

					return result;
				}
				case WM_DESTROY:
				{
					auto _This = DataWindow::Singleton.GetSingleton();
					auto Path = EditorAPI::BSString::Utils::GetApplicationPath();
					WritePrivateProfileStringA("General", "Data Column 0 width",
						std::to_string(ListView_GetColumnWidth(_This->m_pluginList.Handle, 0)).c_str(),
						(Path + "CreationKitPrefs.ini").c_str());
				}
				break;
				case WM_COMMAND:
				{
					// Text boxes use WM_COMMAND instead of WM_NOTIFY. Why? Well, who knows.
					if ((HIWORD(wParam) == EN_CHANGE) && (LOWORD(wParam) == UI_DATA_DIALOG_FILTERBOX))
					{
						UpdateListViewResult();
						return S_OK;
					}
					else if (LOWORD(wParam) == 1121)
					{
						auto _This = DataWindow::Singleton.GetSingleton();
						if (_This->m_pluginResultList.Visible)
						{
							constexpr auto SIZEBUF = 1024;

							CHAR szStrs[SIZEBUF] = { 0 };
							HWND hListView = _This->m_pluginList.Handle;
							HWND hListViewResult = _This->m_pluginResultList.Handle;

							auto idx = Common::EditorUI::ListViewGetSelectedItemIndex(hListViewResult);
							if (idx != -1)
							{
								ListView_GetItemText(hListViewResult, idx, 0, szStrs, SIZEBUF);
								Common::EditorUI::ListViewSelectItem(hListView,
									Common::EditorUI::ListViewFindItemByString(hListView, szStrs), false);

								auto nRes = CallWindowProc(_This->GetOldWndProc(), Hwnd, Message, wParam, lParam);
								UpdateListViewResult();
								Common::EditorUI::ListViewSelectItem(hListViewResult, idx, false);

								return nRes;
							}
						}
					}
				}
				break;
				case WM_NOTIFY:
				{
					if (LOWORD(wParam) == UI_NEW_LISTVIEW_CONTROL_TO_RESULT)
					{
						auto _This = DataWindow::Singleton.GetSingleton();
						constexpr auto SIZEBUF = 1024;

						switch (((LPNMHDR)lParam)->code)
						{
						// Select
						case NM_CLICK:
						{
							auto lpnmItem = (LPNMITEMACTIVATE)lParam;
							HWND hWnd = _This->m_pluginResultList.Handle;
							HWND hWndOld = _This->m_pluginList.Handle;
							CHAR szStrs[SIZEBUF] = { 0 };

							if (lpnmItem->iItem != -1)
							{
								ListView_GetItemText(hWnd, lpnmItem->iItem, 0, szStrs, SIZEBUF);
								Common::EditorUI::ListViewSelectItem(hWndOld, 
									Common::EditorUI::ListViewFindItemByString(hWndOld, szStrs), false);
							}
						}
						break;
						// Double-click the mouse, just check the box, do the same
						case NM_DBLCLK:
						// By double - clicking the right mouse button, 
						// I get the active mod installed in the original list of mods.
						// I model this behavior with a short list.
						case NM_RDBLCLK:
						{
							auto lpnmItem = (LPNMITEMACTIVATE)lParam;
							HWND hWnd = _This->m_pluginResultList.Handle;
							HWND hWndOld = _This->m_pluginList.Handle;
							RECT rRectItem{};
							CHAR szStrs[SIZEBUF] = { 0 };
							auto code = ((LPNMHDR)lParam)->code;

							// fix bug (can click in an empty space)
							if (lpnmItem->iItem != -1)
							{
								ListView_GetItemText(hWnd, lpnmItem->iItem, 0, szStrs, SIZEBUF);
								auto idx = Common::EditorUI::ListViewFindItemByString(hWndOld, szStrs);

								CKPE_ASSERT(idx != -1);
								CKPE_ASSERT(ListView_GetItemRect(hWndOld, idx, &rRectItem, LVIR_BOUNDS));

								NMITEMACTIVATE nmItemFake{};
								std::fill_n((std::uint8_t*)&nmItemFake, sizeof(nmItemFake), 0);

								nmItemFake.hdr.code = code;
								nmItemFake.hdr.hwndFrom = hWndOld;
								nmItemFake.hdr.idFrom = UI_LISTVIEW_PLUGINS;
								nmItemFake.iItem = idx;
								nmItemFake.ptAction.x = rRectItem.left + 5;
								nmItemFake.ptAction.y = rRectItem.top + 5;

								// fake click
								SendMessageA(Hwnd, WM_NOTIFY, UI_LISTVIEW_PLUGINS, (LPARAM)&nmItemFake);

								// update short list
								UpdateListViewResult();

								Common::EditorUI::ListViewSelectItem(hWnd, lpnmItem->iItem, false);
								Common::EditorUI::ListViewSelectItem(hWndOld, idx, false);
							}
						}
						break;
						default:
							break;
						}

						return S_OK;
					}
				}
				break;
				default:
					break;
				}

				return CallWindowProc(DataWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void DataWindow::DoListViewDraw([[maybe_unused]] HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem,
				const char* lpstrFileName, [[maybe_unused]] const char* lpstrTypeName) noexcept(true)
			{
				CRECT rc = lpDrawItem->rcItem;
				Canvas canvas(lpDrawItem->hDC);

				auto type = EditorAPI::TESFile::GetTypeFile(
					(EditorAPI::BSString::Utils::GetRelativeDataPath() + lpstrFileName).Get());

				if (((type & EditorAPI::TESFile::FILE_RECORD_ESM) == EditorAPI::TESFile::FILE_RECORD_ESM) &&
					((type & EditorAPI::TESFile::FILE_RECORD_ESL) == EditorAPI::TESFile::FILE_RECORD_ESL))
					canvas.FillWithTransparent(rc, RGB(124, 252, 0), 20);
				else if ((type & EditorAPI::TESFile::FILE_RECORD_ESM) == EditorAPI::TESFile::FILE_RECORD_ESM)
					canvas.FillWithTransparent(rc, RGB(255, 0, 0), 20);
				else if ((type & EditorAPI::TESFile::FILE_RECORD_ESL) == EditorAPI::TESFile::FILE_RECORD_ESL)
					canvas.FillWithTransparent(rc, RGB(0, 250, 154), 20);
			}
		}
	}
}