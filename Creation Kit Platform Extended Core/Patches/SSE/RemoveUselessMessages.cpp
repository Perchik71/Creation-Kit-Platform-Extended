// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RemoveUselessMessages.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			RemoveUselessMessagesPatch::RemoveUselessMessagesPatch() : Module(GlobalEnginePtr)
			{}

			bool RemoveUselessMessagesPatch::HasOption() const
			{
				return false;
			}

			bool RemoveUselessMessagesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RemoveUselessMessagesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RemoveUselessMessagesPatch::GetName() const
			{
				return "Remove Useless Messages";
			}

			bool RemoveUselessMessagesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RemoveUselessMessagesPatch::GetDependencies() const
			{
				return {};
			}

			bool RemoveUselessMessagesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool RemoveUselessMessagesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Cutting a lot is faster this way
					ScopeRelocator text; 

					// Disable useless "Processing Topic X..." status bar updates
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 5);

					lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0xC3 });	// Disable "MEM_CATEGORY_X" log spam
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(4), 5);		// Disable "utility failed id" log spam
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(5), 5);		// Disable "Should have been converted offline" log spam
					
					// Disable "The maximum length allowed is 33 characters" log spam
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(6), 5);		
					// Disable "Data array for partition does not match partition count" log spam
					lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xEB });
					
					// Disable "Compiling Vertex Shader: %s %s - TechnicID: %u" log spam
					uintptr_t rav = lpRelocationDatabaseItem->At(8);
					uint8_t size = (uint8_t)(*(uintptr_t*)(lpRelocator->Rav2Off(rav + 2)));
					lpRelocator->Patch(rav, { 0xE9, (uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
					rav = lpRelocationDatabaseItem->At(9);
					size = (uint8_t)(*(uintptr_t*)(lpRelocator->Rav2Off(rav + 2)));
					lpRelocator->Patch(rav, { 0xE9, (uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
					// Disable "Compiling Pixel Shader: %s %s - TechnicID: %u" log spam
					rav = lpRelocationDatabaseItem->At(10);
					size = (uint8_t)(*(uintptr_t*)(lpRelocator->Rav2Off(rav + 2)));
					lpRelocator->Patch(rav, { 0xE9, (uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
					rav = lpRelocationDatabaseItem->At(11);
					size = (uint8_t)(*(uintptr_t*)(lpRelocator->Rav2Off(rav + 2)));
					lpRelocator->Patch(rav, { 0xE9, (uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });

					// Disable "Unable to initialize perforce, check your logs for more information."
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(12), 6);

					return true;
				}

				return false;
			}

			bool RemoveUselessMessagesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}