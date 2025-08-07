// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.RemoveUselessMessages.h>

namespace CKPE
{
	namespace SkyrimSE
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RemoveUselessMessages::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				// Disable useless "Processing Topic X..." status bar updates
				text.WriteNop(__CKPE_OFFSET(0), 5);
				text.WriteNop(__CKPE_OFFSET(1), 5);
				text.WriteNop(__CKPE_OFFSET(2), 5);
				text.Write(__CKPE_OFFSET(3), { 0xC3 });	// Disable "MEM_CATEGORY_X" log spam
				text.WriteNop(__CKPE_OFFSET(4), 5);		// Disable "utility failed id" log spam
				text.WriteNop(__CKPE_OFFSET(5), 5);		// Disable "Should have been converted offline" log spam

				// Disable "The maximum length allowed is 33 characters" log spam
				text.WriteNop(__CKPE_OFFSET(6), 5);
				// Disable "Data array for partition does not match partition count" log spam
				text.Write(__CKPE_OFFSET(7), { 0xEB });

				// Disable "Compiling Vertex Shader: %s %s - TechnicID: %u" log spam
				std::uintptr_t rav = __CKPE_OFFSET(8);
				std::uint8_t size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
				rav = __CKPE_OFFSET(9);
				size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
				// Disable "Compiling Pixel Shader: %s %s - TechnicID: %u" log spam
				rav = __CKPE_OFFSET(10);
				size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
				rav = __CKPE_OFFSET(11);
				size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });

				// Disable "Unable to initialize perforce, check your logs for more information."
				text.WriteNop(__CKPE_OFFSET(12), 6);

				for (std::uint32_t i = 13; i < db->GetCount(); i++)
					text.WriteNop(__CKPE_OFFSET(i), 5);

				return true;
			}
		}
	}
}