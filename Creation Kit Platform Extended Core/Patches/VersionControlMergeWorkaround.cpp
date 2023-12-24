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
			return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
		}

		bool VersionControlMergeWorkaroundPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB });

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