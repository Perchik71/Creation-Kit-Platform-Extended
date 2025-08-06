// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.UIListView.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.SkyrimSE.Patch.DataWindow.h>
#include "..\CKPE.Common\resource.h"

#include <richedit.h>
#include <commctrl.h>

#define UI_DATA_DIALOG_PLUGINLISTVIEW		1056
#define UI_DATA_DIALOG_FILTERBOX			1003	// See: resource.rc
#define UI_LISTVIEW_PLUGINS					UI_DATA_DIALOG_PLUGINLISTVIEW
#define UI_EDIT_SEARCH_PLUGIN_BY_NAME		52004
#define UI_NEW_LISTVIEW_CONTROL_TO_RESULT	52005

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool DataWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (uintptr_t)&HKWndProc);

				return true;
			}

			INT_PTR CALLBACK DataWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					auto _This = DataWindow::Singleton.GetSingleton();

					INT_PTR result = CallWindowProc(_This->GetOldWndProc(), Hwnd, Message, wParam, lParam);
					HWND pluginListHandle = GetDlgItem(Hwnd, UI_LISTVIEW_PLUGINS);

					_This->m_hWnd = Hwnd;
					_This->m_pluginList = pluginListHandle;

					_This->Style = WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU;

					// Subscribe to notifications when the user types in the filter text box
					SendMessageA(GetDlgItem(Hwnd, UI_DATA_DIALOG_FILTERBOX), EM_SETEVENTMASK, 0, ENM_CHANGE);

					// Prevent flickering & adjust width to fit file names
					ListView_SetExtendedListViewStyleEx(pluginListHandle, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					ListView_SetColumnWidth(pluginListHandle, 0, 250);

					// Create two separate list view groups: one for default items and one for hidden (filtered) items. This has to be run
					// after WM_INITDIALOG because list views can't have groups with no items present.
					LVGROUP defaultGroup
					{
						.cbSize = sizeof(LVGROUP),
						.mask = LVGF_GROUPID,
						.iGroupId = VisibleGroupId,
					};

					LVGROUP hiddenGroup
					{
						.cbSize = sizeof(LVGROUP),
						.mask = LVGF_GROUPID | LVGF_STATE,
						.iGroupId = FilteredGroupId,
						.stateMask = LVGS_HIDDEN,
						.state = LVGS_HIDDEN,
					};

					ListView_InsertGroup(pluginListHandle, -1, &defaultGroup);
					ListView_InsertGroup(pluginListHandle, -1, &hiddenGroup);

					if (Common::UI::IsDarkTheme())
						_This->m_pluginList.SetStyle(_This->m_pluginList.GetStyle() | LVS_OWNERDRAWFIXED);

					// Bethesda probably doesn't know about the existence of Check. 
					// They have created icons that mimic pictorially for the user.
					// I completely take everything from there, although I'm not happy about it, but this is a ready-made mechanism, and I'm just trying to make a search in it.
					HIMAGELIST hImageList = ListView_GetImageList(pluginListHandle, LVSIL_SMALL);
					ImageList_Destroy(hImageList);

					_This->m_ImageList.ReCreate(16, 16, true, ilct24Bit);

					HINSTANCE hModule = (HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL();

					if (Common::UI::IsDarkTheme())
					{
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

					// fix no checked in list 
					RedrawWindow(pluginListHandle, NULL, NULL, RDW_UPDATENOW | RDW_NOCHILDREN);

					return result;
				}
				else if (Message == WM_COMMAND)
				{
					// Text boxes use WM_COMMAND instead of WM_NOTIFY. Why? Well, who knows.
					if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == UI_DATA_DIALOG_FILTERBOX)
					{
						HWND pluginListHandle = GetDlgItem(Hwnd, UI_LISTVIEW_PLUGINS);
						char filter[1024] = {};

						GetWindowTextA(reinterpret_cast<HWND>(lParam), filter, static_cast<int>(std::ssize(filter)));

						if (strlen(filter) <= 0)
							// No filtering
							SendMessageA(pluginListHandle, LVM_ENABLEGROUPVIEW, FALSE, 0);
						else
						{
							SendMessageA(pluginListHandle, LVM_ENABLEGROUPVIEW, TRUE, 0);

							// Iterate over each item in the list, compare its file name text, then assign it to the relevant group
							int itemCount = ListView_GetItemCount(pluginListHandle);

							for (int i = 0; i < itemCount; i++)
							{
								char itemText[MAX_PATH] = {};

								LVITEMA getItem
								{
									.mask = LVIF_TEXT,
									.iItem = i,
									.iSubItem = 0,
									.pszText = itemText,
									.cchTextMax = static_cast<int>(std::ssize(itemText)),
								};

								ListView_GetItem(pluginListHandle, &getItem);

								// Case insensitive strstr
								bool isVisible = [&]()
									{
										for (auto c = getItem.pszText; *c != '\0'; c++)
										{
											if (_strnicmp(c, filter, strlen(filter)) == 0)
												return true;
										}

										return false;
									}();

								LVITEMA setItem
								{
									.mask = LVIF_GROUPID,
									.iItem = i,
									.iGroupId = isVisible ? VisibleGroupId : FilteredGroupId,
								};

								ListView_SetItem(pluginListHandle, &setItem);
							}
						}

						return 1;
					}
				}

				return CallWindowProc(DataWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void DataWindow::DoListViewDraw(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem,
				const char* lpstrFileName, const char* lpstrTypeName) noexcept(true)
			{
				CRECT rc = lpDrawItem->rcItem;
				Canvas canvas(lpDrawItem->hDC);

				auto type = EditorAPI::TESFile::GetTypeFile(
					(EditorAPI::BSString::Utils::GetRelativeDataPath() + lpstrFileName).Get());
				if ((type & EditorAPI::TESFile::FILE_RECORD_ESM) == EditorAPI::TESFile::FILE_RECORD_ESM)
					canvas.FillWithTransparent(rc, RGB(255, 0, 0), 10);
				else if ((type & EditorAPI::TESFile::FILE_RECORD_ESL) == EditorAPI::TESFile::FILE_RECORD_ESL)
					canvas.FillWithTransparent(rc, RGB(0, 255, 0), 10);
			}
		}
	}
}