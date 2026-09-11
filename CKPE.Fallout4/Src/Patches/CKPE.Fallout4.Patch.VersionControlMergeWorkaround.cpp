// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.VersionControlMergeWorkaround.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			VersionControlMergeWorkaround::VersionControlMergeWorkaround() : Common::Patch()
			{
				SetName("Version Control Merge Workaround");
			}

			bool VersionControlMergeWorkaround::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* VersionControlMergeWorkaround::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bVersionControlMergeWorkaround";
			}

			bool VersionControlMergeWorkaround::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> VersionControlMergeWorkaround::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool VersionControlMergeWorkaround::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool VersionControlMergeWorkaround::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				auto interface = CKPE::Common::Interface::GetSingleton();

				//
				// Workaround for version control not allowing merges when a plugin index is above 02.
				// Bethesda's VC bitmap files determine heck-in status along with user IDs for each specific form in the game.
				// They're also hardcoded for 2 masters only. Using this hack for anything EXCEPT merging will break the bitmaps.
				//

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				const auto target1 = ID{ 76508, 1361378 };
				const auto target2 = ID(49336);
				const auto target3 = ID(581077);
				const auto target4 = ID(581078);
				const auto target5 = ID(438785);

				text.Write(Relocation(target1, Offset{ 0x21f, 0x123 }).Address(), { JMP });
				text.Write(Relocation(target1, Offset{ 0x242, 0x3AF }).Address(), { JMP });
				text.Write(Relocation(target2, 0x17).Address(), { JMP });
				text.Write(Relocation(target2, 0x3A).Address(), { JMP });
				text.Write(Relocation(target3, Offset{ 0x497, 0x41E }).Address(), { JMP });
				text.Write(Relocation(target3, Offset{ 0x4BA, 0x441 }).Address(), { JMP });
				text.Write(Relocation(target4, Offset{ 0x218, 0x238 }).Address(), { JMP });
				text.Write(Relocation(target4, Offset{ 0x23B, 0x25B }).Address(), { JMP });
				text.Write(Relocation(target5, 0x4E).Address(), { JMP });
				text.Write(Relocation(target5, 0x6F).Address(), { JMP });

				return true;
			}
		}
	}
}