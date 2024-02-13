// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "IgnoreGroundHeightTest.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			IgnoreGroundHeightTestPatch::IgnoreGroundHeightTestPatch() : Module(GlobalEnginePtr)
			{}

			bool IgnoreGroundHeightTestPatch::HasOption() const
			{
				return true;
			}

			bool IgnoreGroundHeightTestPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* IgnoreGroundHeightTestPatch::GetOptionName() const
			{
				return "CreationKit:bIgnoreGroundHeightTest";
			}

			const char* IgnoreGroundHeightTestPatch::GetName() const
			{
				return "Ignore Ground Height Test";
			}

			bool IgnoreGroundHeightTestPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> IgnoreGroundHeightTestPatch::GetDependencies() const
			{
				return {};
			}

			bool IgnoreGroundHeightTestPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool IgnoreGroundHeightTestPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// At this point, the obtained height is compared with an incorrect value. 
					// Skip all this if the user requested it in the settings.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xE9, 0xB1, 0x02, 0x00, 0x00, 0x90 });
	
					return true;
				}

				return false;
			}

			bool IgnoreGroundHeightTestPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}