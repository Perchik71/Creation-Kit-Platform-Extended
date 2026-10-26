// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <unordered_map>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/T/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.DeferredDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TDeferredDlg_sub1 = void(void*, HWND);
			using TDeferredDlg_sub2 = void(void*, HWND, std::int64_t);
			using TDeferredDlg_sub3 = void(void*, HWND*);
			using TDeferredDlg_sub4 = void(void*);
			using TDeferredDlg_sub5 = std::ptrdiff_t(*)(void*, std::uint32_t, std::uintptr_t, std::ptrdiff_t);

			static std::function<TDeferredDlg_sub1> DeferredDlg_sub1;
			static std::function<TDeferredDlg_sub2> DeferredDlg_sub2;
			static std::function<TDeferredDlg_sub3> DeferredDlg_sub3;
			static std::function<TDeferredDlg_sub4> DeferredDlg_sub5;
			static std::function<TDeferredDlg_sub4> DeferredDlg_sub6;
			static std::function<TDeferredDlg_sub4> DeferredDlg_sub9;
			static std::function<TDeferredDlg_sub4> DeferredDlg_subA;
			static TDeferredDlg_sub5 DeferredDlg_subB = nullptr;

			template<typename T, bool Stable = false>
			static void DeferredDlg_ArrayQuickSortRecursive(EditorAPI::BSTArray<T>& Array,
				int(*SortFunction)(const void*, const void*)) noexcept(true)
			{
				auto compare = [SortFunction](const T& A, const T& B)
					{
						return SortFunction(A, B) == -1;
					};

				if constexpr (Stable)
					std::stable_sort(&Array[0], &Array[Array.size()], compare);
				else
					std::sort(&Array[0], &Array[Array.size()], compare);
			}

			static void DeferredDlg_SortDialogueInfo(std::int64_t TESDataHandler, std::uint32_t FormType,
				int(*SortFunction)(const void*, const void*)) noexcept(true)
			{
				static std::unordered_map<EditorAPI::BSTArray<EditorAPI::Forms::TESForm*>*, std::pair<void*, std::uint32_t>> arrayCache;

				auto formArray = &((EditorAPI::BSTArray<EditorAPI::Forms::TESForm*>*)(TESDataHandler + 104))[FormType];
				auto itr = arrayCache.find(formArray);

				// If not previously found or any counters changed...
				if (itr == arrayCache.end() || itr->second.first != formArray->data() || itr->second.second != formArray->size())
				{
					// Update and resort the array
					DeferredDlg_ArrayQuickSortRecursive(*formArray, SortFunction);

					arrayCache[formArray] = std::make_pair(formArray->data(), formArray->size());
				}
			}

			static void DeferredDlg_UpdateTreeView(void* Thisptr, HWND ControlHandle) noexcept(true)
			{
				SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
				DeferredDlg_sub1(Thisptr, ControlHandle);
				SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			static void DeferredDlg_UpdateCellList(void* Thisptr, HWND ControlHandle, std::int64_t Unknown) noexcept(true)
			{
				SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
				DeferredDlg_sub2(Thisptr, ControlHandle, Unknown);
				SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			static void DeferredDlg_UpdateObjectList(void* Thisptr, HWND* ControlHandle)
			{
				SendMessage(*ControlHandle, WM_SETREDRAW, FALSE, 0);
				DeferredDlg_sub3(Thisptr, ControlHandle);
				SendMessage(*ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(*ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			static LRESULT DeferredDlg_PickScriptsToCompileDlgProc(void* This, UINT Message, WPARAM wParam,
				LPARAM lParam) noexcept(true)
			{
				return Common::EditorUI::CompileScriptsDialogProc(This, Message, wParam, lParam,
					Common::EditorUI::UI_SSE_COMPILESCRIPT_DIALOG_LISTVIEW, DeferredDlg_subB,
					DeferredDlg_sub6, DeferredDlg_sub5, DeferredDlg_sub9, DeferredDlg_subA);
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

			bool DeferredDlg::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool DeferredDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool DeferredDlg::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				DeferredDlg_sub1 = Relocation<TDeferredDlg_sub1>(ID(559670)).Get();
				DeferredDlg_sub2 = Relocation<TDeferredDlg_sub2>(ID(66544)).Get();
				DeferredDlg_sub3 = Relocation<TDeferredDlg_sub3>(ID{ 754119, 1018099 }).Get();
				DeferredDlg_sub5 = Relocation<TDeferredDlg_sub4>(ID(339341)).Get();
				DeferredDlg_sub6 = Relocation<TDeferredDlg_sub4>(ID(134769)).Get();
				DeferredDlg_sub9 = Relocation<TDeferredDlg_sub4>(ID(344535)).Get();
				DeferredDlg_subA = Relocation<TDeferredDlg_sub4>(ID(32469)).Get();
				DeferredDlg_subB = Relocation<TDeferredDlg_sub5>(ID(346632)).Get();

				// Deferred dialog loading (batched UI updates)
				Relocation(ID(361131)).WriteJump(&DeferredDlg_SortDialogueInfo);
				Relocation(ID(264130)).WriteJump(&EditorUI::ComboBoxInsertItemDeferred);
				Relocation(ID(314185)).WriteJump(&EditorUI::ListViewInsertItemDeferred);
				Relocation(ID(362342), 0x73).WriteCall(&DeferredDlg_UpdateTreeView);
				Relocation(ID(196690), 0x1BC4).WriteCall(&DeferredDlg_UpdateCellList);
				Relocation(ID{ 19166, 1018098 }, 0x3C).WriteCall(&DeferredDlg_UpdateObjectList);
				Relocation(ID(343097)).WriteJump(&DeferredDlg_PickScriptsToCompileDlgProc);
				
				return true;
			}
		}
	}
}