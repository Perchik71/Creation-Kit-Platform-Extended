// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "VersionControlMergeWorkaround.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		VersionControlMergeWorkaroundPatch::VersionControlMergeWorkaroundPatch() : Module(GlobalEnginePtr)
		{}

		bool VersionControlMergeWorkaroundPatch::HasOption() const
		{
			return true;
		}

		bool VersionControlMergeWorkaroundPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* VersionControlMergeWorkaroundPatch::GetOptionName() const
		{
			return "CreationKit:bVersionControlMergeWorkaround";
		}

		const char* VersionControlMergeWorkaroundPatch::GetName() const
		{
			return "Version Control Merge Workaround";
		}

		bool VersionControlMergeWorkaroundPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> VersionControlMergeWorkaroundPatch::GetDependencies() const
		{
			return {};
		}

		bool VersionControlMergeWorkaroundPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool VersionControlMergeWorkaroundPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			//
			// Workaround for version control not allowing merges when a plugin index is above 02.
			// Bethesda's VC bitmap files determine heck-in status along with user IDs for each specific form in the game. 
			// They're also hardcoded for 2 masters only. Using this hack for anything EXCEPT merging will break the bitmaps.
			//

			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB });

				return true;
			}
			else if (verPatch == 2)
			{
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(4), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(5), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(8), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(9), { 0xEB });

				return true;
			}

			return false;
		}

		bool VersionControlMergeWorkaroundPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}