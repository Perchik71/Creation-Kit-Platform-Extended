// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixSky.h"
#include "Editor API/FO4/TESObjectCELL.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixSkyPatch::FixSkyPatch() : Module(GlobalEnginePtr)
			{}

			bool FixSkyPatch::HasOption() const
			{
				return false;
			}

			bool FixSkyPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSkyPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSkyPatch::GetName() const
			{
				return "Fix Sky";
			}

			bool FixSkyPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixSkyPatch::GetDependencies() const
			{
				return {};
			}

			bool FixSkyPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixSkyPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					ScopeRelocator text;

					// Fix for crash (nullptr no test) when close CK with Sky enable 

					if (verPatch == 1)
					{
						lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB, 0x4D, 0x90 });
						lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0x48, 0x85, 0xC9, 0x74, 0xB5,
							0x48, 0x8B, 0x01, 0xEB, 0xAA });
					}
					else
					{
						lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB, 0x54, 0x90 });
						lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0x48, 0x85, 0xC9, 0x74, 0xAE,
							0x48, 0x8B, 0x01, 0xEB, 0xA3 });
					}

					return true;
				}

				return false;
			}

			bool FixSkyPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}