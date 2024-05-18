// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixFog.h"
#include "Patches/Windows/FO4/MainWindowF4.h"
#include "Editor API/EditorUI.h"
#include "Editor API/FO4/TESF4.h"


namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			bool* globalFogEnabled = nullptr;
			static uintptr_t function_FixFogPatch_sub02 = 0;

			FixFogPatch::FixFogPatch() : Module(GlobalEnginePtr)
			{}

			bool FixFogPatch::HasOption() const
			{
				return false;
			}

			bool FixFogPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixFogPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixFogPatch::GetName() const
			{
				return "Fix Fog";
			}

			bool FixFogPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixFogPatch::GetDependencies() const
			{
				return {};
			}

			bool FixFogPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixFogPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					globalFogEnabled = (bool*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					// Replace hotkey
					function_FixFogPatch_sub02 =
						lpRelocator->DetourFunctionClass(lpRelocationDatabaseItem->At(1), (uintptr_t)&ToggleFog);
					// Initializing fog from an setting
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&InitializeFogSettings);
					
					return true;
				}

				return false;
			}

			bool FixFogPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool FixFogPatch::IsFogEnabled()
			{
				return *globalFogEnabled;
			}

			void FixFogPatch::ToggleFog()
			{
				fastCall<void>(function_FixFogPatch_sub02);

				CheckMenuItem(GlobalMainWindowPtr->MainMenu.Handle,
					EditorAPI::EditorUI::UI_EDITOR_TOGGLEFOG,
					*globalFogEnabled ? MF_CHECKED : MF_UNCHECKED);

				Utils::UpdateProfileValue("CreationKitPrefs.ini", "Display", "bFogEnabled", *globalFogEnabled);
			}

			int FixFogPatch::InitializeFogSettings(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				*globalFogEnabled = Utils::GetProfileValue("CreationKitPrefs.ini", "Display", "bFogEnabled", true);
				return EditorAPI::EditorUI::HKSendMessageA(hWnd, uMsg, wParam, lParam);
			}
		}
	}
}