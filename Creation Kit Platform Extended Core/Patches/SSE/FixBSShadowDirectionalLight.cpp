// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixBSShadowDirectionalLight.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixBSShadowDirectionalLightPatch::FixBSShadowDirectionalLightPatch() : Module(GlobalEnginePtr)
			{}

			bool FixBSShadowDirectionalLightPatch::HasOption() const
			{
				return false;
			}

			bool FixBSShadowDirectionalLightPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixBSShadowDirectionalLightPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixBSShadowDirectionalLightPatch::GetName() const
			{
				return "BSShadowDirectionalLight Fix";
			}

			bool FixBSShadowDirectionalLightPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixBSShadowDirectionalLightPatch::GetDependencies() const
			{
				return {};
			}

			bool FixBSShadowDirectionalLightPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBSShadowDirectionalLightPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Memory bug fix during BSShadowDirectionalLight calculations (see game patch for more information)
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x4D, 0x89, 0xE1, 0x90, 0x90, 0x90, 0x90 });

					return true;
				}

				return false;
			}

			bool FixBSShadowDirectionalLightPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}