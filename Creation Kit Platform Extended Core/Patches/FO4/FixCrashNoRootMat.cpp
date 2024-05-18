// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashNoRootMat.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixCrashNoRootMatPatch::FixCrashNoRootMatPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashNoRootMatPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashNoRootMatPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashNoRootMatPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashNoRootMatPatch::GetName() const
			{
				return "Fixed no root parent to materials";
			}

			bool FixCrashNoRootMatPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashNoRootMatPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashNoRootMatPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixCrashNoRootMatPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					ScopeRelocator text;
					
					// Fixed crash when by load plugin in which there is no root parent to materials
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x0F, 0xC6, 0x1D, 0x1C, 0xC0, 0x30, 0x01, 0xEE,
						0x0F, 0xC6, 0x05, 0x14, 0xC0, 0x30, 0x01, 0x44, 0x31, 0xC0, 0xEB, 0x4B });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB, 0x6F });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0xEB });

					return true;
				}
				else if (verPatch == 2)
				{
					ScopeRelocator text;
					// Fixed crash when by load plugin in which there is no root parent to materials
					auto rva = lpRelocationDatabaseItem->At(0);
					auto rva_data = lpRelocationDatabaseItem->At(3);
					lpRelocator->Patch(rva, { 0x0F, 0xC6, 0x1D });
					auto RelOff = (uint32_t)(rva_data - (rva + 8));
					lpRelocator->Patch((uintptr_t)rva + 3, (uint8_t*)&RelOff, 4);
					lpRelocator->Patch((uintptr_t)rva + 7, { 0xEE });	
					rva += 8;
					lpRelocator->Patch(rva, { 0x0F, 0xC6, 0x05 });
					RelOff = (uint32_t)(rva_data - (rva + 8));
					lpRelocator->Patch((uintptr_t)rva + 3, (uint8_t*)&RelOff, 4);
					lpRelocator->Patch((uintptr_t)rva + 7, { 0x44, 0x31, 0xC0, 0xEB, 0x4D });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB, 0x74 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0xEB });

					return true;
				}

				return false;
			}

			bool FixCrashNoRootMatPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}