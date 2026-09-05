// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
				using namespace Common;

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				text.WriteNop(Relocation(ID{ 795189, 1627205 }, Offset{ 0x3C, 0x3A }).Address(), 5);	// Disable "Out of Pixel Shaders (running total: X)" log spam
				text.WriteNop(Relocation(ID{ 700920, 1476321 }, 0x56).Address(), 5);					// Disable "Out of UCode space" log spam

				const auto target1 = ID{ 721218, 1856015 };
				text.WriteNop(Relocation(target1, Offset{ 0x293, 0x254 }).Address(), 5);				// Disable "BSMeshCombiner" log spam
				text.WriteNop(Relocation(target1, Offset{ 0x2F7, 0x2B8 }).Address(), 5);				// Disable "BSMeshCombiner" log spam

				// Disable useless "Processing Topic X..." status bar updates
				text.WriteNop(Relocation(ID{ 668914, 1541476 }, Offset{ 0x97, 0xF6 }).Address(), 5);
				text.WriteNop(Relocation(ID{ 668919, 1995823 }, Offset{ 0xA3, 0xF5 }).Address(), 5);
				text.WriteNop(Relocation(ID(530432), 0x92).Address(), 5);

				// Skipping the string check is more than 33 characters, this is irrelevant for new games
				text.Write(Relocation(ID(579610), Offset{ 0xD1, 0xD3 }).Address(), { JMP });

				// Missing root material: %s.
				text.WriteNop(Relocation(ID{ 419645, 1580466 }, Offset{ 0x1FD, 0x205 }).Address(), 0x5);

				// EXTRA SPACE
				text.WriteNop(Relocation(ID(135014), 0x41).Address(), 0x35);

				// NavMesh ID %08X in cell %s has invalid cover data. It will be removed.
				text.WriteNop(Relocation(ID(548011), Offset{ 0xAFC, 0x8C5 }).Address(), 0x5);

				// Animation messages
				const auto target2 = ID{ 142900, 1621640 };
				text.WriteNop(Relocation(target2, 0x158).Address(), 5);
				text.WriteNop(Relocation(target2, Offset{ 0x61A, 0x5C4 }).Address(), 5);
				text.WriteNop(Relocation(target2, Offset{ 0x683, 0x62F }).Address(), 5);
				text.WriteNop(Relocation(target2, Offset{ 0x6E8, 0x694 }).Address(), 5);		
				text.WriteNop(Relocation(ID{ 660208, 1990223 }, Offset{ 0x1E7, 0x205 }).Address(), 5);

				// TEXTURE Unable to load file
				text.Write(Relocation(ID{ 797202, 1623969 }, Offset{ 0x157, 0x1AA }).Address(), { 0xE9, 0xB8, 0x00, 0x00, 0x00, 0x90 });

				// MODEL Could not find model
				text.WriteNop(Relocation(ID{ 305059, 1522044 }, Offset{ 0x196, 0x189 }).Address(), 5);
				text.WriteNop(Relocation(ID{ 773177, 1789956 }, Offset{ 0x223, 0x1CA }).Address(), 5);

				// SHADER Could not find material
				text.WriteNop(Relocation(ID(370815), 0x452).Address(), 5);
				text.WriteNop(Relocation(ID{ 117776, 1666411 }, Offset{ 0xBC, 0xC0 }).Address(), 5);
				
				if (VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					text.WriteNop(Relocation(ID(127277), 0xEB1).Address(), 5);
	
				// MODEL Found geometry
				text.WriteNop(Relocation(ID{ 11905, 1731262 }, Offset{ 0x876, 0x972 }).Address(), 5);

				// Always off option bAllowFileWrite:MESSAGES
				text.Write(Relocation(ID(587727)).Address(), { 0x31, 0xC0, 0xC3, 0x90 });
				
				// The format of face customization texture %s (%d) does not match that of other textures (%u) on race '%s' (%08X)
				const auto target3 = ID(313515); 
				text.WriteNop(Relocation(target3, 0xCE).Address(), 5);
				text.WriteNop(Relocation(target3, 0x12B).Address(), 5);
				text.WriteNop(Relocation(target3, Offset{ 0x1A2, 0x19D }).Address(), 5);
				text.WriteNop(Relocation(target3, Offset{ 0x29C, 0x295 }).Address(), 5);

				// Could not determine the voice type from %s
				text.WriteNop(Relocation(ID{ 620742, 1816981 }, Offset{ 0x676, 0x83C }).Address(), 5);
				
				if (VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					// Scene Form ID: (%08X), HoldPackageNextScene set to on load: true
					text.WriteNop(Relocation(ID(1957714), 0x432).Address(), 5);
					// Scene Form ID: %i, HoldPackageNextScene set to on save: %s
					text.WriteNop(Relocation(ID(384122), 0x75).Address(), 5);
					// Could not load dependencies for File '%s'.
					text.WriteNop(Relocation(ID(1995710), 0x35).Address(), 5);
					// Remove trash logs bethesda
					text.Write(ID(1352498).Address(), { RET });
				}

				const auto target4 = ID{ 663575, 1874614 };
				// Scene '%s' (%08X) has multiple actions that share ID %u
				text.WriteNop(Relocation(target4, Offset{ 0x810, 0xAAD }).Address(), 5);
				// Scene '%s' (%08X) is not a player dialogue scene but has a nonzero max dialogue distance
				text.WriteNop(Relocation(target4, Offset{ 0xC1A, 0xBF3 }).Address(), 5);

				// DEFAULT: Setting key '%s' already used in list.\nSetting keys must be unique.\n
				text.WriteNop(Relocation(ID(364903), 0xD2).Address(), 5);
				
				// DEFAULT: Setting key '%s' already used in map.\nSetting keys must be unique.\n
				text.WriteNop(Relocation(ID{ 623367, 1892575 }, Offset{ 0x97, 0x15A }).Address(), 5);

				// Empty reference group '%s' (%08X)
				text.WriteNop(Relocation(ID{ 530118 }, Offset{ 0xC6, 0xB7 }).Address(), 5);

				return true;
			}
		}
	}
}