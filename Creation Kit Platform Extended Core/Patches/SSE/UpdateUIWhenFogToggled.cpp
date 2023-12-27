// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "UpdateUIWhenFogToggled.h"
#include "Editor API/EditorUI.h"
#include "Patches/Windows/SSE/MainWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_UpdateUIWhenFogToggled_sub = 0;

			UpdateUIWhenFogToggledPatch::UpdateUIWhenFogToggledPatch() : Module(GlobalEnginePtr)
			{}

			bool UpdateUIWhenFogToggledPatch::HasOption() const
			{
				return false;
			}

			bool UpdateUIWhenFogToggledPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* UpdateUIWhenFogToggledPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* UpdateUIWhenFogToggledPatch::GetName() const
			{
				return "Update UI When Fog Toggled";
			}

			bool UpdateUIWhenFogToggledPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> UpdateUIWhenFogToggledPatch::GetDependencies() const
			{
				return { "Main Window" };
			}

			bool UpdateUIWhenFogToggledPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool UpdateUIWhenFogToggledPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Update the UI options when fog is toggled
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_UpdateUIWhenFogToggled_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool UpdateUIWhenFogToggledPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void UpdateUIWhenFogToggledPatch::sub(__int64 a1, bool Enable)
			{
				// Modify the global setting itself then update UI to match
				((void(__fastcall*)(__int64, bool))pointer_UpdateUIWhenFogToggled_sub)(a1, Enable);

				CheckMenuItem(GetMenu(GlobalMainWindowPtr->Handle), EditorAPI::EditorUI::UI_EDITOR_TOGGLEFOG,
					Enable ? MF_CHECKED : MF_UNCHECKED);
			}
		}
	}
}