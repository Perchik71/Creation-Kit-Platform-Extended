// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <vector>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API EditorUI
		{
			bool _UseDeferredDialogInsert{ false };
			void* _DeferredListView { nullptr };
			void* _DeferredComboBox{ nullptr };
			std::uintptr_t _DeferredStringLength{ 0 };
			bool _DeferredAllowResize{ false };
			std::vector<std::pair<const char*, void*>>* _DeferredMenuItems{ nullptr };

			EditorUI(const EditorUI&) = delete;
			EditorUI& operator=(const EditorUI&) = delete;
		public:
			constexpr static const char* UI_USER_DATA_ACTIVE_CELLS_ONLY = "ActiveCellsOnly";
			constexpr static const char* UI_USER_DATA_FILTER_CELLS_LEN = "FilterCellsLen";
			constexpr static const char* UI_USER_DATA_ACTIVE_OBJECT_ONLY = "ActiveObjectsOnly";
			constexpr static const char* UI_USER_DATA_SELECT_OBJECT_ONLY = "SelectObjectsOnly";
			constexpr static const char* UI_USER_DATA_VISIBLE_OBJECT_ONLY = "VisibleObjectsOnly";

			constexpr static const char* UI_EDITOR_CHANGEBASEFORM_STR = "Replace base for select Ref's";

			constexpr static std::uint32_t UI_EDITOR_TOOLBAR = 1;
			constexpr static std::uint32_t UI_EDITOR_STATUSBAR = 40139;
			constexpr static std::uint32_t UI_EDITOR_TOGGLEOBJECTWND = 40199;		// Object Window's
			constexpr static std::uint32_t UI_EDITOR_TOGGLECELLVIEW = 40200;		// Cell View
			constexpr static std::uint32_t UI_EDITOR_TOGGLEMAKERS = 40030;			// "View" menu
			constexpr static std::uint32_t UI_EDITOR_TOGGLEFOG = 40937;				// "View" menu
			constexpr static std::uint32_t UI_EDITOR_TOGGLEGRASS_BUTTON = 40960;	// Main toolbar
			constexpr static std::uint32_t UI_EDITOR_TOGGLEGRASS = 40963;			// "View" menu
			constexpr static std::uint32_t UI_EDITOR_OPENFORMBYID = 52001;			// Sent from the LogWindow on double click
			constexpr static std::uint32_t UI_EDITOR_CHANGEBASEFORM = 40670;		// "Change Base Form"

			constexpr static std::uint32_t UI_DATA_DIALOG_PLUGINLISTVIEW = 1056;

			EditorUI() noexcept(true);

			[[nodiscard]] static EditorUI* GetSingleton() noexcept(true);
			[[nodiscard]] static void* ListViewGetSelectedItem(void* ListViewHandle) noexcept(true);
			static bool ListViewSetItemState(void* ListViewHandle, std::ptrdiff_t Index, 
				std::uint32_t Data, std::uint32_t Mask) noexcept(true);
			static void ListViewSelectItem(void* ListViewHandle, int ItemIndex, 
				bool KeepOtherSelections) noexcept(true);
			static void ListViewFindAndSelectItem(void* ListViewHandle, void* Parameter, 
				bool KeepOtherSelections) noexcept(true);
			static void ListViewDeselectItem(void* ListViewHandle, void* Parameter) noexcept(true);
			static void ListViewInsertItemDeferred(void* ListViewHandle, void* Parameter,
				bool UseImage, int ItemIndex) noexcept(true);

			[[nodiscard]] inline bool HasUseDeferredDialogInsert() const noexcept(true) { return _UseDeferredDialogInsert; }
			[[nodiscard]] inline void* GetDeferredListView() const noexcept(true) { return _DeferredListView; }
			[[nodiscard]] inline void* GetDeferredComboBox() const noexcept(true) { return _DeferredComboBox; }
			[[nodiscard]] inline std::uintptr_t GetDeferredStringLength() const noexcept(true) { return _DeferredStringLength; }
			[[nodiscard]] inline bool HasDeferredAllowResize() const noexcept(true) { return _DeferredAllowResize; }
			[[nodiscard]] inline std::vector<std::pair<const char*, void*>>* GetDeferredMenuItems() noexcept(true) 
			{ return _DeferredMenuItems; }
			inline void SetUseDeferredDialogInsert(bool v) noexcept(true) { _UseDeferredDialogInsert = v; }
			inline void SetDeferredListView(void* v) noexcept(true) { _DeferredListView = v; }
			inline void SetDeferredComboBox(void* v) noexcept(true) { _DeferredComboBox = v; }
			inline void SetDeferredStringLength(std::uintptr_t v) noexcept(true) { _DeferredStringLength = v; }
			inline void SetDeferredAllowResize(bool v) noexcept(true) { _DeferredAllowResize = v; }

			CKPE_PROPERTY(HasUseDeferredDialogInsert, SetUseDeferredDialogInsert) bool UseDeferredDialogInsert;
			CKPE_PROPERTY(GetDeferredListView, SetDeferredListView) void* DeferredListView;
			CKPE_PROPERTY(GetDeferredComboBox, SetDeferredComboBox) void* DeferredComboBox;
			CKPE_PROPERTY(GetDeferredStringLength, SetDeferredStringLength) std::uintptr_t DeferredStringLength;
			CKPE_PROPERTY(HasDeferredAllowResize, SetDeferredAllowResize) bool DeferredAllowResize;

			void ResetUIDefer() noexcept(true);
			void BeginUIDefer() noexcept(true);
			void EndUIDefer() noexcept(true);

			static void SuspendComboBoxUpdates(void* ComboHandle, bool Suspend) noexcept(true);
			static void ComboBoxInsertItemDeferred(void* ComboBoxHandle, const char* DisplayText,
				void* Value, bool AllowResize) noexcept(true);
		public:
			struct CKPE_COMMON_API Hook
			{
				static void* HKCreateWindowA(const char* lpClassName, const char* lpWindowName,
					std::uint32_t dwStyle, std::int32_t nX, std::int32_t nY, std::int32_t nWidth, std::int32_t nHeight, 
					void* hWndParent, void* hMenu, void* hInstance, void* lpParam) noexcept(true);
				static void* HKCreateWindowExA(std::uint32_t dwExStyle, const char* lpClassName, const char* lpWindowName,
					std::uint32_t dwStyle, std::int32_t nX, std::int32_t nY, std::int32_t nWidth, std::int32_t nHeight,
					void* hWndParent, void* hMenu, void* hInstance, void* lpParam) noexcept(true);
				static void* HKCreateDialogParamA(void* hInstance, const char* lpTemplateName, void* hWndParent,
					std::uintptr_t lpDialogFunc, std::ptrdiff_t dwInitParam) noexcept(true);
				static std::ptrdiff_t HKDialogBoxParamA(void* hInstance, const char* lpTemplateName, void* hWndParent,
					std::uintptr_t lpDialogFunc, std::ptrdiff_t dwInitParam) noexcept(true);
				static bool HKEndDialog(void* hDlg, std::ptrdiff_t nResult) noexcept(true);
				static bool HKDestroyWindow(void* hDlg) noexcept(true);
				static std::ptrdiff_t HKSendMessageA(void* hWnd, std::uint32_t Msg, std::ptrdiff_t wParam, 
					std::ptrdiff_t lParam) noexcept(true);
				static std::ptrdiff_t DialogFuncOverride(void* hwndDlg, std::uint32_t uMsg, std::ptrdiff_t wParam, 
					std::ptrdiff_t lParam) noexcept(true);
				static void HKResetUIDefer() noexcept(true);
				static void HKBeginUIDefer() noexcept(true);
				static void HKEndUIDefer() noexcept(true);

				static void Initialize() noexcept(true);
			};
		};
	}
}