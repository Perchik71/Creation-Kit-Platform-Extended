// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Common.MemoryManager.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.CriticalSection.h>
#include <CKPE.Exception.h>
#include <CKPE.Detours.h>
#include <algorithm>
#include <unordered_map>
#include <array>

namespace CKPE
{
	namespace Common
	{
		struct DialogOverrideData
		{
			DLGPROC DialogFunc;	// Original function pointer
			bool IsDialog;		// True if it requires EndDialog()
		};

		EditorUI seditorUI;

		EditorUI::EditorUI() noexcept(true)
		{
			_DeferredMenuItems = new std::vector<std::pair<const char*, void*>>;
			InitCommonControls();
		}

		EditorUI* EditorUI::GetSingleton() noexcept(true)
		{
			return &seditorUI;
		}
		
		void* EditorUI::ListViewGetSelectedItem(void* ListViewHandle) noexcept(true)
		{
			if (!ListViewHandle)
				return NULL;

			int index = ListView_GetNextItem((HWND)ListViewHandle, -1, LVNI_SELECTED);

			if (index == -1)
				return NULL;

			LVITEMA item = { 0 };
			item.mask = LVIF_PARAM;
			item.iItem = index;

			ListView_GetItem((HWND)ListViewHandle, &item);
			return (void*)item.lParam;
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
				ListViewSetItemState(ListViewHandle, -1, 0, LVIS_SELECTED);

			if (ItemIndex != -1)
			{
				ListView_EnsureVisible((HWND)ListViewHandle, ItemIndex, FALSE);
				ListViewSetItemState(ListViewHandle, ItemIndex, LVIS_SELECTED, LVIS_SELECTED);
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

			int index = ListView_FindItem((HWND)ListViewHandle, -1, &findInfo);

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

			int index = ListView_FindItem((HWND)ListViewHandle, -1, &findInfo);

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

			if (seditorUI.UseDeferredDialogInsert)
			{
				if (seditorUI.DeferredListView && (seditorUI.DeferredListView != ListViewHandle))
					ErrorHandler::Trigger("Got handles to different list views? Reset probably wasn't called.");

				if (!seditorUI.DeferredListView)
				{
					seditorUI.DeferredListView = ListViewHandle;
					SendMessage((HWND)ListViewHandle, WM_SETREDRAW, FALSE, 0);
				}
			}

			ListView_InsertItem((HWND)ListViewHandle, &item);
		}

		void EditorUI::ResetUIDefer() noexcept(true)
		{
			UseDeferredDialogInsert = false;
			DeferredListView = nullptr;
			DeferredComboBox = nullptr;
			DeferredStringLength = 0;
			DeferredAllowResize = false;
			if (_DeferredMenuItems) _DeferredMenuItems->clear();
		}

		void EditorUI::BeginUIDefer() noexcept(true)
		{
			ResetUIDefer();
			UseDeferredDialogInsert = true;
		}

		void EditorUI::EndUIDefer() noexcept(true)
		{
			if (!UseDeferredDialogInsert)
				return;

			if (DeferredListView)
			{
				SendMessage((HWND)DeferredListView, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)DeferredListView, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			if (_DeferredMenuItems && !_DeferredMenuItems->empty())
			{
				auto control = (const HWND)DeferredComboBox;

				// Sort alphabetically if requested to try and speed up inserts
				int finalWidth = 0;
				LONG_PTR style = GetWindowLongPtr(control, GWL_STYLE);

				if ((style & CBS_SORT) == CBS_SORT)
				{
					std::sort(_DeferredMenuItems->begin(), _DeferredMenuItems->end(),
						[](const auto& a, const auto& b) -> bool
						{
							return _stricmp(a.first, b.first) > 0;
						});
				}

				SendMessage(control, CB_INITSTORAGE, _DeferredMenuItems->size(), DeferredStringLength * sizeof(char));

				if (HDC hdc = GetDC(control); hdc)
				{
					SuspendComboBoxUpdates(control, true);

					// Pre-calculate font widths for resizing, starting with TrueType
					std::array<int, UCHAR_MAX + 1> fontWidths = { 0 };
					std::array<ABC, UCHAR_MAX + 1> trueTypeFontWidths = { 0 };

					if (!GetCharABCWidthsA(hdc, 0, static_cast<UINT>(trueTypeFontWidths.size() - 1), trueTypeFontWidths.data()))
					{
						BOOL result = GetCharWidthA(hdc, 0, static_cast<UINT>(fontWidths.size() - 1), fontWidths.data());
						if (!result) ErrorHandler::Trigger("Failed to determine any font widths");
					}
					else
					{
						for (int i = 0; i < fontWidths.size(); i++)
							fontWidths[i] = trueTypeFontWidths[i].abcB;
					}

					// Insert everything all at once
					for (auto& [display, value] : *_DeferredMenuItems)
					{
						LRESULT index = SendMessageA(control, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(display));
						int lineSize = 0;

						if (index != CB_ERR && index != CB_ERRSPACE)
							SendMessageA(control, CB_SETITEMDATA, index, reinterpret_cast<LPARAM>(value));

						for (const char* c = display; *c != '\0'; c++)
							lineSize += fontWidths[*c];

						finalWidth = std::max(finalWidth, lineSize);
						free(const_cast<char*>(display));
					}

					SuspendComboBoxUpdates(control, false);
					ReleaseDC(control, hdc);
				}

				// Resize to fit
				if (DeferredAllowResize)
				{
					LRESULT currentWidth = SendMessage(control, CB_GETDROPPEDWIDTH, 0, 0);

					if (finalWidth > currentWidth)
						SendMessage(control, CB_SETDROPPEDWIDTH, finalWidth, 0);
				}
			}

			ResetUIDefer();
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

			if (seditorUI.UseDeferredDialogInsert)
			{
				if (seditorUI.DeferredComboBox && (seditorUI.DeferredComboBox != ComboBoxHandle))
					ErrorHandler::Trigger("Got handles to different combo boxes? Reset probably wasn't called.");

				seditorUI.DeferredComboBox = ComboBoxHandle;
				seditorUI.DeferredStringLength += strlen(DisplayText) + 1;
				seditorUI.DeferredAllowResize |= AllowResize;

				// A copy must be created since lifetime isn't guaranteed after this function returns
				seditorUI.GetDeferredMenuItems()->emplace_back(strdup(DisplayText), Value);
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
			return CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, nX, nY, nWidth, nHeight,
					(HWND)hWndParent, (HMENU)hMenu, (HINSTANCE)hInstance, lpParam);
		}

		void* EditorUI::Hook::HKCreateDialogParamA(void* hInstance, const char* lpTemplateName, void* hWndParent,
			std::uintptr_t lpDialogFunc, std::ptrdiff_t dwInitParam) noexcept(true)
		{
			// EndDialog MUST NOT be used
			ThreadDialogData.DialogFunc = (DLGPROC)lpDialogFunc;
			ThreadDialogData.IsDialog = false;

			// Override certain default dialogs to use this DLL's resources
			//switch (reinterpret_cast<uintptr_t>(lpTemplateName))
			//{
			//case 0x64:// "About"
			//	lpTemplateName = (LPCSTR)0xEB;
			//	ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc2;
			//	hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
			//	break;
			//case 0xEB:// "Logo"
			//	lpTemplateName = (LPCSTR)0xEB;
			//	ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc;
			//	hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
			//	break;
			//}

			//_MESSAGE("DBG dialog: %X(%u) %p", (DWORD)lpTemplateName, (DWORD)lpTemplateName, lpDialogFunc);

			//auto dialog = Core::GlobalDialogManagerPtr->GetDialog(reinterpret_cast<LONG_PTR>(lpTemplateName));
			//if (dialog)
			//	return Core::GlobalRegistratorWindowPtr->Register(
			//		dialog->Show(hWndParent, DialogFuncOverride, dwInitParam, hInstance));

			return CreateDialogParamA((HINSTANCE)hInstance, lpTemplateName, (HWND)hWndParent, 
				(DLGPROC)DialogFuncOverride, dwInitParam);
		}

		std::ptrdiff_t EditorUI::Hook::HKDialogBoxParamA(void* hInstance, const char* lpTemplateName, void* hWndParent,
			std::uintptr_t lpDialogFunc, std::ptrdiff_t dwInitParam) noexcept(true)
		{
			// EndDialog MUST be used
			ThreadDialogData.DialogFunc = (DLGPROC)lpDialogFunc;
			ThreadDialogData.IsDialog = true;

			// Override certain default dialogs to use this DLL's resources
			//switch (reinterpret_cast<uintptr_t>(lpTemplateName))
			//{
			//case 0x64:// "About"
			//	lpTemplateName = (LPCSTR)0xEB;
			//	ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc2;
			//	hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
			//	break;
			//case 0xEB:// "Logo"
			//	lpTemplateName = (LPCSTR)0xEB;
			//	ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc;
			//	hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
			//	break;
			//}

			//_MESSAGE("DBG dialog modal: %X(%u) %p", (DWORD)lpTemplateName, (DWORD)lpTemplateName, lpDialogFunc);

			//auto dialog = Core::GlobalDialogManagerPtr->GetDialog(reinterpret_cast<ULONG_PTR>(lpTemplateName));
			//if (dialog)
			//	return dialog->ShowModal(hWndParent, DialogFuncOverride, dwInitParam, hInstance);

			return DialogBoxParamA((HINSTANCE)hInstance, lpTemplateName, (HWND)hWndParent, 
				(DLGPROC)DialogFuncOverride, dwInitParam);
		}

		bool EditorUI::Hook::HKEndDialog(void* hDlg, std::ptrdiff_t nResult) noexcept(true)
		{
			ScopeCriticalSection lock{ DialogLock };

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

		void EditorUI::Hook::HKResetUIDefer() noexcept(true)
		{
			seditorUI.ResetUIDefer();
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

			// TODO need only wm_Create
			//if (UITheme::IsDarkTheme())
			//	Patches::UIThemePatch::ApplyThemeForWindow(hwndDlg);

			return proc((HWND)hwndDlg, uMsg, wParam, lParam);
		}

		void EditorUI::Hook::Initialize() noexcept(true)
		{
			auto base = (std::uintptr_t)GetModuleHandleA(nullptr);

			Detours::DetourIAT(base, "USER32.DLL", "CreateDialogParamA", (std::uintptr_t)HKCreateDialogParamA);
			Detours::DetourIAT(base, "USER32.DLL", "DialogBoxParamA", (std::uintptr_t)HKDialogBoxParamA);
			Detours::DetourIAT(base, "USER32.DLL", "EndDialog", (std::uintptr_t)HKEndDialog);
			Detours::DetourIAT(base, "USER32.DLL", "SendMessageA", (std::uintptr_t)HKSendMessageA);
			Detours::DetourIAT(base, "USER32.DLL", "CreateWindowA", (std::uintptr_t)HKCreateWindowA);
			Detours::DetourIAT(base, "USER32.DLL", "CreateWindowExA", (std::uintptr_t)HKCreateWindowExA);
			Detours::DetourIAT(base, "USER32.DLL", "DestroyWindow", (std::uintptr_t)HKDestroyWindow);
		}
	}
}