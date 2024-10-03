// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RTDynamicCastCrash.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			uintptr_t pointer_RTDynamicCastCrashPatch_sub1 = 0;

			RTDynamicCastCrashPatch::RTDynamicCastCrashPatch() : Module(GlobalEnginePtr)
			{}

			bool RTDynamicCastCrashPatch::HasOption() const
			{
				return false;
			}

			bool RTDynamicCastCrashPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RTDynamicCastCrashPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RTDynamicCastCrashPatch::GetName() const
			{
				return "RTDynamicCast Crash";
			}

			bool RTDynamicCastCrashPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RTDynamicCastCrashPatch::GetDependencies() const
			{
				return {};
			}

			bool RTDynamicCastCrashPatch::sub(int64_t a1, int64_t a2, int64_t a3, int64_t a4)
			{
				// Fixing an error inside dynamic_cast.

				__try
				{
					return fastCall<bool>(pointer_RTDynamicCastCrashPatch_sub1, a1, a2, a3, a4);
				}
				__except (1)
				{
					return false;
				}
			}

			bool RTDynamicCastCrashPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) && 
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool RTDynamicCastCrashPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					pointer_RTDynamicCastCrashPatch_sub1 = 
						voltek::detours_function_class_jump(_RELDATA_ADDR(0), (uintptr_t)&sub);
	
					return true;
				}

				return false;
			}

			bool RTDynamicCastCrashPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}