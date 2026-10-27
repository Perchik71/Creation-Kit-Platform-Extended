// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.AboutWindow.h>
#include <CKPE.Common.MemoryManager.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.DialogManager.h>
#include <CKPE.Common.Docking.h>
#include <CKPE.CriticalSection.h>
#include <CKPE.Exception.h>
#include <CKPE.Detours.h>
#include <algorithm>
#include <Shlwapi.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>

namespace CKPE
{
	namespace Common
	{
		// Bypass Wine's result truncation bug
		// Declare a thread-local variable
		thread_local std::ptrdiff_t dialogModalResult = 0;

		struct DialogOverrideData
		{
			DLGPROC DialogFunc;	// Original function pointer
			bool IsDialog;		// True if it requires EndDialog()
		};

		EditorUI seditorUI;

		struct EditorUI::DeferFrame
		{
			void* ListView{ nullptr };
			void* ComboBox{ nullptr };
			std::uintptr_t StringLength{ 0 };
			bool AllowResize{ false };
			std::vector<std::pair<const char*, void*>> Items;
		};

		EditorUI::EditorUI() noexcept(true)
		{
			_DeferFrames = new std::vector<DeferFrame>;
			InitCommonControls();
		}

		EditorUI* EditorUI::GetSingleton() noexcept(true)
		{
			return &seditorUI;
		}
		
		void* EditorUI::ListViewGetSelectedItem(void* ListViewHandle) noexcept(true)
		{
			if (!ListViewHandle)
				return nullptr;

			// Get the first focused item
			auto index = ListView_GetNextItem((HWND)ListViewHandle, -1, LVIS_FOCUSED);

			if (index == -1)
				return nullptr;

			LVITEMA item = { 0 };
			item.mask = LVIF_PARAM;
			item.iItem = index;

			ListView_GetItem((HWND)ListViewHandle, &item);
			return (void*)item.lParam;
		}

		int EditorUI::ListViewGetSelectedItemIndex(void* ListViewHandle) noexcept(true)
		{
			// Get the first focused item
			return ListView_GetNextItem((HWND)ListViewHandle, -1, LVIS_FOCUSED);
		}

		bool EditorUI::ListViewSetItemState(void* ListViewHandle, std::ptrdiff_t Index,
			std::uint32_t Data, std::uint32_t Mask) noexcept(true)
		{
			// Microsoft's implementation of this define is broken (ListView_SetItemState)
			LVITEMA item
			{
				.mask = LVIF_STATE,
				.state = Data,
				.stateMask = Mask
			};

			return static_cast<bool>(SendMessageA((HWND)ListViewHandle, LVM_SETITEMSTATE, Index, 
				reinterpret_cast<LPARAM>(&item)));
		}

		void EditorUI::ListViewSelectItem(void* ListViewHandle, int ItemIndex, bool KeepOtherSelections) noexcept(true)
		{
			if (!KeepOtherSelections)
				ListViewSetItemState(ListViewHandle, -1, 0, LVIS_SELECTED | LVIS_FOCUSED);

			if (ItemIndex != -1)
			{
				ListView_EnsureVisible((HWND)ListViewHandle, ItemIndex, FALSE);
				ListViewSetItemState(ListViewHandle, ItemIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}

		void EditorUI::ListViewFindAndSelectItem(void* ListViewHandle, void* Parameter, bool KeepOtherSelections) noexcept(true)
		{
			if (!KeepOtherSelections)
				ListViewSetItemState(ListViewHandle, -1, 0, LVIS_SELECTED);

			LVFINDINFOA findInfo
			{
				.flags = LVFI_PARAM,
				.lParam = reinterpret_cast<LPARAM>(Parameter)
			};

			auto index = ListView_FindItem((HWND)ListViewHandle, -1, &findInfo);

			if (index != -1)
				ListViewSelectItem(ListViewHandle, index, KeepOtherSelections);
		}

		void EditorUI::ListViewDeselectItem(void* ListViewHandle, void* Parameter) noexcept(true)
		{
			LVFINDINFOA findInfo
			{
				.flags = LVFI_PARAM,
				.lParam = reinterpret_cast<LPARAM>(Parameter)
			};

			auto index = ListView_FindItem((HWND)ListViewHandle, -1, &findInfo);

			if (index != -1)
				ListViewSetItemState(ListViewHandle, index, 0, LVIS_SELECTED);
		}

		void EditorUI::ListViewInsertItemDeferred(void* ListViewHandle, void* Parameter,
			bool UseImage, int ItemIndex) noexcept(true)
		{
			if (ItemIndex == -1)
				ItemIndex = INT_MAX;

			LVITEMA item
			{
				.mask = LVIF_PARAM | LVIF_TEXT,
				.iItem = ItemIndex,
				.pszText = LPSTR_TEXTCALLBACK,
				.lParam = reinterpret_cast<LPARAM>(Parameter)
			};

			if (UseImage)
			{
				item.mask |= LVIF_IMAGE;
				item.iImage = I_IMAGECALLBACK;
			}

			if (auto frame = seditorUI.GetDeferFrame())
			{
				if (frame->ListView && (frame->ListView != ListViewHandle))
					ErrorHandler::Trigger("Got handles to different list views in one deferred window.");

				if (!frame->ListView)
				{
					frame->ListView = ListViewHandle;
					SendMessage((HWND)ListViewHandle, WM_SETREDRAW, FALSE, 0);
				}
			}

			ListView_InsertItem((HWND)ListViewHandle, &item);
		}

		int EditorUI::ListViewFindItemByString(void* ListViewHandle, const char* str, int start_idx) noexcept(true)
		{
			// The standard search engine is too weak. 
			// Mine allows you to find the first match in the list even if the word is somewhere in the middle. 
			// Standard will only find if there is a match first.

			if (strlen(str) == 0)
				return -1;

			CHAR szBuf[1024] = { 0 };
			auto nRows = ListView_GetItemCount((HWND)ListViewHandle);

			if (nRows > start_idx)
			{
				for (int idx = start_idx; idx < nRows; idx++)
				{
					ListView_GetItemText((HWND)ListViewHandle, idx, 0, szBuf, sizeof(szBuf));
					if (StrStrI(szBuf, str))
						return idx;
				}
			}

			return -1;
		}

		EditorUI::DeferFrame* EditorUI::GetDeferFrame() noexcept(true)
		{
			return _DeferFrames->empty() ? nullptr : &_DeferFrames->back();
		}

		// Windows nest when an iterator's predicate runs another deferred iterator
		void EditorUI::BeginUIDefer() noexcept(true)
		{
			_DeferFrames->emplace_back();
		}

		void EditorUI::EndUIDefer() noexcept(true)
		{
			if (_DeferFrames->empty())
				return;

			auto frame = std::move(_DeferFrames->back());
			_DeferFrames->pop_back();
			FlushDeferFrame(frame);
		}

		void EditorUI::FlushDeferFrame(DeferFrame& Frame) noexcept(true)
		{
			if (Frame.ListView)
			{
				SendMessage((HWND)Frame.ListView, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)Frame.ListView, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			if (!Frame.Items.empty())
			{
				auto control = (const HWND)Frame.ComboBox;

				// Sort alphabetically if requested to try and speed up inserts
				int finalWidth = 0;
				LONG_PTR style = GetWindowLongPtr(control, GWL_STYLE);

				if ((style & CBS_SORT) == CBS_SORT)
				{
					std::sort(Frame.Items.begin(), Frame.Items.end(),
						[](const auto& a, const auto& b) -> bool
						{
							return _stricmp(a.first, b.first) < 0;
						});
				}

				SendMessage(control, CB_INITSTORAGE, Frame.Items.size(), Frame.StringLength * sizeof(char));

				if (HDC hdc = GetDC(control); hdc)
				{
					SuspendComboBoxUpdates(control, true);

					// Pre-calculate font widths for resizing, starting with TrueType
					std::array<int, UCHAR_MAX + 1> fontWidths = { 0 };
					std::array<ABC, UCHAR_MAX + 1> trueTypeFontWidths = { 0 };

					if (!GetCharABCWidthsA(hdc, 0, static_cast<UINT>(trueTypeFontWidths.size() - 1), trueTypeFontWidths.data()))
					{
						BOOL result = GetCharWidthA(hdc, 0, static_cast<UINT>(fontWidths.size() - 1), fontWidths.data());
						if (!result) 
							ErrorHandler::Trigger(StringUtils::FormatString("Failed to determine any font widths: %s",
								ErrorHandler::GetSystemMessage(GetLastError()).c_str()));
					}
					else
					{
						for (int i = 0; i < fontWidths.size(); i++)
							fontWidths[i] = trueTypeFontWidths[i].abcB;
					}

					// Insert everything all at once
					for (auto& [display, value] : Frame.Items)
					{
						LRESULT index = SendMessageA(control, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(display));
						int lineSize = 0;

						if (index != CB_ERR && index != CB_ERRSPACE)
							SendMessageA(control, CB_SETITEMDATA, index, reinterpret_cast<LPARAM>(value));

						for (const char* c = display; *c != '\0'; c++)
							lineSize += fontWidths[*c];

						finalWidth = std::max(finalWidth, lineSize);
					}

					SuspendComboBoxUpdates(control, false);
					ReleaseDC(control, hdc);
				}

				for (auto& item : Frame.Items)
					free(const_cast<char*>(item.first));

				// Resize to fit
				if (Frame.AllowResize)
				{
					LRESULT currentWidth = SendMessage(control, CB_GETDROPPEDWIDTH, 0, 0);

					if (finalWidth > currentWidth)
						SendMessage(control, CB_SETDROPPEDWIDTH, finalWidth, 0);
				}
			}
		}

		void EditorUI::SuspendComboBoxUpdates(void* ComboHandle, bool Suspend) noexcept(true)
		{
			COMBOBOXINFO info
			{
				.cbSize = sizeof(COMBOBOXINFO)
			};

			if (!GetComboBoxInfo((HWND)ComboHandle, &info))
				return;

			if (!Suspend)
			{
				SendMessage(info.hwndList, WM_SETREDRAW, TRUE, 0);
				SendMessage((HWND)ComboHandle, CB_SETMINVISIBLE, 30, 0);
				SendMessage((HWND)ComboHandle, WM_SETREDRAW, TRUE, 0);
			}
			else
			{
				SendMessage((HWND)ComboHandle, WM_SETREDRAW, FALSE, 0);	// Prevent repainting until finished
				SendMessage((HWND)ComboHandle, CB_SETMINVISIBLE, 1, 0);	// Possible optimization for older libraries (source: MSDN forums)
				SendMessage(info.hwndList, WM_SETREDRAW, FALSE, 0);
			}
		}

		void EditorUI::ComboBoxInsertItemDeferred(void* ComboBoxHandle, const char* DisplayText,
			void* Value, bool AllowResize) noexcept(true)
		{
			if (!ComboBoxHandle)
				return;

			if (!DisplayText)
				DisplayText = "NONE";

			if (auto frame = seditorUI.GetDeferFrame())
			{
				if (frame->ComboBox && (frame->ComboBox != ComboBoxHandle))
					ErrorHandler::Trigger("Got handles to different combo boxes in one deferred window.");

				frame->ComboBox = ComboBoxHandle;
				frame->StringLength += strlen(DisplayText) + 1;
				frame->AllowResize = AllowResize;

				// A copy must be created since lifetime isn't guaranteed after this function returns
				frame->Items.emplace_back(strdup(DisplayText), Value);
			}
			else
			{
				if (AllowResize)
				{
					if (HDC hdc = GetDC((HWND)ComboBoxHandle); hdc)
					{
						if (SIZE size; GetTextExtentPoint32A(hdc, DisplayText, static_cast<int>(strlen(DisplayText)), &size))
						{
							LRESULT currentWidth = SendMessageA((HWND)ComboBoxHandle, CB_GETDROPPEDWIDTH, 0, 0);

							if (size.cx > currentWidth)
								SendMessageA((HWND)ComboBoxHandle, CB_SETDROPPEDWIDTH, size.cx, 0);
						}

						ReleaseDC((HWND)ComboBoxHandle, hdc);
					}
				}

				LRESULT index = SendMessageA((HWND)ComboBoxHandle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(DisplayText));

				if (index != CB_ERR && index != CB_ERRSPACE)
					SendMessageA((HWND)ComboBoxHandle, CB_SETITEMDATA, index, reinterpret_cast<LPARAM>(Value));
			}
		}

		void EditorUI::CopyTextToClipboard(void* Hwnd, const char* Text) noexcept(true)
		{
			if (!Text)
				return;
			if (OpenClipboard((HWND)Hwnd))
			{
				EmptyClipboard();
				size_t len = strlen(Text) + 1;
				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
				if (hMem)
				{
					memcpy(GlobalLock(hMem), Text, len);
					GlobalUnlock(hMem);
					SetClipboardData(CF_TEXT, hMem);
				}
				CloseClipboard();
			}
		}

		namespace
		{
			constexpr int kCompileFilterHeight = 20;
			constexpr int kCompileFilterGap = 4;
			constexpr int kCompileFilterOffset = kCompileFilterHeight + kCompileFilterGap;

			struct CompileScriptEntry
			{
				std::string Name;
				LPARAM Param;
			};

			struct CompileScriptsDialogState
			{
				HWND DialogHandle{ nullptr };
				HWND ListViewHandle{ nullptr };
				HWND FilterEditHandle{ nullptr };
				std::vector<CompileScriptEntry> MasterItems;
				std::unordered_set<LPARAM> CheckedParams;
				bool SuppressNotify{ false };
			};

			class ScopedCompileListViewRedraw
			{
				CompileScriptsDialogState& _state;
			public:
				explicit ScopedCompileListViewRedraw(CompileScriptsDialogState& State) noexcept(true) : _state(State)
				{
					_state.SuppressNotify = true;
					if (IsWindow(_state.ListViewHandle))
						SendMessageA(_state.ListViewHandle, WM_SETREDRAW, FALSE, 0);
				}

				~ScopedCompileListViewRedraw() noexcept(true)
				{
					if (IsWindow(_state.ListViewHandle))
					{
						SendMessageA(_state.ListViewHandle, WM_SETREDRAW, TRUE, 0);
						RedrawWindow(_state.ListViewHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
					}
					_state.SuppressNotify = false;
				}
			};

			std::unordered_map<void*, CompileScriptsDialogState> s_CompileScriptsDialogs;

			HWND ResolveCompileDialogHwnd(void* This, HWND FallbackChild) noexcept(true)
			{
				if (auto it = s_CompileScriptsDialogs.find(This); it != s_CompileScriptsDialogs.end() && IsWindow(it->second.DialogHandle))
					return it->second.DialogHandle;

				if (FallbackChild && IsWindow(FallbackChild))
				{
					if (HWND parent = GetParent(FallbackChild); IsWindow(parent))
						return parent;
				}

				if (This)
				{
					HWND candidate = *reinterpret_cast<HWND*>(reinterpret_cast<std::uintptr_t>(This) + 0x10);
					if (IsWindow(candidate))
						return candidate;
				}

				return nullptr;
			}

			void EnsureCompileHandles(CompileScriptsDialogState& State, void* This,
				HWND FallbackChild, std::uint32_t ListViewId) noexcept(true)
			{
				if (!IsWindow(State.DialogHandle))
					State.DialogHandle = ResolveCompileDialogHwnd(This, FallbackChild);
				if (!IsWindow(State.ListViewHandle) && IsWindow(State.DialogHandle))
					State.ListViewHandle = GetDlgItem(State.DialogHandle, ListViewId);
				if (!IsWindow(State.FilterEditHandle) && IsWindow(State.DialogHandle))
					State.FilterEditHandle = GetDlgItem(State.DialogHandle, EditorUI::UI_COMPILESCRIPT_DIALOG_FILTER);
			}

			template<typename Fn>
			void ForEachVisibleCompileItem(HWND ListViewHandle, Fn&& Callback) noexcept(true)
			{
				const int count = ListView_GetItemCount(ListViewHandle);
				for (int i = 0; i < count; ++i)
				{
					LVITEMA item{};
					item.mask = LVIF_PARAM;
					item.iItem = i;
					if (ListView_GetItem(ListViewHandle, &item))
						Callback(i, item.lParam);
				}
			}

			void UpdateCompileDialogButtons(const CompileScriptsDialogState& State) noexcept(true)
			{
				if (!IsWindow(State.DialogHandle) || !IsWindow(State.ListViewHandle))
					return;

				const int visibleCount = ListView_GetItemCount(State.ListViewHandle);
				const BOOL hasChecked = !State.CheckedParams.empty() ? TRUE : FALSE;
				const BOOL hasVisible = (visibleCount > 0) ? TRUE : FALSE;

				if (HWND hCompile = GetDlgItem(State.DialogHandle, EditorUI::UI_COMPILESCRIPT_DIALOG_COMPILE))
					EnableWindow(hCompile, hasChecked);
				if (HWND hCheckAll = GetDlgItem(State.DialogHandle, EditorUI::UI_COMPILESCRIPT_DIALOG_CHECKALL))
					EnableWindow(hCheckAll, hasVisible);
				if (HWND hUncheckAll = GetDlgItem(State.DialogHandle, EditorUI::UI_COMPILESCRIPT_DIALOG_UNCHECKALL))
					EnableWindow(hUncheckAll, hasVisible);
			}

			void SyncCompileFilterBounds(CompileScriptsDialogState& State, void* This, std::uint32_t ListViewId) noexcept(true)
			{
				EnsureCompileHandles(State, This, nullptr, ListViewId);
				if (!IsWindow(State.ListViewHandle) || !IsWindow(State.FilterEditHandle))
					return;

				RECT rcList{};
				GetWindowRect(State.ListViewHandle, &rcList);
				MapWindowPoints(HWND_DESKTOP, State.DialogHandle, reinterpret_cast<LPPOINT>(&rcList), 2);

				SetWindowPos(State.FilterEditHandle, nullptr, rcList.left, rcList.top - kCompileFilterOffset,
					rcList.right - rcList.left, kCompileFilterHeight, SWP_NOZORDER | SWP_NOACTIVATE);
			}

			void PopulateCompileListView(CompileScriptsDialogState& State, const char* Filter) noexcept(true)
			{
				if (!IsWindow(State.ListViewHandle))
					return;

				{
					ScopedCompileListViewRedraw redraw(State);
					ListView_DeleteAllItems(State.ListViewHandle);

					const bool hasFilter = Filter && Filter[0] != '\0';
					int insertIndex = 0;

					for (const auto& entry : State.MasterItems)
					{
						if (hasFilter && !StrStrIA(entry.Name.c_str(), Filter))
							continue;

						LVITEMA item{};
						item.mask = LVIF_TEXT | LVIF_PARAM;
						item.iItem = insertIndex;
						item.pszText = const_cast<LPSTR>(entry.Name.c_str());
						item.lParam = entry.Param;

						const int idx = ListView_InsertItem(State.ListViewHandle, &item);
						if (idx != -1 && State.CheckedParams.contains(entry.Param))
							ListView_SetCheckState(State.ListViewHandle, idx, TRUE);

						++insertIndex;
					}
				}

				UpdateCompileDialogButtons(State);
			}

			void PrepareCompileDialogControls(CompileScriptsDialogState& State, void* This,
				std::uintptr_t wParam, std::uint32_t ListViewId) noexcept(true)
			{
				State.MasterItems.clear();
				State.CheckedParams.clear();
				EnsureCompileHandles(State, This, reinterpret_cast<HWND>(wParam), ListViewId);
				if (!IsWindow(State.DialogHandle) || !IsWindow(State.ListViewHandle))
					return;

				RECT rcList{};
				GetWindowRect(State.ListViewHandle, &rcList);
				MapWindowPoints(HWND_DESKTOP, State.DialogHandle, reinterpret_cast<LPPOINT>(&rcList), 2);

				const int width = rcList.right - rcList.left;
				const int height = (std::max)(20L, (rcList.bottom - rcList.top) - kCompileFilterOffset);

				SetWindowPos(State.ListViewHandle, nullptr, rcList.left, rcList.top + kCompileFilterOffset, width, height,
					SWP_NOZORDER | SWP_NOACTIVATE);

				if (!IsWindow(State.FilterEditHandle))
				{
					State.FilterEditHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, "",
						WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
						rcList.left, rcList.top, width, kCompileFilterHeight,
						State.DialogHandle, reinterpret_cast<HMENU>(static_cast<std::uintptr_t>(EditorUI::UI_COMPILESCRIPT_DIALOG_FILTER)),
						GetModuleHandleA(nullptr), nullptr);
				}

				if (IsWindow(State.FilterEditHandle))
				{
					SendMessageA(State.FilterEditHandle, WM_SETFONT, SendMessageA(State.DialogHandle, WM_GETFONT, 0, 0), TRUE);
					SendMessageW(State.FilterEditHandle, EM_SETCUEBANNER, FALSE, reinterpret_cast<LPARAM>(L"Filter scripts..."));
				}
			}

			void CaptureMasterCompileList(CompileScriptsDialogState& State, void* This, std::uint32_t ListViewId) noexcept(true)
			{
				EnsureCompileHandles(State, This, nullptr, ListViewId);
				if (IsWindow(State.ListViewHandle))
				{
					const int count = ListView_GetItemCount(State.ListViewHandle);
					State.MasterItems.reserve(count);

					char textBuf[1024]{};
					for (int i = 0; i < count; ++i)
					{
						textBuf[0] = '\0';
						LVITEMA item{};
						item.mask = LVIF_TEXT | LVIF_PARAM;
						item.iItem = i;
						item.iSubItem = 0;
						item.pszText = textBuf;
						item.cchTextMax = sizeof(textBuf);

						if (ListView_GetItem(State.ListViewHandle, &item))
						{
							State.MasterItems.push_back({ textBuf, item.lParam });
							if (ListView_GetCheckState(State.ListViewHandle, i))
								State.CheckedParams.insert(item.lParam);
						}
					}
				}

				SyncCompileFilterBounds(State, This, ListViewId);
				UpdateCompileDialogButtons(State);
			}
		}

		std::ptrdiff_t EditorUI::CompileScriptsDialogProc(void* This, std::uint32_t Message,
			std::uintptr_t wParam, std::ptrdiff_t lParam, std::uint32_t ListViewId,
			TDialogProc DefaultDlgProc,
			const std::function<void(void*)>& OnInitDialog,
			const std::function<void(void*)>& OnSize,
			const std::function<void(void*)>& OnCheckAllCheckedOut,
			const std::function<void(void*)>& OnCompile) noexcept(true)
		{
			switch (Message)
			{
			case WM_INITDIALOG:
			{
				auto& state = s_CompileScriptsDialogs[This];
				PrepareCompileDialogControls(state, This, wParam, ListViewId);
				std::ptrdiff_t res = 0;
				{
					ScopedCompileListViewRedraw redraw(state);
					if (OnInitDialog)
						OnInitDialog(This);
					res = DefaultDlgProc(This, Message, wParam, lParam);
				}
				CaptureMasterCompileList(state, This, ListViewId);
				return res;
			}
			case WM_SIZE:
			case WM_WINDOWPOSCHANGED:
			{
				if (Message == WM_SIZE && OnSize)
					OnSize(This);
				const auto res = DefaultDlgProc(This, Message, wParam, lParam);
				if (auto it = s_CompileScriptsDialogs.find(This); it != s_CompileScriptsDialogs.end())
					SyncCompileFilterBounds(it->second, This, ListViewId);
				return res;
			}
			case WM_NOTIFY:
			{
				auto* hdr = reinterpret_cast<LPNMHDR>(lParam);
				if (hdr && hdr->idFrom == ListViewId && hdr->code == LVN_ITEMCHANGED)
				{
					if (auto it = s_CompileScriptsDialogs.find(This); it != s_CompileScriptsDialogs.end())
					{
						auto& state = it->second;
						if (!state.SuppressNotify)
						{
							auto* nmlv = reinterpret_cast<LPNMLISTVIEW>(lParam);
							if (nmlv->uChanged & LVIF_STATE)
							{
								const std::uint32_t stateImg = (nmlv->uNewState & LVIS_STATEIMAGEMASK) >> 12;
								if (stateImg == 2)
									state.CheckedParams.insert(nmlv->lParam);
								else if (stateImg == 1)
									state.CheckedParams.erase(nmlv->lParam);

								UpdateCompileDialogButtons(state);
							}
						}
						return 1;
					}
				}
				break;
			}
			case WM_COMMAND:
			{
				auto it = s_CompileScriptsDialogs.find(This);
				if (it != s_CompileScriptsDialogs.end())
				{
					auto& state = it->second;
					EnsureCompileHandles(state, This, reinterpret_cast<HWND>(lParam), ListViewId);

					const std::uint32_t cmdId = LOWORD(wParam);
					const std::uint32_t notifyCode = HIWORD(wParam);

					if (cmdId == UI_COMPILESCRIPT_DIALOG_FILTER && notifyCode == EN_CHANGE)
					{
						char filterText[256]{};
						if (IsWindow(state.FilterEditHandle))
							GetWindowTextA(state.FilterEditHandle, filterText, sizeof(filterText));

						PopulateCompileListView(state, filterText);
						return 1;
					}

					if (cmdId == UI_COMPILESCRIPT_DIALOG_CHECKALL || cmdId == UI_COMPILESCRIPT_DIALOG_UNCHECKALL)
					{
						if (IsWindow(state.ListViewHandle))
						{
							const bool check = (cmdId == UI_COMPILESCRIPT_DIALOG_CHECKALL);
							{
								ScopedCompileListViewRedraw redraw(state);
								ForEachVisibleCompileItem(state.ListViewHandle, [&](int Index, LPARAM Param)
									{
										if (check)
											state.CheckedParams.insert(Param);
										else
											state.CheckedParams.erase(Param);
										ListView_SetCheckState(state.ListViewHandle, Index, check ? TRUE : FALSE);
									});
							}
							UpdateCompileDialogButtons(state);
						}
						return 1;
					}

					if (cmdId == UI_COMPILESCRIPT_DIALOG_CHECKALLCHECKEDOUT)
					{
						if (IsWindow(state.ListViewHandle))
						{
							{
								ScopedCompileListViewRedraw redraw(state);
								if (OnCheckAllCheckedOut)
									OnCheckAllCheckedOut(This);
								else
									DefaultDlgProc(This, Message, wParam, lParam);

								ForEachVisibleCompileItem(state.ListViewHandle, [&](int Index, LPARAM Param)
									{
										if (ListView_GetCheckState(state.ListViewHandle, Index))
											state.CheckedParams.insert(Param);
										else if (state.CheckedParams.contains(Param))
											ListView_SetCheckState(state.ListViewHandle, Index, TRUE);
									});
							}
							UpdateCompileDialogButtons(state);
						}
						return 1;
					}

					if (cmdId == UI_COMPILESCRIPT_DIALOG_COMPILE)
					{
						if (IsWindow(state.ListViewHandle) &&
							static_cast<std::size_t>(ListView_GetItemCount(state.ListViewHandle)) != state.MasterItems.size())
						{
							PopulateCompileListView(state, "");
						}
						if (OnCompile)
							OnCompile(This);
					}
				}
				break;
			}
			case WM_DESTROY:
			case WM_NCDESTROY:
				s_CompileScriptsDialogs.erase(This);
				break;
			}

			return DefaultDlgProc(This, Message, wParam, lParam);
		}

		static void SyncGroundPlaneCheck(HWND Dialog, std::uint32_t CheckboxId,
			const EditorUI::GroundPlane& Plane) noexcept(true)
		{
			if (void* preview = Plane.GetPreview(Dialog, EditorUI::UI_PREVIEW_CONTROL))
				CheckDlgButton(Dialog, CheckboxId, Plane.IsShown(preview) ? BST_CHECKED : BST_UNCHECKED);
		}

		std::ptrdiff_t EditorUI::GroundPlaneDialogProc(void* This, std::uint32_t Message,
			std::uintptr_t wParam, std::ptrdiff_t lParam, TDialogProc DefaultDlgProc,
			const GroundPlane& Plane, std::uint32_t GameCheckboxId) noexcept(true)
		{
			auto dialog = (HWND)This;
			const bool owned = !GameCheckboxId;
			const auto checkboxId = owned ? UI_GROUNDPLANE_CHECKBOX : GameCheckboxId;

			if (owned && (Message == WM_COMMAND) && (LOWORD(wParam) == checkboxId) && (HIWORD(wParam) == BN_CLICKED))
			{
				if (void* preview = Plane.GetPreview(dialog, UI_PREVIEW_CONTROL))
					Plane.SetShown(preview, IsDlgButtonChecked(dialog, checkboxId) == BST_CHECKED);
				SyncGroundPlaneCheck(dialog, checkboxId, Plane);
				return TRUE;
			}

			auto result = DefaultDlgProc(This, Message, wParam, lParam);

			switch (Message)
			{
			case WM_INITDIALOG:
			{
				HWND preview = GetDlgItem(dialog, UI_PREVIEW_CONTROL);
				HWND ok = GetDlgItem(dialog, IDOK);
				if (owned && preview && ok)
				{
					RECT rcPreview{}, rcOk{};
					GetWindowRect(preview, &rcPreview);
					GetWindowRect(ok, &rcOk);
					MapWindowPoints(HWND_DESKTOP, dialog, reinterpret_cast<LPPOINT>(&rcPreview), 2);
					MapWindowPoints(HWND_DESKTOP, dialog, reinterpret_cast<LPPOINT>(&rcOk), 2);

					HWND check = CreateWindowExA(0, WC_BUTTONA, "Ground Plane",
						WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
						rcPreview.left, rcOk.top, 110, rcOk.bottom - rcOk.top, dialog,
						reinterpret_cast<HMENU>(static_cast<std::uintptr_t>(checkboxId)),
						GetModuleHandleA(nullptr), nullptr);
					if (check)
						SendMessageA(check, WM_SETFONT, SendMessageA(dialog, WM_GETFONT, 0, 0), TRUE);
				}
				SyncGroundPlaneCheck(dialog, checkboxId, Plane);
				break;
			}
			case WM_COMMAND:
			case WM_NOTIFY:
			case WM_KEYDOWN:
				SyncGroundPlaneCheck(dialog, checkboxId, Plane);
				break;
			}

			return result;
		}

		/////////////////////////////////////////

		CriticalSection DialogLock;
		std::unordered_map<HWND, DialogOverrideData> DialogOverrides;
		thread_local DialogOverrideData ThreadDialogData;

		void* EditorUI::Hook::HKCreateWindowA(const char* lpClassName, const char* lpWindowName,
			std::uint32_t dwStyle, std::int32_t nX, std::int32_t nY, std::int32_t nWidth, std::int32_t nHeight,
			void* hWndParent, void* hMenu, void* hInstance, void* lpParam) noexcept(true)
		{
			return HKCreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, nX, nY,
				nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		}

		void* EditorUI::Hook::HKCreateWindowExA(std::uint32_t dwExStyle, const char* lpClassName, const char* lpWindowName,
			std::uint32_t dwStyle, std::int32_t nX, std::int32_t nY, std::int32_t nWidth, std::int32_t nHeight,
			void* hWndParent, void* hMenu, void* hInstance, void* lpParam) noexcept(true)
		{
			auto hWnd = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, nX, nY, nWidth, nHeight,
				(HWND)hWndParent, (HMENU)hMenu, (HINSTANCE)hInstance, lpParam);

			//if (hWnd && !_stricmp(lpClassName, "MDICLIENT"))
			//	Interface::GetSingleton()->GetDockingManager()->SetWindow((std::uintptr_t)hWnd);

			return hWnd;
		}

		void* EditorUI::Hook::HKCreateDialogParamA(void* hInstance, const char* lpTemplateName, void* hWndParent,
			std::uintptr_t lpDialogFunc, std::ptrdiff_t dwInitParam) noexcept(true)
		{
			if (reinterpret_cast<uintptr_t>(lpTemplateName) == 0x1D6) // "Warnings"
				return nullptr;

			// EndDialog MUST NOT be used
			ThreadDialogData.DialogFunc = (DLGPROC)lpDialogFunc;
			ThreadDialogData.IsDialog = false;

			//_MESSAGE("DBG dialog: %X(%u) %p", (DWORD)lpTemplateName, (DWORD)lpTemplateName, lpDialogFunc);

			// Override certain default dialogs to use this DLL's resources
			switch (reinterpret_cast<uintptr_t>(lpTemplateName))
			{
			case 0x64:// "About"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = AboutWindow::WndProc2;
				hInstance = (HINSTANCE)Interface::GetSingleton()->GetInstanceDLL();
				break;
			case 0xEB:// "Logo"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = AboutWindow::WndProc;
				hInstance = (HINSTANCE)Interface::GetSingleton()->GetInstanceDLL();
				break;
			}

			auto dmgr = DialogManager::GetSingleton();
			if (dmgr)
			{
				auto dialog = dmgr->GetDialog(reinterpret_cast<ULONG_PTR>(lpTemplateName));
				if (dialog)
					return dialog->Show((HWND)hWndParent, (DLGPROC)DialogFuncOverride, dwInitParam,
						(HINSTANCE)hInstance);
			}

			return CreateDialogParamA((HINSTANCE)hInstance, lpTemplateName, (HWND)hWndParent, 
				(DLGPROC)DialogFuncOverride, dwInitParam);
		}

		std::ptrdiff_t EditorUI::Hook::HKDialogBoxParamA(void* hInstance, const char* lpTemplateName, void* hWndParent,
			std::uintptr_t lpDialogFunc, std::ptrdiff_t dwInitParam) noexcept(true)
		{
			if (reinterpret_cast<uintptr_t>(lpTemplateName) == 0x1D6) // "Warnings"
				return 0;

			// EndDialog MUST be used
			ThreadDialogData.DialogFunc = (DLGPROC)lpDialogFunc;
			ThreadDialogData.IsDialog = true;

			//_MESSAGE("DBG dialog modal: %X(%u) %p", (DWORD)lpTemplateName, (DWORD)lpTemplateName, lpDialogFunc);

			// Override certain default dialogs to use this DLL's resources
			switch (reinterpret_cast<uintptr_t>(lpTemplateName))
			{
			case 0x64:// "About"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = AboutWindow::WndProc2;
				hInstance = (HINSTANCE)Interface::GetSingleton()->GetInstanceDLL();
				break;
			case 0xEB:// "Logo"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = AboutWindow::WndProc;
				hInstance = (HINSTANCE)Interface::GetSingleton()->GetInstanceDLL();
				break;
			}

			auto dmgr = DialogManager::GetSingleton();
			if (dmgr)
			{
				auto dialog = dmgr->GetDialog(reinterpret_cast<ULONG_PTR>(lpTemplateName));
				if (dialog)
					return dialog->ShowModal((HWND)hWndParent, (DLGPROC)DialogFuncOverride, dwInitParam,
						(HINSTANCE)hInstance);
			}

			// Skips result... use dialogModalResult
			(void)DialogBoxParamA((HINSTANCE)hInstance, lpTemplateName, (HWND)hWndParent,
				(DLGPROC)DialogFuncOverride, dwInitParam);	
			return dialogModalResult;
		}

		bool EditorUI::Hook::HKEndDialog(void* hDlg, std::ptrdiff_t nResult) noexcept(true)
		{
			ScopeCriticalSection lock{ DialogLock };

			// Store result for fixed Wine's trunc bug
			dialogModalResult = nResult;

			// Fix for the CK calling EndDialog on a CreateDialogParamA window
			if (auto itr = DialogOverrides.find((HWND)hDlg); itr != DialogOverrides.end() && !itr->second.IsDialog)
			{
				DestroyWindow((HWND)hDlg);
				return true;
			}

			return EndDialog((HWND)hDlg, nResult);
		}

		bool EditorUI::Hook::HKDestroyWindow(void* hDlg) noexcept(true)
		{
			return DestroyWindow((HWND)hDlg);
		}

		std::ptrdiff_t EditorUI::Hook::HKSendMessageA(void* hWnd, std::uint32_t Msg, std::ptrdiff_t wParam,
			std::ptrdiff_t lParam) noexcept(true)
		{
			if (hWnd && Msg == WM_DESTROY)
			{
				ScopeCriticalSection lock{ DialogLock };

				// If this is a dialog, we can't call DestroyWindow on it
				if (auto itr = DialogOverrides.find((HWND)hWnd); itr != DialogOverrides.end())
				{
					if (!itr->second.IsDialog)
						DestroyWindow((HWND)hWnd);
				}

				return 0;
			}

			return SendMessageA((HWND)hWnd, Msg, wParam, lParam);
		}

		void EditorUI::Hook::HKBeginUIDefer() noexcept(true)
		{
			seditorUI.BeginUIDefer();
		}

		void EditorUI::Hook::HKEndUIDefer() noexcept(true)
		{
			seditorUI.EndUIDefer();
		}

		std::ptrdiff_t EditorUI::Hook::DialogFuncOverride(void* hwndDlg, std::uint32_t uMsg, std::ptrdiff_t wParam,
			std::ptrdiff_t lParam) noexcept(true)
		{
			DLGPROC proc = nullptr;

			// RichEdit drops text past 32K chars; lift it before the CK fills it
			if (uMsg == WM_INITDIALOG)
				EnumChildWindows((HWND)hwndDlg, [](HWND Child, LPARAM) -> BOOL
					{
						char className[32]{};
						if (GetClassNameA(Child, className, sizeof(className)) && !_strnicmp(className, "RichEdit", 8))
							SendMessageA(Child, EM_EXLIMITTEXT, 0, 0x7FFFFFFF);
						return TRUE;
					}, 0);

			ScopeCriticalSection lock{ DialogLock };
			{
				if (auto itr = DialogOverrides.find((HWND)hwndDlg); itr != DialogOverrides.end())
					proc = itr->second.DialogFunc;

				// if (is new entry)
				if (!proc)
				{
					DialogOverrides[(HWND)hwndDlg] = ThreadDialogData;
					proc = ThreadDialogData.DialogFunc;

					ThreadDialogData.DialogFunc = nullptr;
					ThreadDialogData.IsDialog = false;
				}

				// Purge old entries every now and then
				if (DialogOverrides.size() >= 50)
				{
					for (auto itr = DialogOverrides.begin(); itr != DialogOverrides.end();)
					{
						if (itr->first == hwndDlg || IsWindow(itr->first))
						{
							itr++;
							continue;
						}

						itr = DialogOverrides.erase(itr);
					}
				}
			}

			return proc((HWND)hwndDlg, uMsg, wParam, lParam);
		}

		void EditorUI::Hook::Initialize() noexcept(true)
		{
			auto base = (std::uintptr_t)GetModuleHandleA(nullptr);

			Detours::DetourIAT(base, "USER32.DLL", "CreateDialogParamA", (std::uintptr_t)&HKCreateDialogParamA);
			Detours::DetourIAT(base, "USER32.DLL", "DialogBoxParamA", (std::uintptr_t)&HKDialogBoxParamA);
			Detours::DetourIAT(base, "USER32.DLL", "EndDialog", (std::uintptr_t)&HKEndDialog);
			Detours::DetourIAT(base, "USER32.DLL", "SendMessageA", (std::uintptr_t)&HKSendMessageA);
			Detours::DetourIAT(base, "USER32.DLL", "CreateWindowA", (std::uintptr_t)&HKCreateWindowA);
			Detours::DetourIAT(base, "USER32.DLL", "CreateWindowExA", (std::uintptr_t)&HKCreateWindowExA);
			Detours::DetourIAT(base, "USER32.DLL", "DestroyWindow", (std::uintptr_t)&HKDestroyWindow);
		}
	}
}