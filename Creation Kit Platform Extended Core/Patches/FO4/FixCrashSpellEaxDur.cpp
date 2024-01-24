// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashSpellEaxDur.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixCrashSpellEaxDurPatch::FixCrashSpellEaxDurPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashSpellEaxDurPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashSpellEaxDurPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashSpellEaxDurPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashSpellEaxDurPatch::GetName() const
			{
				return "Fix crash spell effect duration";
			}

			bool FixCrashSpellEaxDurPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashSpellEaxDurPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashSpellEaxDurPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixCrashSpellEaxDurPatch::Activate(const Relocator* lpRelocator,
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

			bool FixCrashSpellEaxDurPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}