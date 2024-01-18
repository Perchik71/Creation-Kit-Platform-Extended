// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixTabDeleteCrash.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixTabDeleteCrashPatch::FixTabDeleteCrashPatch() : Module(GlobalEnginePtr)
			{}

			bool FixTabDeleteCrashPatch::HasOption() const
			{
				return false;
			}

			bool FixTabDeleteCrashPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixTabDeleteCrashPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixTabDeleteCrashPatch::GetName() const
			{
				return "Fix uninitialized TCITEMA structure variables";
			}

			bool FixTabDeleteCrashPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixTabDeleteCrashPatch::GetDependencies() const
			{
				return {};
			}

			bool FixTabDeleteCrashPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixTabDeleteCrashPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fix for crash when tab control buttons are deleted. Uninitialized TCITEMA structure variables.
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixTabDeleteCrashPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixTabDeleteCrashPatch::sub(HWND TabControlHandle, uint32_t TabIndex)
			{
				TCITEMA itemInfo = {};

				if (TabCtrl_GetItem(TabControlHandle, TabIndex, &itemInfo))
					TabCtrl_DeleteItem(TabControlHandle, TabIndex);
			}
		}
	}
}