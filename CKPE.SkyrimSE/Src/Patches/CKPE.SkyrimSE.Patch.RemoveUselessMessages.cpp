// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool RemoveUselessMessages::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool RemoveUselessMessages::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RemoveUselessMessages::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Disable "The maximum length allowed is 33 characters" log spam
				Relocation(ID(550126), 0x1C0).WriteFill(NOP, 5);
				// Disable "Found geometry '%s' under fade node '%s' with no shader property." log spam
				Relocation(ID(554913), Offset{ 0x93C, 0x98D }).WriteFill(NOP, 5);
				// Disable "Unable to initialize perforce, check your logs for more information."
				Relocation(ID(47467), 0x20E).WriteFill(NOP, 6);
				// Disable "Data array for partition does not match partition count" log spam
				Relocation(ID(555460), 0x5E).Write(JMP);

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				// Disable useless "Processing Topic X..." status bar updates
				text.WriteNop(Relocation(ID(198454), 0xB9).Address(), 5);
				text.WriteNop(Relocation(ID(174902), 0xAE).Address(), 5);
				text.WriteNop(Relocation(ID(2803), 0xC2).Address(), 5);
				text.Write(ID(219292).Address(), { 0xC3 });									// Disable "MEM_CATEGORY_X" log spam
				text.WriteNop(Relocation(ID(147614), Offset{ 0x9EC, 0xA74 }).Address(), 5);	// Disable "utility failed id" log spam
				text.WriteNop(Relocation(ID(355109), Offset{ 0x163, 0x17C }).Address(), 5);	// Disable "Should have been converted offline" log spam

				// Disable "Compiling Vertex Shader: %s %s - TechnicID: %u" log spam
				auto rav = Relocation(ID(141814), Offset{ 0x143, 0x13E }).Address();
				auto size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
				rav = Relocation(ID{ 612233, 942915 }, Offset{ 0x293, 0x160 }).Address();
				size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
				// Disable "Compiling Pixel Shader: %s %s - TechnicID: %u" log spam
				rav = Relocation(ID{ 189952, 1106400 }, Offset{ 0x132, 0x141 }).Address();
				size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });
				rav = Relocation(ID{ 771820, 924228 }, Offset{ 0x293, 0x160 }).Address();
				size = (std::uint8_t)(*(std::uintptr_t*)(rav + 2));
				text.Write(rav, { 0xE9, (std::uint8_t)(size + 1), 0x00, 0x00, 0x00, 0x90 });

				return true;
			}
		}
	}
}