// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixIconsScriptProp.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixIconsScriptPropPatch::FixIconsScriptPropPatch() : Module(GlobalEnginePtr)
			{}

			bool FixIconsScriptPropPatch::HasOption() const
			{
				return false;
			}

			bool FixIconsScriptPropPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixIconsScriptPropPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixIconsScriptPropPatch::GetName() const
			{
				return "Fix Icons Script Prop";
			}

			bool FixIconsScriptPropPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixIconsScriptPropPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for icons not appearing in the script properties dialog (list view) (LVIF_TEXT -> LVIF_IMAGE)
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x02 });

					return true;
				}

				return false;
			}

			bool FixIconsScriptPropPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}