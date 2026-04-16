// Copyright © 2023-2025 aka perchik71. All rights reserved.
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
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.ObjectWindow.h>
#include <commctrl.h>

#define UI_OBJECT_WINDOW_CHECKBOX			6329
#define UI_OBJECT_WINDOW_ADD_ITEM			2579
#define UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW	(WM_USER + 34400)

namespace CKPE
{
	namespace SkyrimSE
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

				auto WndRect = lpObjWnd->ObjectWindow.ClientRect();

				LONG w_tree = lpObjWnd->Controls.TreeList.Width;
				lpObjWnd->Controls.ActiveOnly.Width = w_tree;

				LONG w_left = w_tree - lpObjWnd->Controls.EditFilter.Left;
				lpObjWnd->Controls.EditFilter.Width = w_left;

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

				lpObjWnd->Controls.EditFilter.UnlockUpdate();
				lpObjWnd->Controls.ItemList.UnlockUpdate();
				lpObjWnd->Controls.TreeList.UnlockUpdate();
				lpObjWnd->Controls.EditFilter.Repaint();
			}

			static void SplitterResizeObjectWndChildControls(LPOBJWND lpObjWnd) noexcept(true)
			{
				lpObjWnd->Controls.TreeList.LockUpdate();
				lpObjWnd->Controls.ItemList.LockUpdate();
				lpObjWnd->Controls.EditFilter.LockUpdate();

				auto WndRect = lpObjWnd->ObjectWindow.ClientRect();

				LONG w_tree = lpObjWnd->Controls.TreeList.Width;
				lpObjWnd->Controls.ActiveOnly.Width = w_tree;

				LONG w_left = w_tree - lpObjWnd->Controls.EditFilter.Left;
				lpObjWnd->Controls.EditFilter.Width = w_left;

				auto TopT = lpObjWnd->Controls.TreeList.Top;

				lpObjWnd->Controls.ItemList.Left = w_tree + 5;
				lpObjWnd->Controls.ItemList.Width = WndRect.Width - (w_tree + 5);

				// fix bad pic
				auto handle = lpObjWnd->ObjectWindow.Handle;
				RECT r = { 0, 0, lpObjWnd->Controls.ItemList.Left, TopT };
				InvalidateRect(handle, &r, TRUE);
				UpdateWindow(handle);

				lpObjWnd->Controls.EditFilter.UnlockUpdate();
				lpObjWnd->Controls.ItemList.UnlockUpdate();
				lpObjWnd->Controls.TreeList.UnlockUpdate();

				lpObjWnd->Controls.EditFilter.Repaint();
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ObjectWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();

				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				// Fix resize ObjectWindowProc
				auto OffsetTotal = __CKPE_OFFSET(1);
				SafeWrite::WriteNop(OffsetTotal, 0x70);
				Detours::DetourCall(OffsetTotal, (std::uintptr_t)&HKMoveWindow);

				// In 1.6.1130 the filter is no longer needed
				if (verPatch == 1)
				{
					pointer_ObjectWindow_sub = __CKPE_OFFSET(3);
					// Allow forms to be filtered in EditorUI_ObjectWindowProc
					Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub);
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
				const std::int64_t objectWindowInstance = *(std::int64_t*)(ObjectListInsertData + 0x8) - 0x28;
				const HWND objectWindowHandle = *(HWND*)(objectWindowInstance);

				bool allowInsert = true;
				SendMessageA(objectWindowHandle, UI_OBJECT_WINDOW_ADD_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 1;

				return fast_call<std::int32_t>(pointer_ObjectWindow_sub, ObjectListInsertData, Form);
			}

			INT_PTR CALLBACK ObjectWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					auto lpObjWnd = new OBJWND;
					lpObjWnd->ObjectWindow = Hwnd;
					lpObjWnd->Controls.TreeList = lpObjWnd->ObjectWindow.GetControl(2093);
					lpObjWnd->Controls.ItemList = lpObjWnd->ObjectWindow.GetControl(1041);
					lpObjWnd->Controls.EditFilter = lpObjWnd->ObjectWindow.GetControl(2581);
					lpObjWnd->Controls.Spliter = lpObjWnd->ObjectWindow.GetControl(2157);
					lpObjWnd->Controls.ActiveOnly.CreateWnd(lpObjWnd->ObjectWindow,
						lpObjWnd->ObjectWindow.GetControl(UI_OBJECT_WINDOW_CHECKBOX), UI_OBJECT_WINDOW_CHECKBOX);

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
					auto lpMMI = (LPMINMAXINFO)lParam;
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
							auto dc = (HDC)wParam;
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

					if ((param == UI_OBJECT_WINDOW_CHECKBOX) &&
						(VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_6_438))
					{
						bool enableFilter = SendMessage(reinterpret_cast<HWND>(lParam), BM_GETCHECK, 0, 0) == BST_CHECKED;
						SetPropA(Hwnd, "ActiveOnly", reinterpret_cast<HANDLE>(enableFilter));

						// Force the list items to update as if it was by timer
						SendMessageA(Hwnd, WM_TIMER, 0x4D, 0);
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
								MessageBox::OpenError("You have too many selected forms in the Object Window.\nChoose one thing.");
								return S_OK;
							}

							auto Form = (EditorAPI::Forms::TESForm*)Common::EditorUI::ListViewGetSelectedItem(ItemList);
							CKPE_ASSERT(Form);

							if (SelCount != 1)
								if (MessageBox::OpenQuestion(std::format("Do you really want to replace base form in {} refs?"sv, SelCount)) !=
									MessageBox::mrYes)
									return S_OK;

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
					else if ((param == Common::EditorUI::UI_EDITOR_COPY_EDITOR_ID) || (param == Common::EditorUI::UI_EDITOR_COPY_FORM_ID))
					{
						auto ItemList = GetDlgItem(Hwnd, 1041);
						CKPE_ASSERT(ItemList);

						auto Form = (EditorAPI::Forms::TESForm*)Common::EditorUI::ListViewGetSelectedItem(ItemList);
						CKPE_ASSERT(Form);

						if (param == Common::EditorUI::UI_EDITOR_COPY_EDITOR_ID)
						{
							Common::EditorUI::CopyTextToClipboard(Hwnd, Form->EditorID);
						}
						else
						{
							char szBuf[16]{};
							sprintf_s(szBuf, "%08X", Form->FormID);
							Common::EditorUI::CopyTextToClipboard(Hwnd, szBuf);
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
				else if (Message == WM_NOTIFY)
				{
					// tooltips
					if (lParam && (((LPNMHDR)lParam)->code == LVN_GETINFOTIP) && (((LPNMHDR)lParam)->idFrom == 1041))
					{
						auto pGetInfoTip = (LPNMLVGETINFOTIP)lParam;
						if (pGetInfoTip->pszText && pGetInfoTip->cchTextMax)
						{
							std::fill_n((std::uint8_t*)pGetInfoTip->pszText, pGetInfoTip->cchTextMax, 0);

							LVITEMA item = { 0 };
							item.mask = LVIF_PARAM;
							item.iItem = pGetInfoTip->iItem;
							if (!ListView_GetItem(pGetInfoTip->hdr.hwndFrom, &item))
							{
							failed:
								strcpy_s(pGetInfoTip->pszText, pGetInfoTip->cchTextMax, "<FAILED>");
							}
							else
							{
								auto form = (EditorAPI::Forms::TESForm*)item.lParam;
								if (!form) goto failed;
								form->DebugInfo(pGetInfoTip->pszText, pGetInfoTip->cchTextMax);

								char szBuf[200]{};
								auto tracking = form->GetTrackingData();

								auto mods = form->GetModInfo();
								if (mods && mods->size)
								{
									if (mods->size > 3)
									{
										auto mod = form->GetDescriptionOwnerFile();
										sprintf_s(szBuf, "\n\nLast User:\t%u\nFile(s):\t\t%s",
											tracking.lastUser, mod->GetFileName().c_str());
										if (mod->IsActive()) strcat_s(szBuf, "*, ...");
										else strcat_s(szBuf, ", ...");
									}
									else
									{
										sprintf_s(szBuf, "\n\nLast User:\t%u\nFile(s):\t\t", tracking.lastUser);

										for (std::uint32_t i = 0; i < mods->size; i++)
										{
											auto mod = mods->entries[i];
											strcat_s(szBuf, mod->GetFileName().c_str());
											if (mod->IsActive()) strcat_s(szBuf, "*");
											if (i < (mods->size - 1))
												strcat_s(szBuf, ", ");
										}
									}
								}
								else
									sprintf_s(szBuf, "\n\nLast User:\t%u\nFile(s):\t\tUNKNOWN", tracking.lastUser);

								strcat_s(pGetInfoTip->pszText, pGetInfoTip->cchTextMax, szBuf);
								pGetInfoTip->pszText[pGetInfoTip->cchTextMax - 1] = '\0';
							}
						}

						return S_OK;
					}
				}
				else if (Message == WM_INITMENU)
				{
					auto hMenu = reinterpret_cast<HMENU>(wParam);
					if (IsMenu(hMenu) &&
						GetMenuState(hMenu, Common::EditorUI::UI_EDITOR_DELETEFORM, MF_BYCOMMAND) != (UINT)-1 &&
						GetMenuState(hMenu, Common::EditorUI::UI_EDITOR_COPY_EDITOR_ID, MF_BYCOMMAND) == (UINT)-1)
					{
						int count = GetMenuItemCount(hMenu);
						for (int i = 0; i < count; i++)
						{
							if (GetMenuItemID(hMenu, i) == Common::EditorUI::UI_EDITOR_DELETEFORM)
							{
								InsertMenuA(hMenu, i + 1, MF_BYPOSITION | MF_STRING, Common::EditorUI::UI_EDITOR_COPY_EDITOR_ID, "Copy Editor ID");
								InsertMenuA(hMenu, i + 2, MF_BYPOSITION | MF_STRING, Common::EditorUI::UI_EDITOR_COPY_FORM_ID, "Copy Form ID");
								break;
							}
						}
					}
				}

				return CallWindowProc(GlobalObjectWindowBasePtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}