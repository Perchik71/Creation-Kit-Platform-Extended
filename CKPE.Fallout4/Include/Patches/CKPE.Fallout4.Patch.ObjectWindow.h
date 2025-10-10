// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESObjectREFR.h>
#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>
#include <CKPE.Common.UICheckboxControl.h>
#include <CKPE.ImageList.h>
#include <unordered_map>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			typedef struct tagOBJWND_CONTROLS
			{
				Common::UI::CUIBaseControl TreeList;
				Common::UI::CUIBaseControl ItemList;
				Common::UI::CUIBaseControl ToggleDecompose;
				Common::UI::CUIBaseControl BtnObjLayout;
				Common::UI::CUIBaseControl ComboLayout;
				Common::UI::CUIBaseControl EditFilter;
				Common::UI::CUIBaseControl Spliter;
				Common::UI::CUICheckbox ActiveOnly;
			} OBJWND_CONTROLS, *POBJWND_CONTROLS, *LPOBJWND_CONTROLS;

			typedef struct tagOBJWND
			{
				bool StartResize;
				OBJWND_CONTROLS Controls;
				Common::UI::CUICustomWindow ObjectWindow;
			} OBJWND, * POBJWND, * LPOBJWND;

			typedef std::unordered_map<HWND, LPOBJWND> OBJWNDS;
			extern OBJWNDS ObjectWindows;

			class ObjectWindow : public Common::PatchBaseWindow
			{
				static std::int32_t sub(std::int64_t ObjectListInsertData, void* Form) noexcept(true);
				static std::int32_t sub2(HWND Hwnd, void* Form, const char* filterText) noexcept(true);

				ObjectWindow(const ObjectWindow&) = delete;
				ObjectWindow& operator=(const ObjectWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ObjectWindow();

				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static BOOL WINAPI HKMoveWindow(HWND hWindow, INT32 X, INT32 Y, INT32 nWidth, INT32 nHeight, BOOL bRepaint);

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}