// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "EditorUI.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		EditorUI* GlobalEditorUIPtr = nullptr;

		EditorUI::EditorUI() : UseDeferredDialogInsert(false), DeferredListView(nullptr),
			DeferredComboBox(nullptr), DeferredStringLength(0), DeferredAllowResize(false)
		{
			InitCommonControls();
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
	}
}