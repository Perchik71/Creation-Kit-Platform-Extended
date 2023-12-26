// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BaseWindow.h"
#include "Editor API/UI/UICheckboxControl.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			class ObjectWindow : public BaseWindow, public Classes::CUIBaseWindow
			{
			public:
				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				ObjectWindow();

				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static BOOL WINAPI HKMoveWindow(HWND hWindow, INT32 X, INT32 Y, INT32 nWidth, INT32 nHeight, BOOL bRepaint);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			};

			typedef struct tagOBJWND_CONTROLS
			{
				Classes::CUIBaseControl TreeList;
				Classes::CUIBaseControl ItemList;
				Classes::CUIBaseControl EditFilter;
				Classes::CUIBaseControl Spliter;
				Classes::CUICheckbox ActiveOnly;
			} OBJWND_CONTROLS, *POBJWND_CONTROLS, *LPOBJWND_CONTROLS;

			typedef struct tagOBJWND
			{
				BOOL StartResize;
				OBJWND_CONTROLS Controls;
				Classes::CUICustomWindow ObjectWindow;
			} OBJWND, *POBJWND, *LPOBJWND;

			typedef UnorderedMap<HWND, LPOBJWND> OBJWNDS;
			extern OBJWNDS ObjectWindows;
		}
	}
}