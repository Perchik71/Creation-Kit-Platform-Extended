// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BaseWindow.h"
#include "Editor API/FO4/TESF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			class ResponseWindow : public BaseWindow, public Classes::CUIBaseWindow
			{
			public:
				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				ResponseWindow();

				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void sub(HWND hWndButtonGenerate);

				static void ModuleInit();
				static bool ExecuteApplication(const char* CmdLine, bool Hide = true, uint32_t Timeout = INFINITE);
				static bool GenerationLip(const char* AudioPath, const char* LipPath, const char* ResponseText);
				static bool GenerationFuz(const char* FuzPath, const char* AudioPath, const char* LipPath);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				::Core::Classes::UI::CUIBaseControl ListViewItems;
			};

			extern ResponseWindow* GlobalResponseWindowPtr;
		}
	}
}