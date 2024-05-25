// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class EditorUI
		{
		public:
			constexpr static const char* UI_USER_DATA_ACTIVE_CELLS_ONLY = "ActiveCellsOnly";
			constexpr static const char* UI_USER_DATA_FILTER_CELLS_LEN = "FilterCellsLen";
			constexpr static const char* UI_USER_DATA_ACTIVE_OBJECT_ONLY = "ActiveObjectsOnly";
			constexpr static const char* UI_USER_DATA_SELECT_OBJECT_ONLY = "SelectObjectsOnly";
			constexpr static const char* UI_USER_DATA_VISIBLE_OBJECT_ONLY = "VisibleObjectsOnly";

			constexpr static const char* UI_EDITOR_CHANGEBASEFORM_STR = "Replace base for select Ref's";

			constexpr static uint32_t UI_EDITOR_TOOLBAR = 1;
			constexpr static uint32_t UI_EDITOR_STATUSBAR = 40139;
			constexpr static uint32_t UI_EDITOR_TOGGLEOBJECTWND = 40199;	// Object Window's
			constexpr static uint32_t UI_EDITOR_TOGGLECELLVIEW = 40200;		// Cell View
			constexpr static uint32_t UI_EDITOR_TOGGLEFOG = 40937;			// "View" menu
			constexpr static uint32_t UI_EDITOR_TOGGLEGRASS_BUTTON = 40960;	// Main toolbar
			constexpr static uint32_t UI_EDITOR_TOGGLEGRASS = 40963;		// "View" menu
			constexpr static uint32_t UI_EDITOR_OPENFORMBYID = 52001;		// Sent from the LogWindow on double click
			constexpr static uint32_t UI_EDITOR_CHANGEBASEFORM = 40670;		// "Change Base Form"

			constexpr static uint32_t UI_DATA_DIALOG_PLUGINLISTVIEW = 1056;

			EditorUI();

			inline bool HasUseDeferredDialogInsert() const { return _UseDeferredDialogInsert; }
			inline HWND GetDeferredListView() const { return _DeferredListView; }
			inline HWND GetDeferredComboBox() const { return _DeferredComboBox; }
			inline uintptr_t GetDeferredStringLength() const { return _DeferredStringLength; }
			inline bool HasDeferredAllowResize() const { return _DeferredAllowResize; }
			inline void SetUseDeferredDialogInsert(bool v) { _UseDeferredDialogInsert = v; }
			inline void SetDeferredListView(HWND v) { _DeferredListView = v; }
			inline void SetDeferredComboBox(HWND v) { _DeferredComboBox = v; }
			inline void SetDeferredStringLength(uintptr_t v) { _DeferredStringLength = v; }
			inline void SetDeferredAllowResize(bool v) { _DeferredAllowResize = v; }
			inline Array<std::pair<const char*, void*>>& GetDeferredMenuItems() { return DeferredMenuItems; }

			PROPERTY(HasUseDeferredDialogInsert, SetUseDeferredDialogInsert) bool UseDeferredDialogInsert;
			PROPERTY(GetDeferredListView, SetDeferredListView) HWND DeferredListView;
			PROPERTY(GetDeferredComboBox, SetDeferredComboBox) HWND DeferredComboBox;
			PROPERTY(GetDeferredStringLength, SetDeferredStringLength) uintptr_t DeferredStringLength;
			PROPERTY(HasDeferredAllowResize, SetDeferredAllowResize) bool DeferredAllowResize;

			void ResetUIDefer();
			void BeginUIDefer();
			void EndUIDefer();
			void SuspendComboBoxUpdates(HWND ComboHandle, bool Suspend);
		public:
			static HWND HKCreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, 
				DLGPROC lpDialogFunc, LPARAM dwInitParam);
			static INT_PTR HKDialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, 
				DLGPROC lpDialogFunc, LPARAM dwInitParam);
			static BOOL HKEndDialog(HWND hDlg, INT_PTR nResult);
			static LRESULT HKSendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
			static INT_PTR CALLBACK DialogFuncOverride(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static void HKResetUIDefer();
			static void HKBeginUIDefer();
			static void HKEndUIDefer();
			static void ComboBoxInsertItemDeferred(HWND ComboBoxHandle, const char* DisplayText,
				void* Value, bool AllowResize);
			static void ListViewInsertItemDeferred(HWND ListViewHandle, void* Parameter,
				bool UseImage, int ItemIndex);
			static void* ListViewGetSelectedItem(HWND ListViewHandle);
			static bool ListViewSetItemState(HWND ListViewHandle, WPARAM Index, UINT Data, UINT Mask);
			static void ListViewSelectItem(HWND ListViewHandle, int ItemIndex, bool KeepOtherSelections);
			static void ListViewFindAndSelectItem(HWND ListViewHandle, void* Parameter, bool KeepOtherSelections);
			static void ListViewDeselectItem(HWND ListViewHandle, void* Parameter);
		private:
			EditorUI(const EditorUI&) = default;
			EditorUI& operator=(const EditorUI&) = default;

			bool _UseDeferredDialogInsert;
			HWND _DeferredListView;
			HWND _DeferredComboBox;
			uintptr_t _DeferredStringLength;
			bool _DeferredAllowResize;
			Array<std::pair<const char*, void*>> DeferredMenuItems;
		};

		extern EditorUI* GlobalEditorUIPtr;
	}
}