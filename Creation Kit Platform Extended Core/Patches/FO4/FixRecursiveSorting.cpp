// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixRecursiveSorting.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixRecursiveSortingPatch::FixRecursiveSortingPatch() : Module(GlobalEnginePtr)
			{}

			bool FixRecursiveSortingPatch::HasOption() const
			{
				return false;
			}

			bool FixRecursiveSortingPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixRecursiveSortingPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixRecursiveSortingPatch::GetName() const
			{
				return "Fixed recursive sorting function";
			}

			bool FixRecursiveSortingPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixRecursiveSortingPatch::GetDependencies() const
			{
				return {};
			}

			bool FixRecursiveSortingPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixRecursiveSortingPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{	
					// Fix for crash (recursive sorting function stack overflow) when saving certain ESP files (i.e SimSettlements.esp)
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub<class TESForm_CK*>);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xC3 });

					return true;
				}

				return false;
			}

			bool FixRecursiveSortingPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}