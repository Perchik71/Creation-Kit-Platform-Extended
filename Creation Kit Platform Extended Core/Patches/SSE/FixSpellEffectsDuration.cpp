// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixSpellEffectsDuration.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixSpellEffectsDurationPatch::FixSpellEffectsDurationPatch() : Module(GlobalEnginePtr)
			{}

			bool FixSpellEffectsDurationPatch::HasOption() const
			{
				return false;
			}

			bool FixSpellEffectsDurationPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSpellEffectsDurationPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSpellEffectsDurationPatch::GetName() const
			{
				return "Fix a spell effect duration";
			}

			bool FixSpellEffectsDurationPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixSpellEffectsDurationPatch::GetDependencies() const
			{
				return {};
			}

			bool FixSpellEffectsDurationPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSpellEffectsDurationPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when editing a spell effect with a large (>= 1'000'000'000) duration. 
					// WARNING: Stack padding allows the buffer to be up to 12 bytes, 10 are originally reserved.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xBA, 0x0C, 0x00, 0x00, 0x00 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xBA, 0x0C, 0x00, 0x00, 0x00 });

					return true;
				}

				return false;
			}

			bool FixSpellEffectsDurationPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}