// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Core/Engine.h"
#include "UITheme/VarCommon.h"
#include "Editor API/UI/UIImageList.h"
#include "DataWindow.h"

#define UI_DATA_DIALOG_PLUGINLISTVIEW		1056
#define UI_DATA_DIALOG_FILTERBOX			1003	// See: resource.rc
#define UI_LISTVIEW_PLUGINS					UI_DATA_DIALOG_PLUGINLISTVIEW
#define UI_EDIT_SEARCH_PLUGIN_BY_NAME		52004
#define UI_NEW_LISTVIEW_CONTROL_TO_RESULT	52005

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			constexpr int VisibleGroupId = 0;
			constexpr int FilteredGroupId = 1;

			DataWindow* GlobalDataWindowPtr = nullptr;
			::Core::Classes::UI::CUIBaseControl pluginList;
			::Core::Classes::UI::CUIImageList ImageList;

			bool DataWindow::HasOption() const
			{
				return false;
			}

			bool DataWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* DataWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* DataWindow::GetName() const
			{
				return "Data Window";
			}

			bool DataWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> DataWindow::GetDependencies() const
			{
				return {};
			}

			bool DataWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_SKYRIM_SE_LAST;
			}

			bool DataWindow::Activate(const Relocator* lpRelocator,
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

			bool DataWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			DataWindow::DataWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalDataWindowPtr);
				GlobalDataWindowPtr = this;
			}

			INT_PTR CALLBACK DataWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					INT_PTR result = CallWindowProc(GlobalDataWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
					HWND pluginListHandle = GetDlgItem(Hwnd, UI_LISTVIEW_PLUGINS);

					GlobalDataWindowPtr->m_hWnd = Hwnd;
					pluginList = pluginListHandle;

					GlobalDataWindowPtr->Style = WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU;

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

					if (UITheme::IsDarkTheme())
						pluginList.SetStyle(pluginList.GetStyle() | LVS_OWNERDRAWFIXED);

					// Bethesda probably doesn't know about the existence of Check. 
					// They have created icons that mimic pictorially for the user.
					// I completely take everything from there, although I'm not happy about it, but this is a ready-made mechanism, and I'm just trying to make a search in it.
					HIMAGELIST hImageList = ListView_GetImageList(pluginListHandle, LVSIL_SMALL);
					ImageList_Destroy(hImageList);

					ImageList.ReCreate(16, 16, TRUE, ::Core::Classes::UI::ilct24Bit);

					HINSTANCE hModule = Core::GlobalEnginePtr->GetInstanceDLL();

					if (UITheme::IsDarkTheme()) {
						ImageList.AddFromResource(hModule, MAKEINTRESOURCEA(IDB_BITMAP6), RGB(32, 32, 32));
						ImageList.AddFromResource(hModule, MAKEINTRESOURCEA(IDB_BITMAP2), RGB(32, 32, 32));
					}
					else {
						ImageList.AddFromResource(hModule, MAKEINTRESOURCEA(IDB_BITMAP5), RGB(255, 255, 255));
						ImageList.AddFromResource(hModule, MAKEINTRESOURCEA(IDB_BITMAP1), RGB(255, 255, 255));
					}

					ListView_SetImageList(pluginListHandle, ImageList.Handle, LVSIL_SMALL);

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

				return CallWindowProc(GlobalDataWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}