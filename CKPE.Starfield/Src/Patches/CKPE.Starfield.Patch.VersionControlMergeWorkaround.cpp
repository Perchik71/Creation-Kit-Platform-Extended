// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.VersionControlMergeWorkaround.h>

namespace CKPE
{
	namespace Starfield
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool VersionControlMergeWorkaround::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Workaround for version control not allowing merges when a plugin index is above 02.
				// Bethesda's VC bitmap files determine heck-in status along with user IDs for each specific form in the game. 
				// They're also hardcoded for 2 masters only. Using this hack for anything EXCEPT merging will break the bitmaps.
				//

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				for (std::uint32_t i = 0; i < db->GetCount(); i++)
					text.Write(__CKPE_OFFSET(i), { 0xEB });

				return true;
			}
		}
	}
}