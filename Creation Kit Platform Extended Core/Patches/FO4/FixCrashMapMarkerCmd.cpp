// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashMapMarkerCmd.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_FixCrashMapMarkerCmd_sub = 0;

			FixCrashMapMarkerCmdPatch::FixCrashMapMarkerCmdPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashMapMarkerCmdPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashMapMarkerCmdPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashMapMarkerCmdPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashMapMarkerCmdPatch::GetName() const
			{
				return "Fixed MapMaker command line option";
			}

			bool FixCrashMapMarkerCmdPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashMapMarkerCmdPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashMapMarkerCmdPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixCrashMapMarkerCmdPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fix for crash when using the -MapMaker command line option. Nullptr camera passed to 
					// BSGraphics::State::SetCameraData.
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_FixCrashMapMarkerCmd_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool FixCrashMapMarkerCmdPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixCrashMapMarkerCmdPatch::sub(int64_t a1, int64_t a2, int64_t a3)
			{
				if (a2)
					fastCall<VOID>(pointer_FixCrashMapMarkerCmd_sub, a1, a2, a3);
			}
		}
	}
}