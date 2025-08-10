// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <unordered_map>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSTArray.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.DeferredDlg.h>

#define UI_COMIPLESCRIPT_DIALOG_COMPILE				1		// "Compile"
#define UI_COMIPLESCRIPT_DIALOG_CHECKALL			5474	// "Check All"
#define UI_COMIPLESCRIPT_DIALOG_UNCHECKALL			5475	// "Uncheck All"
#define UI_COMIPLESCRIPT_DIALOG_CHECKALLCHECKEDOUT	5602	// "Check All Checked-Out"
#define UI_COMIPLESCRIPT_DIALOG_LISTVIEW			5401	// Script list view

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_DeferredDlg_sub1 = 0;
			std::uintptr_t pointer_DeferredDlg_sub2 = 0;
			std::uintptr_t pointer_DeferredDlg_sub3 = 0;
			std::uintptr_t pointer_DeferredDlg_sub4 = 0;
			std::uintptr_t pointer_DeferredDlg_sub5 = 0;
			std::uintptr_t pointer_DeferredDlg_sub6 = 0;
			std::uintptr_t pointer_DeferredDlg_sub7 = 0;
			std::uintptr_t pointer_DeferredDlg_sub8 = 0;
			std::uintptr_t pointer_DeferredDlg_sub9 = 0;
			std::uintptr_t pointer_DeferredDlg_subA = 0;
			std::uintptr_t pointer_DeferredDlg_subB = 0;

			template<typename T, bool Stable = false>
			static void DeferredDlg_ArrayQuickSortRecursive(EditorAPI::BSTArray<T>& Array,
				int(*SortFunction)(const void*, const void*)) noexcept(true)
			{
				auto compare = [SortFunction](const T& A, const T& B)
					{
						return SortFunction(A, B) == -1;
					};

				if constexpr (Stable)
					std::stable_sort(&Array[0], &Array[Array.QSize()], compare);
				else
					std::sort(&Array[0], &Array[Array.QSize()], compare);
			}

			static void DeferredDlg_SortDialogueInfo(std::int64_t TESDataHandler, std::uint32_t FormType,
				int(*SortFunction)(const void*, const void*)) noexcept(true)
			{
				static std::unordered_map<EditorAPI::BSTArray<EditorAPI::Forms::TESForm*>*, std::pair<void*, uint32_t>> arrayCache;

				auto formArray = &((EditorAPI::BSTArray<EditorAPI::Forms::TESForm*>*)(TESDataHandler + 104))[FormType];
				auto itr = arrayCache.find(formArray);

				// If not previously found or any counters changed...
				if (itr == arrayCache.end() || itr->second.first != formArray->QBuffer() || itr->second.second != formArray->QSize())
				{
					// Update and resort the array
					DeferredDlg_ArrayQuickSortRecursive(*formArray, SortFunction);

					arrayCache[formArray] = std::make_pair(formArray->QBuffer(), formArray->QSize());
				}
			}

			static void DeferredDlg_UpdateTreeView(void* Thisptr, HWND ControlHandle) noexcept(true)
			{
				SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
				((void(__fastcall*)(void*, HWND))pointer_DeferredDlg_sub1)(Thisptr, ControlHandle);
				SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			static void DeferredDlg_UpdateCellList(void* Thisptr, HWND ControlHandle, std::int64_t Unknown) noexcept(true)
			{
				SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
				((void(__fastcall*)(void*, HWND, __int64))pointer_DeferredDlg_sub2)(Thisptr, ControlHandle, Unknown);
				SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			static void DeferredDlg_UpdateObjectList(void* Thisptr, HWND* ControlHandle)
			{
				SendMessage(*ControlHandle, WM_SETREDRAW, FALSE, 0);
				((void(__fastcall*)(void*, HWND*))pointer_DeferredDlg_sub3)(Thisptr, ControlHandle);
				SendMessage(*ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(*ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			static LRESULT DeferredDlg_PickScriptsToCompileDlgProc(void* This, UINT Message, WPARAM wParam,
				LPARAM lParam) noexcept(true)
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

				return ((LRESULT(__fastcall*)(void*, UINT, WPARAM, LPARAM))pointer_DeferredDlg_subB)
					(This, Message, wParam, lParam);
			}

			DeferredDlg::DeferredDlg() : Common::Patch()
			{
				SetName("Deferred dialog loading");
			}

			bool DeferredDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* DeferredDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool DeferredDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> DeferredDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool DeferredDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool DeferredDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				pointer_DeferredDlg_sub1 = __CKPE_OFFSET(0);
				pointer_DeferredDlg_sub2 = __CKPE_OFFSET(1);
				pointer_DeferredDlg_sub3 = __CKPE_OFFSET(2);
				pointer_DeferredDlg_sub4 = __CKPE_OFFSET(3);
				pointer_DeferredDlg_sub5 = __CKPE_OFFSET(4);
				pointer_DeferredDlg_sub6 = __CKPE_OFFSET(5);
				pointer_DeferredDlg_sub7 = __CKPE_OFFSET(6);
				pointer_DeferredDlg_sub8 = __CKPE_OFFSET(7);
				pointer_DeferredDlg_sub9 = __CKPE_OFFSET(8);
				pointer_DeferredDlg_subA = __CKPE_OFFSET(9);
				pointer_DeferredDlg_subB = __CKPE_OFFSET(10);

				// Deferred dialog loading (batched UI updates)
				Detours::DetourJump(__CKPE_OFFSET(11), (std::uintptr_t)&DeferredDlg_SortDialogueInfo);
				Detours::DetourJump(__CKPE_OFFSET(12), (std::uintptr_t)&Common::EditorUI::ComboBoxInsertItemDeferred);
				Detours::DetourJump(__CKPE_OFFSET(13), (std::uintptr_t)&Common::EditorUI::ListViewInsertItemDeferred);
				Detours::DetourCall(__CKPE_OFFSET(14), (std::uintptr_t)&DeferredDlg_UpdateTreeView);
				Detours::DetourCall(__CKPE_OFFSET(15), (std::uintptr_t)&DeferredDlg_UpdateCellList);
				Detours::DetourCall(__CKPE_OFFSET(16), (std::uintptr_t)&DeferredDlg_UpdateObjectList);
				Detours::DetourJump(__CKPE_OFFSET(17), (std::uintptr_t)&DeferredDlg_PickScriptsToCompileDlgProc);

				return true;
			}

			
		}
	}
}