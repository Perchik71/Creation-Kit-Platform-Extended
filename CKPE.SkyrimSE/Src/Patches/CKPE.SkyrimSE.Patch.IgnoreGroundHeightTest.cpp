// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.IgnoreGroundHeightTest.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			IgnoreGroundHeightTest::IgnoreGroundHeightTest() : Common::Patch()
			{
				SetName("Ignore Ground Height Test");
			}

			bool IgnoreGroundHeightTest::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* IgnoreGroundHeightTest::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bIgnoreGroundHeightTest";
			}

			bool IgnoreGroundHeightTest::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> IgnoreGroundHeightTest::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool IgnoreGroundHeightTest::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool IgnoreGroundHeightTest::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// At this point, the obtained height is compared with an incorrect value. 
				// Skip all this if the user requested it in the settings.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xE9, 0xB1, 0x02, 0x00, 0x00, 0x90 });

				return true;
			}
		}
	}
}