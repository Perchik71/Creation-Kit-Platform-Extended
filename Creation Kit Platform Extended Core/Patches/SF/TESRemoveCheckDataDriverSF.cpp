// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "TESRemoveCheckDataDriverSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			TESRemoveCheckDriverSFPatch::TESRemoveCheckDriverSFPatch() : Module(GlobalEnginePtr)
			{}

			bool TESRemoveCheckDriverSFPatch::HasOption() const
			{
				return false;
			}

			bool TESRemoveCheckDriverSFPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* TESRemoveCheckDriverSFPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* TESRemoveCheckDriverSFPatch::GetName() const
			{
				return "TES Remove Check Driver";
			}

			bool TESRemoveCheckDriverSFPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> TESRemoveCheckDriverSFPatch::GetDependencies() const
			{
				return {};
			}

			bool TESRemoveCheckDriverSFPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_78_0;
			}

			bool TESRemoveCheckDriverSFPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->Patch(_RELDATA_RAV(0), { 0xC3, 0x90, 0x90, 0x90, 0x90 });

					return true;
				}

				return false;
			}

			bool TESRemoveCheckDriverSFPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}