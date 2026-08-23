// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
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
				if (!db)
				{
					using namespace Common;

					auto interface = CKPE::Common::Interface::GetSingleton();

					// Cutting a lot is faster this way
					auto stext = interface->GetApplication()->GetSegment(Segment::text);
					ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

					text.WriteNop(Relocation(ID{ 1597242 }, Offset{ 0x3A }).Address(), 5);// Disable "Out of Pixel Shaders (running total: X)" log spam
					text.WriteNop(Relocation(ID{ 1460016 }, Offset{ 0x56 }).Address(), 5);// Disable "Out of UCode space" log spam
					text.WriteNop(Relocation(ID{ 1812506 }, Offset{ 0x254 }).Address(), 5);// Disable "BSMeshCombiner" log spam
					text.WriteNop(Relocation(ID{ 1812506 }, Offset{ 0x2B8 }).Address(), 5);// Disable "BSMeshCombiner" log spam
					// Disable useless "Processing Topic X..." status bar updates
					text.WriteNop(Relocation(ID{ 1517908 }, Offset{ 0xF6 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1943931 }, Offset{ 0xF5 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 530154 }, Offset{ 0x92 }).Address(), 5);
					// Skipping the string check is more than 33 characters, this is irrelevant for new games
					text.Write(Relocation(ID{ 181955 }, Offset{ 0xD3 }).Address(), { 0xEB });
					// Missing root material: %s.
					text.WriteNop(Relocation(ID{ 1553189 }, Offset{ 0x205 }).Address(), 0x5);
					// EXTRA SPACE
					text.WriteNop(Relocation(ID{ 180692 }, Offset{ 0x41 }).Address(), 0x35);
					// NavMesh ID %08X in cell %s has invalid cover data. It will be removed.
					text.WriteNop(Relocation(ID{ 376750 }, Offset{ 0x8C5 }).Address(), 0x5);
					// Animation messages
					text.WriteNop(Relocation(ID{ 1591969 }, Offset{ 0x158 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1591969 }, Offset{ 0x5C4 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1591969 }, Offset{ 0x62F }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1591969 }, Offset{ 0x5C4 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1938816 }, Offset{ 0x205 }).Address(), 5);
					// TEXTURE Unable to load file
					text.Write(Relocation(ID{ 1594183 }, Offset{ 0x1AA }).Address(), { 0xE9, 0xB8, 0x00, 0x00, 0x00, 0x90 });
					// MODEL Could not find model
					text.WriteNop(Relocation(ID{ 1500759 }, Offset{ 0x189 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1789956 }, Offset{ 0x1CA }).Address(), 5);
					// SHADER Could not find material
					text.WriteNop(Relocation(ID{ 471763 }, Offset{ 0x452 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 170773 }, Offset{ 0xEB1 }).Address(), 5);
					// MODEL Found geometry
					text.WriteNop(Relocation(ID{ 1695229 }, Offset{ 0x972 }).Address(), 5);
					// Always off option bAllowFileWrite:MESSAGES
					text.Write(Relocation(ID{ 530889 }).Address(), { 0x31, 0xC0, 0xC3, 0x90 });

					// SHADER Could not find material
					// Could not determine topic info from ID from %s
					// Scene Form ID: %i, HoldPackageNextScene set to on save: %s
					// The width of face customization texture %s (%u) does not match that of other textures (%u) on race '%s' (%08X)
					// The height of face customization texture %s (%u) does not match that of other textures (%u) on race '%s' (%08X)
					// DEFAULT: Setting key '%s' already used in list.\nSetting keys must be unique.\n
					// DEFAULT: Setting key '%s' already used in map.\nSetting keys must be unique.\n
					// The format of face customization texture %s (%d) does not match that of other textures (%u) on race '%s' (%08X)
					// etc
					text.WriteNop(Relocation(ID{ 1634140 }, Offset{ 0xC0 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1634142 }, Offset{ 0xC0 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1776000 }, Offset{ 0x83C }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1908305 }, Offset{ 0x432 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 358290 }, Offset{ 0x75 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 470056 }, Offset{ 0xCE }).Address(), 5);
					text.WriteNop(Relocation(ID{ 470056 }, Offset{ 0x12B }).Address(), 5);
					text.WriteNop(Relocation(ID{ 375972 }, Offset{ 0xD2 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1846985 }, Offset{ 0x15A }).Address(), 5);
					text.WriteNop(Relocation(ID{ 470056 }, Offset{ 0x19D }).Address(), 5);
					text.WriteNop(Relocation(ID{ 470056 }, Offset{ 0x295 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 357841 }, Offset{ 0xB7 }).Address(), 5);
					text.WriteNop(Relocation(ID{ 1943828 }, Offset{ 0x35 }).Address(), 5);

					return true;
				}

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