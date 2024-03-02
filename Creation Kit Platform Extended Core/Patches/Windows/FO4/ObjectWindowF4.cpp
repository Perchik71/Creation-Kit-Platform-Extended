// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "UITheme/VarCommon.h"
#include "Editor API/EditorUI.h"
#include "Editor API/FO4/BGSRenderWindow.h"
#include "ObjectWindowF4.h"

#define UI_OBJECT_WINDOW_CHECKBOX			6329
#define UI_OBJECT_WINDOW_ADD_ITEM			2579
#define UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW	(WM_USER + 34400)

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			OBJWNDS ObjectWindows;
			ObjectWindow* GlobalObjectWindowBasePtr = nullptr;
			uintptr_t pointer_ObjectWindow_sub = 0;

			void ResizeObjectWndChildControls(LPOBJWND lpObjWnd)
			{
				// The perfectionist in me is dying....

				lpObjWnd->Controls.TreeList.LockUpdate();
				lpObjWnd->Controls.ItemList.LockUpdate();
				lpObjWnd->Controls.EditFilter.LockUpdate();
				lpObjWnd->Controls.ToggleDecompose.LockUpdate();
				lpObjWnd->Controls.BtnObjLayout.LockUpdate();

				auto WndRect = lpObjWnd->ObjectWindow.ClientRect();

				LONG w_btns = lpObjWnd->Controls.BtnObjLayout.Width;
				LONG w_tree = lpObjWnd->Controls.TreeList.Width;
				LONG w_left = w_tree - w_btns + 1;
				lpObjWnd->Controls.BtnObjLayout.Left = w_left;
				lpObjWnd->Controls.ToggleDecompose.Left = w_left;
				lpObjWnd->Controls.ActiveOnly.Width = w_tree;

				w_left = w_tree - lpObjWnd->Controls.EditFilter.Left - w_btns - 3;
				lpObjWnd->Controls.EditFilter.Width = w_left;
				lpObjWnd->Controls.ComboLayout.Width = w_left;

				auto TopT = lpObjWnd->Controls.TreeList.Top;

				lpObjWnd->Controls.ItemList.Left = w_tree + 5;
				lpObjWnd->Controls.ItemList.Width = WndRect.Width - (w_tree + 5);
				lpObjWnd->Controls.ItemList.Height = WndRect.Height - 3;
				lpObjWnd->Controls.TreeList.Height = WndRect.Height - TopT;
				lpObjWnd->Controls.Spliter.Height = WndRect.Height - TopT;
				
				// fix bad pic
				auto handle = lpObjWnd->ObjectWindow.Handle;
				RECT r = { 0, 0, lpObjWnd->Controls.ItemList.Left, TopT };
				InvalidateRect(handle, &r, TRUE);
				UpdateWindow(handle);

				lpObjWnd->Controls.BtnObjLayout.UnlockUpdate();
				lpObjWnd->Controls.ToggleDecompose.UnlockUpdate();
				lpObjWnd->Controls.EditFilter.UnlockUpdate();
				lpObjWnd->Controls.ItemList.UnlockUpdate();
				lpObjWnd->Controls.TreeList.UnlockUpdate();
				lpObjWnd->Controls.EditFilter.Repaint();
				lpObjWnd->Controls.BtnObjLayout.Repaint();
				lpObjWnd->Controls.ToggleDecompose.Repaint();
			}

			void SplitterResizeObjectWndChildControls(LPOBJWND lpObjWnd)
			{
				lpObjWnd->Controls.TreeList.LockUpdate();
				lpObjWnd->Controls.ItemList.LockUpdate();
				lpObjWnd->Controls.EditFilter.LockUpdate();
				lpObjWnd->Controls.ToggleDecompose.LockUpdate();
				lpObjWnd->Controls.BtnObjLayout.LockUpdate();

				auto WndRect = lpObjWnd->ObjectWindow.ClientRect();

				LONG w_btns = lpObjWnd->Controls.BtnObjLayout.Width;
				LONG w_tree = lpObjWnd->Controls.TreeList.Width;
				LONG w_left = w_tree - w_btns + 1;
				lpObjWnd->Controls.BtnObjLayout.Left = w_left;
				lpObjWnd->Controls.ToggleDecompose.Left = w_left;
				lpObjWnd->Controls.ActiveOnly.Width = w_tree;

				w_left = w_tree - lpObjWnd->Controls.EditFilter.Left - w_btns - 3;
				lpObjWnd->Controls.EditFilter.Width = w_left;
				lpObjWnd->Controls.ComboLayout.Width = w_left;

				auto TopT = lpObjWnd->Controls.TreeList.Top;

				lpObjWnd->Controls.ItemList.Left = w_tree + 5;
				lpObjWnd->Controls.ItemList.Width = WndRect.Width - (w_tree + 5);

				// fix bad pic
				auto handle = lpObjWnd->ObjectWindow.Handle;
				RECT r = { 0, 0, lpObjWnd->Controls.ItemList.Left, TopT };
				InvalidateRect(handle, &r, TRUE);
				UpdateWindow(handle);

				lpObjWnd->Controls.BtnObjLayout.UnlockUpdate();
				lpObjWnd->Controls.ToggleDecompose.UnlockUpdate();
				lpObjWnd->Controls.EditFilter.UnlockUpdate();
				lpObjWnd->Controls.ItemList.UnlockUpdate();
				lpObjWnd->Controls.TreeList.UnlockUpdate();

				lpObjWnd->Controls.EditFilter.Repaint();
				lpObjWnd->Controls.BtnObjLayout.Repaint();
				lpObjWnd->Controls.ToggleDecompose.Repaint();
			}

			bool ObjectWindow::HasOption() const
			{
				return false;
			}

			bool ObjectWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ObjectWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* ObjectWindow::GetName() const
			{
				return "Object Window";
			}

			bool ObjectWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> ObjectWindow::GetDependencies() const
			{
				return {};
			}

			bool ObjectWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool ObjectWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)),
							(uintptr_t)&HKWndProc);
					
					// Fix resize ObjectWindowProc
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&HKMoveWindow);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 0x46);

					// Allow forms to be filtered in ObjectWindowProc
					pointer_ObjectWindow_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool ObjectWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			BOOL WINAPI ObjectWindow::HKMoveWindow(HWND hWindow, INT32 X, INT32 Y, INT32 nWidth, INT32 nHeight, BOOL bRepaint)
			{
				BOOL bResult = MoveWindow(hWindow, X, Y, nWidth, nHeight, bRepaint);

				if (auto iterator = ObjectWindows.find(GetParent(hWindow)); iterator != ObjectWindows.end())
				{
					LPOBJWND lpObjWnd = (*iterator).second;
					if (lpObjWnd) lpObjWnd->ObjectWindow.Perform(WM_COMMAND, UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW, 0);
				}

				return bResult;
			}

			int ObjectWindow::sub(__int64 ObjectListInsertData, TESForm* Form)
			{
				const HWND objectWindowHandle = GetParent((*(HWND*)(ObjectListInsertData + 0x18)));

				bool allowInsert = true;
				SendMessageA(objectWindowHandle, UI_OBJECT_WINDOW_ADD_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 1;

				return ((int(__fastcall*)(__int64, TESForm*))pointer_ObjectWindow_sub)
					(ObjectListInsertData, Form);
			}

			ObjectWindow::ObjectWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				GlobalObjectWindowBasePtr = this;
			}

			void ObjectWindow::SetObjectWindowFilter(LPOBJWND lpObjWnd, const char* name,
				const bool SkipText, const bool actived)
			{
				if (!SkipText)
					lpObjWnd->Controls.EditFilter.Caption = name;

				//lpObjWnd->Controls.ActiveOnly.Checked = actived;
				// Force the list items to update as if it was by timer
				//lpObjWnd->ObjectWindow.Perform(WM_TIMER, 0x1B58, 0);
				// This is a bit slower but works in multi windows
				lpObjWnd->Controls.EditFilter.Caption = lpObjWnd->Controls.EditFilter.Caption;
			}

			INT_PTR CALLBACK ObjectWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					LPOBJWND lpObjWnd = new OBJWND;

					lpObjWnd->ObjectWindow = Hwnd;

					lpObjWnd->Controls.TreeList = lpObjWnd->ObjectWindow.GetControl(2093);
					lpObjWnd->Controls.ItemList = lpObjWnd->ObjectWindow.GetControl(1041);
					lpObjWnd->Controls.ToggleDecompose = lpObjWnd->ObjectWindow.GetControl(6027);
					lpObjWnd->Controls.BtnObjLayout = lpObjWnd->ObjectWindow.GetControl(6025);
					lpObjWnd->Controls.ComboLayout = lpObjWnd->ObjectWindow.GetControl(6024);
					lpObjWnd->Controls.EditFilter = lpObjWnd->ObjectWindow.GetControl(2581);
					lpObjWnd->Controls.Spliter = lpObjWnd->ObjectWindow.GetControl(2157);
					lpObjWnd->Controls.ActiveOnly = GetDlgItem(Hwnd, UI_OBJECT_WINDOW_CHECKBOX);

					// Eliminate the flicker when changing categories
					ListView_SetExtendedListViewStyleEx(lpObjWnd->Controls.ItemList.Handle, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
					// Eliminate the flicker when changing size trees
					SendMessage(lpObjWnd->Controls.TreeList.Handle, TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
					// 
					ListView_SetExtendedListViewStyleEx(lpObjWnd->Controls.ItemList.Handle, LVS_EX_INFOTIP, LVS_EX_INFOTIP);

					// Erase Icon and SysMenu
					if (!ObjectWindows.size())
						lpObjWnd->ObjectWindow.Style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME;
					else
						lpObjWnd->ObjectWindow.Style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;	

					ObjectWindows.emplace(Hwnd, lpObjWnd);
				}
				// Don't let us reduce the window too much
				else if (Message == WM_GETMINMAXINFO)
				{
					LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
					lpMMI->ptMinTrackSize.x = 350;
					lpMMI->ptMinTrackSize.y = 200;

					return S_OK;
				}
				else if (Message == WM_ERASEBKGND) {
					if (auto iterator = ObjectWindows.find(Hwnd);  iterator != ObjectWindows.end())
					{
						LPOBJWND lpObjWnd = (*iterator).second;
						if (lpObjWnd) {
							HDC dc = (HDC)wParam;
							auto Rect = lpObjWnd->Controls.ItemList.BoundsRect;
							ExcludeClipRect(dc, Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
							Rect = lpObjWnd->Controls.TreeList.BoundsRect;
							ExcludeClipRect(dc, Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
							Rect = lpObjWnd->Controls.ActiveOnly.BoundsRect;
							ExcludeClipRect(dc, Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
							Rect = lpObjWnd->Controls.EditFilter.BoundsRect;
							ExcludeClipRect(dc, Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);

							HBRUSH brush;

							if (UITheme::IsDarkTheme())
								brush = UITheme::Comctl32GetSysColorBrush(COLOR_BTNFACE);
							else
								brush = GetSysColorBrush(COLOR_BTNFACE);

							RECT rc;
							GetClipBox(dc, &rc);
							FillRect(dc, &rc, brush);

							return 1;
						}
					}
				}
				else if (Message == WM_SIZE)
				{
					if (auto iterator = ObjectWindows.find(Hwnd);  iterator != ObjectWindows.end())
					{
						LPOBJWND lpObjWnd = (*iterator).second;
						if (lpObjWnd) {
							ResizeObjectWndChildControls(lpObjWnd);
							return 0;
						}
					}
				}
				else if (Message == WM_COMMAND)
				{
					const uint32_t param = LOWORD(wParam);
					if (param == UI_OBJECT_WINDOW_CHECKBOX)
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, "ActiveOnly", reinterpret_cast<HANDLE>(enableFilter));
						
						if (auto iterator = ObjectWindows.find(Hwnd); iterator != ObjectWindows.end())
						{
							LPOBJWND lpObjWnd = (*iterator).second;
							if (lpObjWnd) SetObjectWindowFilter(lpObjWnd, "", TRUE, 
								!lpObjWnd->Controls.ActiveOnly.Checked);
						}
	
						return S_OK;
					}
					else if (param == UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW) {
						if (auto iterator = ObjectWindows.find(Hwnd); iterator != ObjectWindows.end())
						{
							LPOBJWND lpObjWnd = (*iterator).second;
							if (lpObjWnd) SplitterResizeObjectWndChildControls(lpObjWnd);
						}
						return S_OK;
					}
					else if (param == EditorAPI::EditorUI::UI_EDITOR_CHANGEBASEFORM)
					{
						auto Renderer = BGSRenderWindow::Singleton.Singleton;
						if (Renderer && Renderer->PickHandler)
						{
							// get the desired form from the selected list
							auto ItemList = GetDlgItem(Hwnd, 1041);
							Assert(ItemList);
							auto ItemCount = ListView_GetSelectedCount(ItemList);
							if (ItemCount != 1)
								MessageBoxA(0, "You have too many forms selected in the Object Window or not selected at all.\n"
									"1 item must be selected.", "Error", MB_OK | MB_ICONERROR);
							auto Pick = Renderer->PickHandler;
							if (Pick->Count != 1)
								MessageBoxA(0, "1 object must be selected in Render Window, but no more.",
									"Error", MB_OK | MB_ICONERROR);
							else
							{
								// get the Ref selected on the render window
								auto Ref = (TESObjectREFR*)(Pick->Items->First->Ref);
								Assert(Ref);

								auto Form = (TESForm*)EditorAPI::EditorUI::ListViewGetSelectedItem(ItemList);
								Assert(Form);

								// Replace the parent form and update
								TESObjectREFR::SetParentWithRedraw(Ref, Form);
								// Fix no mark change
								Ref->MarkAsChanged();
							}
						}

						return S_OK;
					}
				}
				else if (Message == UI_OBJECT_WINDOW_ADD_ITEM)
				{
					auto form = reinterpret_cast<const EditorAPI::Fallout4::TESForm*>(wParam);
					auto allowInsert = reinterpret_cast<bool*>(lParam);

					*allowInsert = true;

					// Skip the entry if "Show only active forms" is checked
					if (static_cast<bool>(GetPropA(Hwnd, "ActiveOnly")))
					{
						if (form && !form->Active)
							*allowInsert = false;
					}

					return 1;
				}
				else if (Message == WM_SHOWWINDOW)
				{
					if (auto iterator = ObjectWindows.find(Hwnd); iterator != ObjectWindows.end())
					{
						LPOBJWND lpObjWnd = (*iterator).second;
						if (lpObjWnd) ResizeObjectWndChildControls(lpObjWnd);
					}
				}
				else if (Message == WM_DESTROY)
				{
					LPOBJWND lpObjWnd = ObjectWindows.at(Hwnd);
					if (lpObjWnd)
					{
						ObjectWindows.erase(Hwnd);

						delete lpObjWnd;
						lpObjWnd = NULL;
					}
				}

				return CallWindowProc(GlobalObjectWindowBasePtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}