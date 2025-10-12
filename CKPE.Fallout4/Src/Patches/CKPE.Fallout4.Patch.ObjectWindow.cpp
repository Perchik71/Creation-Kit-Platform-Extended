﻿// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Utils.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.UIListView.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.ObjectWindow.h>
#include <commctrl.h>

#define UI_OBJECT_WINDOW_CHECKBOX			6329
#define UI_OBJECT_WINDOW_ADD_ITEM			2579
#define UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW	(WM_USER + 34400)

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			OBJWNDS ObjectWindows;
			ObjectWindow* GlobalObjectWindowBasePtr = nullptr;
			std::uintptr_t pointer_ObjectWindow_sub = 0;

			static void ResizeObjectWndChildControls(LPOBJWND lpObjWnd) noexcept(true)
			{
				// The perfectionist in me is dying....

				lpObjWnd->Controls.TreeList.LockUpdate();
				lpObjWnd->Controls.ItemList.LockUpdate();
				lpObjWnd->Controls.EditFilter.LockUpdate();
				lpObjWnd->Controls.ToggleDecompose.LockUpdate();
				lpObjWnd->Controls.BtnObjLayout.LockUpdate();

				auto WndRect = lpObjWnd->ObjectWindow.ClientRect();

				// Fix https://www.nexusmods.com/fallout4/images/274405
				lpObjWnd->Controls.TreeList.Left = 1;

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

			static void SplitterResizeObjectWndChildControls(LPOBJWND lpObjWnd) noexcept(true)
			{
				lpObjWnd->Controls.TreeList.LockUpdate();
				lpObjWnd->Controls.ItemList.LockUpdate();
				lpObjWnd->Controls.EditFilter.LockUpdate();
				lpObjWnd->Controls.ToggleDecompose.LockUpdate();
				lpObjWnd->Controls.BtnObjLayout.LockUpdate();

				auto WndRect = lpObjWnd->ObjectWindow.ClientRect();

				// Fix https://www.nexusmods.com/fallout4/images/274405
				lpObjWnd->Controls.TreeList.Left = 1;

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

			ObjectWindow::ObjectWindow() : Common::PatchBaseWindow()
			{
				SetName("Object Window");
				GlobalObjectWindowBasePtr = this;
			}

			bool ObjectWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ObjectWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ObjectWindow::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> ObjectWindow::GetDependencies() const noexcept(true)
			{
				return { "Render Window" };
			}

			bool ObjectWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool ObjectWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				if (verPatch == 1)
				{
					// Allow forms to be filtered in ObjectWindowProc
					pointer_ObjectWindow_sub = __CKPE_OFFSET(3);
					Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&sub);

					// Fix resize ObjectWindowProc
					Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&HKMoveWindow);
					SafeWrite::WriteNop(__CKPE_OFFSET(2), 0x46);
				}
				else
				{
					pointer_ObjectWindow_sub = __CKPE_OFFSET(4);
					// Restore function
					auto rva = __CKPE_OFFSET(3);
					SafeWrite::WriteNop(rva + 0x10, 0x33);
					SafeWrite::Write(rva, { 0x48, 0x8B, 0x4C, 0x24, 0x40, 0x48, 0x89, 0xFA, 0x49, 0x89, 0xF0 });
					Detours::DetourCall(rva + 0xB, (std::uintptr_t)&sub2);

					// Fix resize ObjectWindowProc
					rva = __CKPE_OFFSET(1);
					SafeWrite::WriteNop(rva, 0x4B);
					Detours::DetourCall(rva, (std::uintptr_t)&HKMoveWindow);
				}

				return true;
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

			std::int32_t ObjectWindow::sub(std::int64_t ObjectListInsertData, void* Form) noexcept(true)
			{
				const HWND objectWindowHandle = GetParent((*(HWND*)(ObjectListInsertData + 0x18)));

				bool allowInsert = true;
				SendMessageA(objectWindowHandle, UI_OBJECT_WINDOW_ADD_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 1;

				return fast_call<std::int32_t>(pointer_ObjectWindow_sub, ObjectListInsertData, Form);
			}

			std::int32_t ObjectWindow::sub2(HWND Hwnd, void* Form, const char* filterText) noexcept(true)
			{
				const HWND objectWindowHandle = GetParent(Hwnd);

				bool allowInsert = true;
				SendMessageA(objectWindowHandle, UI_OBJECT_WINDOW_ADD_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 0;

				auto EditorID = ((EditorAPI::Forms::TESForm*)Form)->EditorID;

				if (!fast_call<std::int32_t>(pointer_ObjectWindow_sub, EditorID, filterText, 0))
				{
					char szBuf[12];
					sprintf_s(szBuf, "%08X", ((EditorAPI::Forms::TESForm*)Form)->FormID);
					if (!fast_call<std::int32_t>(pointer_ObjectWindow_sub, szBuf, filterText, 0))
						return 0;
				}

				return 1;
			}

			void ObjectWindow::SetObjectWindowFilter(LPOBJWND lpObjWnd, const char* name,
				const bool SkipText, const bool actived) noexcept(true)
			{
				if (!SkipText)
					lpObjWnd->Controls.EditFilter.SetCaption(name);

				//lpObjWnd->Controls.ActiveOnly.Checked = actived;
				// Force the list items to update as if it was by timer
				//lpObjWnd->ObjectWindow.Perform(WM_TIMER, 0x1B58, 0);
				// This is a bit slower but works in multi windows
				lpObjWnd->Controls.EditFilter.SetCaption(lpObjWnd->Controls.EditFilter.GetCaption());
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
				else if (Message == WM_ERASEBKGND)
				{
					if (auto iterator = ObjectWindows.find(Hwnd);  iterator != ObjectWindows.end())
					{
						LPOBJWND lpObjWnd = (*iterator).second;
						if (lpObjWnd)
						{
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

							if (Common::UI::IsDarkTheme())
								brush = (HBRUSH)Common::UI::Comctl32GetSysColorBrush(COLOR_BTNFACE);
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
					else if (param == UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW)
					{
						if (auto iterator = ObjectWindows.find(Hwnd); iterator != ObjectWindows.end())
						{
							LPOBJWND lpObjWnd = (*iterator).second;
							if (lpObjWnd) SplitterResizeObjectWndChildControls(lpObjWnd);
						}
						return S_OK;
					}
					else if (param == Common::EditorUI::UI_EDITOR_CHANGEBASEFORM)
					{
						auto Renderer = EditorAPI::BGSRenderWindow::Singleton.Singleton;

						if (Renderer && Renderer->PickHandler)
						{
							auto Pick = Renderer->PickHandler;
							auto SelCount = Pick->Count;
							if (!SelCount)
								return S_OK;

							auto SelItem = Pick->GetItems()->GetBeginIterator();

							// get the desired form from the selected list
							auto ItemList = GetDlgItem(Hwnd, 1041);
							CKPE_ASSERT(ItemList);

							auto ItemCount = ListView_GetSelectedCount(ItemList);
							if (!ItemCount)
								return S_OK;

							if (ItemCount > 1)
							{
								MessageBoxA(0, "You have too many selected forms in the Object Window.\n"
									"Choose one thing.", "Error", MB_OK | MB_ICONERROR);
								return S_OK;
							}

							auto Form = (EditorAPI::Forms::TESForm*)Common::EditorUI::ListViewGetSelectedItem(ItemList);
							CKPE_ASSERT(Form);

							if (SelCount != 1)
							{
								auto str = std::make_unique<char[]>(120);
								sprintf_s(str.get(), 120, "Do you really want to replace base form in %u refs?", SelCount);
								if (MessageBoxA(0, str.get(), "Question", MB_YESNO | MB_ICONQUESTION) != IDYES)
									return S_OK;
							}

							for (uint32_t i = 0; i < SelCount; i++)
							{
								auto Ref = *SelItem++;
								CKPE_ASSERT(Ref);

								// Replace the parent form and update
								EditorAPI::Forms::TESObjectREFR::SetParentWithRedraw(Ref, Form);
								// Fix no mark change
								Ref->MarkAsChanged();
							}
						}

						return S_OK;
					}
				}
				else if (Message == UI_OBJECT_WINDOW_ADD_ITEM)
				{
					auto form = reinterpret_cast<const EditorAPI::Forms::TESForm*>(wParam);
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
						lpObjWnd = nullptr;
					}
				}

				return CallWindowProc(GlobalObjectWindowBasePtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}