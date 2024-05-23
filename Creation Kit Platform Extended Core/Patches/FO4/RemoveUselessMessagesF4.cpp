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
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					ScopeRelocator text;

					lpRelocator->PatchNop(_RELDATA_RAV(0), 5);// Disable "Out of Pixel Shaders (running total: X)" log spam
					lpRelocator->PatchNop(_RELDATA_RAV(1), 5);// Disable "Out of UCode space" log spam
					lpRelocator->PatchNop(_RELDATA_RAV(2), 5);// Disable "BSMeshCombiner" log spam
					lpRelocator->PatchNop(_RELDATA_RAV(3), 5);// Disable "BSMeshCombiner" log spam
					// Disable useless "Processing Topic X..." status bar updates
					lpRelocator->PatchNop(_RELDATA_RAV(4), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(5), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(6), 5);
					// Skipping the string check is more than 33 characters, this is irrelevant for new games
					lpRelocator->Patch(_RELDATA_RAV(7), { 0xEB });
					// Failed to create instance (0x%08X) for model (0x%08X) while generating visibility.
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(8), 0x5);
					// failed to create visibility model from scene model (0x%08X)
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(9), 0x5);
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(10), 0x5);
					// Missing root material: %s.
					lpRelocator->PatchNop(_RELDATA_RAV(11), 0x5);
					// Bound for object '%s' (%08X) exceeds 32000 unit limits (%.0f,%.0f,%.0f),(%.0f,%.0f,%.0f).
					//lpRelocator->PatchNop(lpRelocationDatabaseItem->At(12), 0x5);
					// EXTRA SPACE
					lpRelocator->PatchNop(_RELDATA_RAV(13), 0x35);
					// NavMesh ID %08X in cell %s has invalid cover data. It will be removed.
					lpRelocator->PatchNop(_RELDATA_RAV(14), 0x5);
					// Animation messages
					lpRelocator->PatchNop(_RELDATA_RAV(15), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(16), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(17), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(18), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(19), 5);
					// TEXTURE Unable to load file
					lpRelocator->Patch(_RELDATA_RAV(20), { 0xE9, 0xB8, 0x00, 0x00, 0x00, 0x90 });
					// MODEL Could not find model
					lpRelocator->PatchNop(_RELDATA_RAV(21), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(22), 5);
					// SHADER Could not find material
					lpRelocator->PatchNop(_RELDATA_RAV(23), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(24), 5);
					// MODEL Found geometry
					lpRelocator->PatchNop(_RELDATA_RAV(25), 5);
					// Always off option bAllowFileWrite:MESSAGES
					lpRelocator->Patch(_RELDATA_RAV(26), { 0x31, 0xC0, 0xC3, 0x90 });

					// SHADER Could not find material
					// Could not determine topic info from ID from %s
					// Scene Form ID: %i, HoldPackageNextScene set to on save: %s
					// The width of face customization texture %s (%u) does not match that of other textures (%u) on race '%s' (%08X)
					// The height of face customization texture %s (%u) does not match that of other textures (%u) on race '%s' (%08X)
					// DEFAULT: Setting key '%s' already used in list.\nSetting keys must be unique.\n
					// DEFAULT: Setting key '%s' already used in map.\nSetting keys must be unique.\n
					// The format of face customization texture %s (%d) does not match that of other textures (%u) on race '%s' (%08X)
					// etc
					for (uint32_t i = 27; i < lpRelocationDatabaseItem->Count(); i++)
						lpRelocator->PatchNop(_RELDATA_RAV(i), 5);

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