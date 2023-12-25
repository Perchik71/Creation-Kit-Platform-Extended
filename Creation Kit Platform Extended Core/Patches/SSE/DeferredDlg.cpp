// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/SSE/TESForm.h"
#include "DeferredDlg.h"

#define UI_COMIPLESCRIPT_DIALOG_COMPILE				1		// "Compile"
#define UI_COMIPLESCRIPT_DIALOG_CHECKALL			5474	// "Check All"
#define UI_COMIPLESCRIPT_DIALOG_UNCHECKALL			5475	// "Uncheck All"
#define UI_COMIPLESCRIPT_DIALOG_CHECKALLCHECKEDOUT	5602	// "Check All Checked-Out"
#define UI_COMIPLESCRIPT_DIALOG_LISTVIEW			5401	// Script list view

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_DeferredDlg_sub1 = 0;
			uintptr_t pointer_DeferredDlg_sub2 = 0;
			uintptr_t pointer_DeferredDlg_sub3 = 0;
			uintptr_t pointer_DeferredDlg_sub4 = 0;
			uintptr_t pointer_DeferredDlg_sub5 = 0;
			uintptr_t pointer_DeferredDlg_sub6 = 0;
			uintptr_t pointer_DeferredDlg_sub7 = 0;
			uintptr_t pointer_DeferredDlg_sub8 = 0;
			uintptr_t pointer_DeferredDlg_sub9 = 0;
			uintptr_t pointer_DeferredDlg_subA = 0;
			uintptr_t pointer_DeferredDlg_subB = 0;

			DeferredDlgPatch::DeferredDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool DeferredDlgPatch::HasOption() const
			{
				return false;
			}

			bool DeferredDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* DeferredDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* DeferredDlgPatch::GetName() const
			{
				return "Deferred dialog loading";
			}

			bool DeferredDlgPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> DeferredDlgPatch::GetDependencies() const
			{
				return {};
			}

			bool DeferredDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool DeferredDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					pointer_DeferredDlg_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					pointer_DeferredDlg_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_DeferredDlg_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_DeferredDlg_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					pointer_DeferredDlg_sub5 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));
					pointer_DeferredDlg_sub6 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));
					pointer_DeferredDlg_sub7 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));
					pointer_DeferredDlg_sub8 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(7));
					pointer_DeferredDlg_sub9 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(8));
					pointer_DeferredDlg_subA = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(9));
					pointer_DeferredDlg_subB = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(10));

					// Deferred dialog loading (batched UI updates)
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(11), (uintptr_t)&SortDialogueInfo);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(12), (uintptr_t)&EditorUI::ComboBoxInsertItemDeferred);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(13), (uintptr_t)&EditorUI::ListViewInsertItemDeferred);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(14), (uintptr_t)&UpdateTreeView);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(15), (uintptr_t)&UpdateCellList);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(16), (uintptr_t)&UpdateObjectList);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(17), (uintptr_t)&PickScriptsToCompileDlgProc);

					return true;
				}

				return false;
			}

			bool DeferredDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void DeferredDlgPatch::SortDialogueInfo(__int64 TESDataHandler, uint32_t FormType,
				int(*SortFunction)(const void*, const void*))
			{
				static UnorderedMap<BSTArray<TESForm*>*, std::pair<void*, uint32_t>> arrayCache;

				auto formArray = &((BSTArray<TESForm*>*)(TESDataHandler + 104))[FormType];
				auto itr = arrayCache.find(formArray);

				// If not previously found or any counters changed...
				if (itr == arrayCache.end() || itr->second.first != formArray->QBuffer() || itr->second.second != formArray->QSize())
				{
					// Update and resort the array
					ArrayQuickSortRecursive(*formArray, SortFunction);

					arrayCache[formArray] = std::make_pair(formArray->QBuffer(), formArray->QSize());
				}
			}

			void DeferredDlgPatch::UpdateTreeView(void* Thisptr, HWND ControlHandle)
			{
				SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
				((void(__fastcall*)(void*, HWND))pointer_DeferredDlg_sub1)(Thisptr, ControlHandle);
				SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void DeferredDlgPatch::UpdateCellList(void* Thisptr, HWND ControlHandle, __int64 Unknown)
			{
				SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
				((void(__fastcall*)(void*, HWND, __int64))pointer_DeferredDlg_sub2)(Thisptr, ControlHandle, Unknown);
				SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void DeferredDlgPatch::UpdateObjectList(void* Thisptr, HWND* ControlHandle)
			{
				SendMessage(*ControlHandle, WM_SETREDRAW, FALSE, 0);
				((void(__fastcall*)(void*, HWND*))pointer_DeferredDlg_sub3)(Thisptr, ControlHandle);
				SendMessage(*ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(*ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			LRESULT DeferredDlgPatch::PickScriptsToCompileDlgProc(void* This, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				thread_local bool disableListViewUpdates;

				auto updateListViewItems = [This]
				{
					if (!disableListViewUpdates)
						((void(__fastcall*)(void*))pointer_DeferredDlg_sub4)(This);
				};

				switch (Message)
				{
				case WM_SIZE:
					((void(__fastcall*)(void*))pointer_DeferredDlg_sub5)(This);
					break;

				case WM_NOTIFY:
				{
					auto notification = reinterpret_cast<LPNMHDR>(lParam);

					// "SysListView32" control
					if (notification->idFrom == UI_COMIPLESCRIPT_DIALOG_LISTVIEW && notification->code == LVN_ITEMCHANGED)
					{
						updateListViewItems();
						return 1;
					}
				}
				break;

				case WM_INITDIALOG:
					disableListViewUpdates = true;
					((void(__fastcall*)(void*))pointer_DeferredDlg_sub6)(This);
					disableListViewUpdates = false;

					// Update it ONCE after everything is inserted
					updateListViewItems();
					break;

				case WM_COMMAND:
				{
					const uint32_t param = LOWORD(wParam);

					if (param == UI_COMIPLESCRIPT_DIALOG_CHECKALL || param == UI_COMIPLESCRIPT_DIALOG_UNCHECKALL || 
						param == UI_COMIPLESCRIPT_DIALOG_CHECKALLCHECKEDOUT)
					{
						disableListViewUpdates = true;
						if (param == UI_COMIPLESCRIPT_DIALOG_CHECKALL)
							((void(__fastcall*)(void*))pointer_DeferredDlg_sub7)(This);
						else if (param == UI_COMIPLESCRIPT_DIALOG_UNCHECKALL)
							((void(__fastcall*)(void*))pointer_DeferredDlg_sub8)(This);
						else if (param == UI_COMIPLESCRIPT_DIALOG_CHECKALLCHECKEDOUT)
							((void(__fastcall*)(void*))pointer_DeferredDlg_sub9)(This);
						disableListViewUpdates = false;

						updateListViewItems();
						return 1;
					}
					else if (param == UI_COMIPLESCRIPT_DIALOG_COMPILE)
						// "Compile" button
						((void(__fastcall*)(void*))pointer_DeferredDlg_subA)(This);
				}
				break;
				}

				return ((LRESULT(__fastcall*)(void*, UINT, WPARAM, LPARAM))pointer_DeferredDlg_subB)(This, Message, wParam, lParam);
			}
		}
	}
}