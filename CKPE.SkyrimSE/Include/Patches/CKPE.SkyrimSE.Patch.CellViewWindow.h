// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESObjectREFR.h>
#include <CKPE.Common.PatchBaseWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class CellViewWindow : public Common::PatchBaseWindow
			{
				Common::UI::CUIBaseControl m_WorldSpaceLabel;
				Common::UI::CUIBaseControl m_WorldSpaceComboBox;
				Common::UI::CUIBaseControl m_XEdit;
				Common::UI::CUIBaseControl m_YEdit;
				Common::UI::CUIBaseControl m_XLabel;
				Common::UI::CUIBaseControl m_YLabel;
				Common::UI::CUIBaseControl m_GoButton;
				Common::UI::CUIBaseControl m_LoadedAtTop;
				Common::UI::CUIBaseControl m_IdEdit;
				Common::UI::CUIBaseControl m_SelectedObjectLabel;
				Common::UI::CUIBaseControl m_ActiveCellsOnly;
				Common::UI::CUIBaseControl m_ActiveObjectsOnly;
				Common::UI::CUIBaseControl m_SelectObjectsOnly;
				Common::UI::CUIBaseControl m_VisibleObjectsOnly;
				Common::UI::CUIBaseControl m_CellListView;
				Common::UI::CUIBaseControl m_ObjectListView;
				Common::UI::CUIBaseControl m_FilterCellEdit;
				bool lock;

				CellViewWindow(const CellViewWindow&) = delete;
				CellViewWindow& operator=(const CellViewWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CellViewWindow();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true);
				static void sub1(HWND ListViewHandle, EditorAPI::Forms::TESForm* Form, bool UseImage, std::int32_t ItemIndex) noexcept(true);
				static std::int32_t sub2(HWND** ListViewHandle, EditorAPI::Forms::TESForm** Form, std::int64_t a3) noexcept(true);
				static std::int64_t sub3(std::int64_t a1, std::int64_t a2, std::int64_t a3) noexcept(true);

				void ResizeWnd(std::uint32_t width, std::uint32_t height) noexcept(true);
				inline void LockUpdateLists() noexcept(true) { lock = true; }
				inline void UnlockUpdateLists() noexcept(true) { lock = false; }
				void UpdateCellList() noexcept(true);
				void UpdateObjectList() noexcept(true);
			};
		}
	}
}