// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashTabControl.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixCrashTabControlPatch::FixCrashTabControlPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashTabControlPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashTabControlPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashTabControlPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashTabControlPatch::GetName() const
			{
				return "Fix Crash TabControl";
			}

			bool FixCrashTabControlPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashTabControlPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashTabControlPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashTabControlPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when tab control buttons are deleted.
					// Uninitialized TCITEMA structure variables.
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixCrashTabControlPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixCrashTabControlPatch::sub(HWND TabControlHandle, uint32_t TabIndex)
			{
				TCITEMA itemInfo = {};

				if (TabCtrl_GetItem(TabControlHandle, TabIndex, &itemInfo))
					TabCtrl_DeleteItem(TabControlHandle, TabIndex);
			}
		}
	}
}