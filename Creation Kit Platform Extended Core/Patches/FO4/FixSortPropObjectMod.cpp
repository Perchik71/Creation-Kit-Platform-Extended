// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixSortPropObjectMod.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixSortPropObjectModPatch::FixSortPropObjectModPatch() : Module(GlobalEnginePtr)
			{}

			bool FixSortPropObjectModPatch::HasOption() const
			{
				return false;
			}

			bool FixSortPropObjectModPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSortPropObjectModPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSortPropObjectModPatch::GetName() const
			{
				return "Fix Sort Prop Object Mod";
			}

			bool FixSortPropObjectModPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixSortPropObjectModPatch::GetDependencies() const
			{
				return {};
			}

			bool FixSortPropObjectModPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool FixSortPropObjectModPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Removing stuff that messes up the index selected item
					lpRelocator->PatchNop(_RELDATA_RAV(0), 0xF);

					return true;
				}

				return false;
			}

			bool FixSortPropObjectModPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}