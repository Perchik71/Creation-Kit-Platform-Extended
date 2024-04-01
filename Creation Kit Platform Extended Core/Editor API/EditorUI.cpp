// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/AboutWindow.h"
#include "Core/DialogManager.h"
#include "Core/RegistratorWindow.h"
#include "EditorUI.h"
#include "..\NiAPI\NiMemoryManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		struct DialogOverrideData
		{
			DLGPROC DialogFunc;	// Original function pointer
			bool IsDialog;		// True if it requires EndDialog()
		};

		std::recursive_mutex DialogOverrideMutex;
		UnorderedMap<HWND, DialogOverrideData> DialogOverrides;
		thread_local DialogOverrideData ThreadDialogData;

		EditorUI* GlobalEditorUIPtr = nullptr;

		EditorUI::EditorUI() : _UseDeferredDialogInsert(false), _DeferredListView(nullptr),
			_DeferredComboBox(nullptr), _DeferredStringLength(0), _DeferredAllowResize(false)
		{
			InitCommonControls();
		}

		void* EditorUI::ListViewGetSelectedItem(HWND ListViewHandle)
		{
			if (!ListViewHandle)
				return NULL;

			int index = ListView_GetNextItem(ListViewHandle, -1, LVNI_SELECTED);

			if (index == -1)
				return NULL;

			LVITEMA item = { 0 };
			item.mask = LVIF_PARAM;
			item.iItem = index;

			ListView_GetItem(ListViewHandle, &item);
			return (void*)item.lParam;
		}

		void EditorUI::ComboBoxInsertItemDeferred(HWND ComboBoxHandle, const char* DisplayText,
			void* Value, bool AllowResize)
		{
			if (!ComboBoxHandle)
				return;

			if (!DisplayText)
				DisplayText = "NONE";

			if (GlobalEditorUIPtr->UseDeferredDialogInsert)
			{
				AssertMsg(!GlobalEditorUIPtr->DeferredComboBox || (GlobalEditorUIPtr->DeferredComboBox == ComboBoxHandle),
					"Got handles to different combo boxes? Reset probably wasn't called.");

				GlobalEditorUIPtr->DeferredComboBox = ComboBoxHandle;
				GlobalEditorUIPtr->DeferredStringLength += strlen(DisplayText) + 1;
				GlobalEditorUIPtr->DeferredAllowResize |= AllowResize;

				// A copy must be created since lifetime isn't guaranteed after this function returns
				GlobalEditorUIPtr->GetDeferredMenuItems().emplace_back(Utils::StrDub(DisplayText), Value);
			}
			else
			{
				if (AllowResize)
				{
					if (HDC hdc = GetDC(ComboBoxHandle); hdc)
					{
						if (SIZE size; GetTextExtentPoint32A(hdc, DisplayText, static_cast<int>(strlen(DisplayText)), &size))
						{
							LRESULT currentWidth = SendMessageA(ComboBoxHandle, CB_GETDROPPEDWIDTH, 0, 0);

							if (size.cx > currentWidth)
								SendMessageA(ComboBoxHandle, CB_SETDROPPEDWIDTH, size.cx, 0);
						}

						ReleaseDC(ComboBoxHandle, hdc);
					}
				}

				LRESULT index = SendMessageA(ComboBoxHandle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(DisplayText));

				if (index != CB_ERR && index != CB_ERRSPACE)
					SendMessageA(ComboBoxHandle, CB_SETITEMDATA, index, reinterpret_cast<LPARAM>(Value));
			}
		}

		void EditorUI::ListViewInsertItemDeferred(HWND ListViewHandle, void* Parameter,
			bool UseImage, int ItemIndex)
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

			if (GlobalEditorUIPtr->UseDeferredDialogInsert)
			{
				AssertMsg(!GlobalEditorUIPtr->DeferredListView || (GlobalEditorUIPtr->DeferredListView == ListViewHandle),
					"Got handles to different list views? Reset probably wasn't called.");

				if (!GlobalEditorUIPtr->DeferredListView)
				{
					GlobalEditorUIPtr->DeferredListView = ListViewHandle;
					SendMessage(ListViewHandle, WM_SETREDRAW, FALSE, 0);
				}
			}

			ListView_InsertItem(ListViewHandle, &item);
		}

		void EditorUI::ResetUIDefer()
		{
			UseDeferredDialogInsert = false;
			DeferredListView = nullptr;
			DeferredComboBox = nullptr;
			DeferredStringLength = 0;
			DeferredAllowResize = false;
			DeferredMenuItems.clear();
		}

		void EditorUI::BeginUIDefer()
		{
			ResetUIDefer();
			UseDeferredDialogInsert = true;
		}

		void EditorUI::EndUIDefer()
		{
			if (!UseDeferredDialogInsert)
				return;

			if (DeferredListView)
			{
				SendMessage(DeferredListView, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(DeferredListView, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			if (!DeferredMenuItems.empty())
			{
				const HWND control = DeferredComboBox;

				// Sort alphabetically if requested to try and speed up inserts
				int finalWidth = 0;
				LONG_PTR style = GetWindowLongPtr(control, GWL_STYLE);

				if ((style & CBS_SORT) == CBS_SORT)
				{
					std::sort(DeferredMenuItems.begin(), DeferredMenuItems.end(),
						[](const auto& a, const auto& b) -> bool
						{
							return _stricmp(a.first, b.first) > 0;
						});
				}

				SendMessage(control, CB_INITSTORAGE, DeferredMenuItems.size(), DeferredStringLength * sizeof(char));

				if (HDC hdc = GetDC(control); hdc)
				{
					SuspendComboBoxUpdates(control, true);

					// Pre-calculate font widths for resizing, starting with TrueType
					std::array<int, UCHAR_MAX + 1> fontWidths = { 0 };
					std::array<ABC, UCHAR_MAX + 1> trueTypeFontWidths = { 0 };

					if (!GetCharABCWidthsA(hdc, 0, static_cast<UINT>(trueTypeFontWidths.size() - 1), trueTypeFontWidths.data()))
					{
						BOOL result = GetCharWidthA(hdc, 0, static_cast<UINT>(fontWidths.size() - 1), fontWidths.data());
						AssertMsg(result, "Failed to determine any font widths");
					}
					else
					{
						for (int i = 0; i < fontWidths.size(); i++)
							fontWidths[i] = trueTypeFontWidths[i].abcB;
					}

					// Insert everything all at once
					for (auto [display, value] : DeferredMenuItems)
					{
						LRESULT index = SendMessageA(control, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(display));
						int lineSize = 0;

						if (index != CB_ERR && index != CB_ERRSPACE)
							SendMessageA(control, CB_SETITEMDATA, index, reinterpret_cast<LPARAM>(value));

						for (const char* c = display; *c != '\0'; c++)
							lineSize += fontWidths[*c];

						finalWidth = std::max(finalWidth, lineSize);

						NiAPI::NiMemoryManager::Free(nullptr, const_cast<char*>(display));
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

		void EditorUI::SuspendComboBoxUpdates(HWND ComboHandle, bool Suspend)
		{
			COMBOBOXINFO info
			{
				.cbSize = sizeof(COMBOBOXINFO)
			};

			if (!GetComboBoxInfo(ComboHandle, &info))
				return;

			if (!Suspend)
			{
				SendMessage(info.hwndList, WM_SETREDRAW, TRUE, 0);
				SendMessage(ComboHandle, CB_SETMINVISIBLE, 30, 0);
				SendMessage(ComboHandle, WM_SETREDRAW, TRUE, 0);
			}
			else
			{
				SendMessage(ComboHandle, WM_SETREDRAW, FALSE, 0);	// Prevent repainting until finished
				SendMessage(ComboHandle, CB_SETMINVISIBLE, 1, 0);	// Possible optimization for older libraries (source: MSDN forums)
				SendMessage(info.hwndList, WM_SETREDRAW, FALSE, 0);
			}
		}

		void EditorUI::HKResetUIDefer()
		{
			GlobalEditorUIPtr->ResetUIDefer();
		}

		void EditorUI::HKBeginUIDefer()
		{
			GlobalEditorUIPtr->BeginUIDefer();
		}

		void EditorUI::HKEndUIDefer()
		{
			GlobalEditorUIPtr->EndUIDefer();
		}

		INT_PTR CALLBACK EditorUI::DialogFuncOverride(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			DLGPROC proc = nullptr;

			DialogOverrideMutex.lock();
			{
				if (auto itr = DialogOverrides.find(hwndDlg); itr != DialogOverrides.end())
					proc = itr->second.DialogFunc;

				// if (is new entry)
				if (!proc)
				{
					DialogOverrides[hwndDlg] = ThreadDialogData;
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
			DialogOverrideMutex.unlock();

			return proc(hwndDlg, uMsg, wParam, lParam);
		}

		HWND EditorUI::HKCreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, 
			HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
		{
			// EndDialog MUST NOT be used
			ThreadDialogData.DialogFunc = lpDialogFunc;
			ThreadDialogData.IsDialog = false;
		
			// Override certain default dialogs to use this DLL's resources
			switch (reinterpret_cast<uintptr_t>(lpTemplateName))
			{
			case 0x64:// "About"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc2;
				hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
				break;
			case 0xEB:// "Logo"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc;
				hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
				break;
			}

			auto dialog = Core::GlobalDialogManagerPtr->GetDialog(reinterpret_cast<LONG_PTR>(lpTemplateName));
			if (dialog)
				return Core::GlobalRegistratorWindowPtr->Register(
					dialog->Show(hWndParent, DialogFuncOverride, dwInitParam, hInstance));

			return Core::GlobalRegistratorWindowPtr->Register(
				CreateDialogParamA(hInstance, lpTemplateName, hWndParent, DialogFuncOverride, dwInitParam));
		}

		INT_PTR EditorUI::HKDialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, 
			DLGPROC lpDialogFunc, LPARAM dwInitParam)
		{
			// EndDialog MUST be used
			ThreadDialogData.DialogFunc = lpDialogFunc;
			ThreadDialogData.IsDialog = true;

			// Override certain default dialogs to use this DLL's resources
			switch (reinterpret_cast<uintptr_t>(lpTemplateName))
			{
			case 0x64:// "About"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc2;
				hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
				break;
			case 0xEB:// "Logo"
				lpTemplateName = (LPCSTR)0xEB;
				ThreadDialogData.DialogFunc = Core::AboutWindow::WndProc;
				hInstance = Core::GlobalEnginePtr->GetInstanceDLL();
				break;
			}

			auto dialog = Core::GlobalDialogManagerPtr->GetDialog(reinterpret_cast<ULONG_PTR>(lpTemplateName));
			if (dialog)
				return dialog->ShowModal(hWndParent, DialogFuncOverride, dwInitParam, hInstance);

			return DialogBoxParamA(hInstance, lpTemplateName, hWndParent, DialogFuncOverride, dwInitParam);
		}

		BOOL EditorUI::HKEndDialog(HWND hDlg, INT_PTR nResult)
		{
			std::lock_guard lock(DialogOverrideMutex);

			// Fix for the CK calling EndDialog on a CreateDialogParamA window
			if (auto itr = DialogOverrides.find(hDlg); itr != DialogOverrides.end() && !itr->second.IsDialog)
			{
				Core::GlobalRegistratorWindowPtr->Unregister(hDlg);
				DestroyWindow(hDlg);
				return TRUE;
			}

			return EndDialog(hDlg, nResult);
		}

		LRESULT EditorUI::HKSendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
		{
			if (hWnd && Msg == WM_DESTROY)
			{
				std::lock_guard lock(DialogOverrideMutex);

				// If this is a dialog, we can't call DestroyWindow on it
				if (auto itr = DialogOverrides.find(hWnd); itr != DialogOverrides.end())
				{
					if (!itr->second.IsDialog)
					{
						Core::GlobalRegistratorWindowPtr->Unregister(hWnd);
						DestroyWindow(hWnd);
					}
				}

				return 0;
			}

			return SendMessageA(hWnd, Msg, wParam, lParam);
		}
	}
}