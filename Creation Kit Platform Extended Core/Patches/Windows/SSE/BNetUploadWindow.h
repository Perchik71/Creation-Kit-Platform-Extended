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
			class BNetUploadWindow : public BaseWindow, public Classes::CUIBaseWindow
			{
			public:
				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				BNetUploadWindow();

				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void sub(int64_t a1, int64_t a2);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				::Core::Classes::UI::CUICheckbox _Platform_PC;
				::Core::Classes::UI::CUICheckbox _Platform_XBOX;
				::Core::Classes::UI::CUICheckbox _Platform_PS4;
				::Core::Classes::UI::CUIBaseControl _ModName_Platform_PC;
				::Core::Classes::UI::CUIBaseControl _Archives_Platform_PC;
				::Core::Classes::UI::CUICheckbox _IncludeArchives_Platform_PC;
				::Core::Classes::UI::CUICheckbox _CreateESL_Platform_PC;
				::Core::Classes::UI::CUIBaseControl _Browse_Platform_PC;
				::Core::Classes::UI::CUIBaseControl _Generate_Platform_PC;
				::Core::Classes::UI::CUIBaseControl _ModName_Platform_XBOX;
				::Core::Classes::UI::CUIBaseControl _Archives_Platform_XBOX;
				::Core::Classes::UI::CUICheckbox _IncludeArchives_Platform_XBOX;
				::Core::Classes::UI::CUICheckbox _CreateESL_Platform_XBOX;
				::Core::Classes::UI::CUIBaseControl _Browse_Platform_XBOX;
				::Core::Classes::UI::CUIBaseControl _Generate_Platform_XBOX;
				::Core::Classes::UI::CUIBaseControl _ModName_Platform_PS4;
				::Core::Classes::UI::CUICheckbox _ExcludeAssetsFiles;
				::Core::Classes::UI::CUICheckbox _CreateESL_Platform_PS4;
				::Core::Classes::UI::CUIBaseControl _Browse_Platform_PS4;
			};

			extern BNetUploadWindow* GlobalBNetUploadWindowPtr;
		}
	}
}