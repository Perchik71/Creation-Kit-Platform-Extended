// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
//#include 
#include <CKPE.Common.PatchBaseWindow.h>

namespace CKPE
{
	namespace Starfield
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
				Common::UI::CUIBaseControl m_FilterCellLabel;
				Common::UI::CUIBaseControl m_GoButton;
				Common::UI::CUIBaseControl m_LoadedAtTop;
				Common::UI::CUIBaseControl m_FilteredOnly;
				Common::UI::CUIBaseControl m_VisibleOnly;
				Common::UI::CUIBaseControl m_SelectedOnly;
				Common::UI::CUIBaseControl m_ActiveLayerOnly;
				Common::UI::CUIBaseControl m_ShowNavmesh;
				Common::UI::CUIBaseControl m_SpecificFormTypes;
				Common::UI::CUIBaseControl m_IdEdit;
				Common::UI::CUIBaseControl m_SelectedObjectLabel;
				Common::UI::CUIBaseControl m_ActiveCellsOnly;
				Common::UI::CUIBaseControl m_ActiveObjectsOnly;
				Common::UI::CUIBaseControl m_CellListView;
				Common::UI::CUIBaseControl m_ObjectListView;
				Common::UI::CUIBaseControl m_FilterCellEdit;
				Common::UI::CUIBaseControl m_FilterWorldspaceEdit;
				Common::UI::CUIBaseControl m_UnloadButton;
				Common::UI::CUIBaseControl m_SelectTypesButton;

				void ResizeWnd(UINT width, UINT height) noexcept(true);

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

				inline static ISingleton<CellViewWindow> Singleton;
				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true);
				//static void sub1(HWND ListViewHandle, TESForm* Form, bool UseImage, int ItemIndex);
				//static void sub2(HWND ListViewHandle, TESForm* Form);
				//static bool sub3(TESForm* Form);
			};
		}
	}
}