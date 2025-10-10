// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.RemoveUselessMessages.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			RemoveUselessMessages::RemoveUselessMessages() : Common::Patch()
			{
				SetName("Remove Useless Messages");
			}

			bool RemoveUselessMessages::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RemoveUselessMessages::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RemoveUselessMessages::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RemoveUselessMessages::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RemoveUselessMessages::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool RemoveUselessMessages::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				text.WriteNop(__CKPE_OFFSET(0), 5);// Disable "Out of Pixel Shaders (running total: X)" log spam
				text.WriteNop(__CKPE_OFFSET(1), 5);// Disable "Out of UCode space" log spam
				text.WriteNop(__CKPE_OFFSET(2), 5);// Disable "BSMeshCombiner" log spam
				text.WriteNop(__CKPE_OFFSET(3), 5);// Disable "BSMeshCombiner" log spam
				// Disable useless "Processing Topic X..." status bar updates
				text.WriteNop(__CKPE_OFFSET(4), 5);
				text.WriteNop(__CKPE_OFFSET(5), 5);
				text.WriteNop(__CKPE_OFFSET(6), 5);
				// Skipping the string check is more than 33 characters, this is irrelevant for new games
				text.Write(__CKPE_OFFSET(7), { 0xEB });
				// Failed to create instance (0x%08X) for model (0x%08X) while generating visibility.
				//text.WriteNop(__CKPE_OFFSET(8), 0x5);
				// failed to create visibility model from scene model (0x%08X)
				//text.WriteNop(__CKPE_OFFSET(9), 0x5);
				//text.WriteNop(__CKPE_OFFSET(10), 0x5);
				// Missing root material: %s.
				text.WriteNop(__CKPE_OFFSET(11), 0x5);
				// Bound for object '%s' (%08X) exceeds 32000 unit limits (%.0f,%.0f,%.0f),(%.0f,%.0f,%.0f).
				//text.WriteNop(__CKPE_OFFSET(12), 0x5);
				// EXTRA SPACE
				text.WriteNop(__CKPE_OFFSET(13), 0x35);
				// NavMesh ID %08X in cell %s has invalid cover data. It will be removed.
				text.WriteNop(__CKPE_OFFSET(14), 0x5);
				// Animation messages
				text.WriteNop(__CKPE_OFFSET(15), 5);
				text.WriteNop(__CKPE_OFFSET(16), 5);
				text.WriteNop(__CKPE_OFFSET(17), 5);
				text.WriteNop(__CKPE_OFFSET(18), 5);
				text.WriteNop(__CKPE_OFFSET(19), 5);
				// TEXTURE Unable to load file
				text.Write(__CKPE_OFFSET(20), { 0xE9, 0xB8, 0x00, 0x00, 0x00, 0x90 });
				// MODEL Could not find model
				text.WriteNop(__CKPE_OFFSET(21), 5);
				text.WriteNop(__CKPE_OFFSET(22), 5);
				// SHADER Could not find material
				text.WriteNop(__CKPE_OFFSET(23), 5);
				text.WriteNop(__CKPE_OFFSET(24), 5);
				// MODEL Found geometry
				text.WriteNop(__CKPE_OFFSET(25), 5);
				// Always off option bAllowFileWrite:MESSAGES
				text.Write(__CKPE_OFFSET(26), { 0x31, 0xC0, 0xC3, 0x90 });

				// SHADER Could not find material
				// Could not determine topic info from ID from %s
				// Scene Form ID: %i, HoldPackageNextScene set to on save: %s
				// The width of face customization texture %s (%u) does not match that of other textures (%u) on race '%s' (%08X)
				// The height of face customization texture %s (%u) does not match that of other textures (%u) on race '%s' (%08X)
				// DEFAULT: Setting key '%s' already used in list.\nSetting keys must be unique.\n
				// DEFAULT: Setting key '%s' already used in map.\nSetting keys must be unique.\n
				// The format of face customization texture %s (%d) does not match that of other textures (%u) on race '%s' (%08X)
				// etc
				for (std::uint32_t i = 27; i < db->GetCount(); i++)
					text.WriteNop(__CKPE_OFFSET(i), 5);

				return true;
			}
		}
	}
}