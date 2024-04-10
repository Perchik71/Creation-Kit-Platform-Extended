// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "SortCrashCombined.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_SortCrashCombinedPatch_sub = 0;

			SortCrashCombinedPatch::SortCrashCombinedPatch() : Module(GlobalEnginePtr)
			{}

			bool SortCrashCombinedPatch::HasOption() const
			{
				return false;
			}

			bool SortCrashCombinedPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* SortCrashCombinedPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* SortCrashCombinedPatch::GetName() const
			{
				return "Sort Crash Combined";
			}

			bool SortCrashCombinedPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> SortCrashCombinedPatch::GetDependencies() const
			{
				return {};
			}

			bool SortCrashCombinedPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool SortCrashCombinedPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&pointer_SortCrashCombinedPatch_sub =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)),
							&sub);

					return true;
				}

				return false;
			}

			bool SortCrashCombinedPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			int SortCrashCombinedPatch::sub(const void* lsb, const void* rsb, size_t size)
			{
				if (!lsb)
					return -1;
				else if (!rsb)
					return 1;
				else
					return fastCall<int>(pointer_SortCrashCombinedPatch_sub, lsb, rsb, size);
			}
		}
	}
}