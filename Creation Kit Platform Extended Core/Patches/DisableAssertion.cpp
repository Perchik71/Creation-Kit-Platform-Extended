// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "DisableAssertion.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		DisableAssertionPatch::DisableAssertionPatch() : Module(GlobalEnginePtr)
		{}

		bool DisableAssertionPatch::HasOption() const
		{
			return true;
		}

		bool DisableAssertionPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* DisableAssertionPatch::GetOptionName() const
		{
			return "CreationKit:bDisableAssertions";
		}

		const char* DisableAssertionPatch::GetName() const
		{
			return "Disable Assertion";
		}

		bool DisableAssertionPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
		}

		bool DisableAssertionPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();
			if ((verPatch == 1) || (verPatch == 2))
			{
				//
				// Remove assertion message boxes
				//
				lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 5);
				if (verPatch == 2) lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 5);

				return true;
			}

			return false;
		}

		bool DisableAssertionPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}