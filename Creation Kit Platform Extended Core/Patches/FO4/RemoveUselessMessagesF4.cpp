// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RemoveUselessMessagesF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool RemoveUselessMessagesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					ScopeRelocator text;

					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 5);// Disable "Out of Pixel Shaders (running total: X)" log spam
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 5);// Disable "Out of UCode space" log spam
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 5);// Disable "BSMeshCombiner" log spam
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 5);// Disable "BSMeshCombiner" log spam
					// Disable useless "Processing Topic X..." status bar updates
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(4), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(5), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(6), 5);
					// Skipping the string check is more than 33 characters, this is irrelevant for new games
					lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xEB });
					// Failed to create instance (0x%08X) for model (0x%08X) while generating visibility.
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(8), 0x5);
					// failed to create visibility model from scene model (0x%08X)
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(9), 0x5);
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(10), 0x5);
					// Missing root material: %s.
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(11), 0x5);
					// Bound for object '%s' (%08X) exceeds 32000 unit limits (%.0f,%.0f,%.0f),(%.0f,%.0f,%.0f).
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(12), 0x5);
					// EXTRA SPACE
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(13), 0x35);
					// NavMesh ID %08X in cell %s has invalid cover data. It will be removed.
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(14), 0x5);
					// Animation messages
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(15), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(16), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(17), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(18), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(19), 5);
					// TEXTURE Unable to load file
					lpRelocator->Patch(lpRelocationDatabaseItem->At(20), { 0xE9, 0xB8, 0x00, 0x00, 0x00, 0x90 });
					// MODEL Could not find model
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(21), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(22), 5);
					// SHADER Could not find material
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(23), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(24), 5);
					// MODEL Found geometry
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(25), 5);

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