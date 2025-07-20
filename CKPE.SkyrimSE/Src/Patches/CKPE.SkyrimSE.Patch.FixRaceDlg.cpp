// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixRaceDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixRaceDlg::FixRaceDlg() : Common::Patch()
			{
				SetName("Fix Race Dlg");
			}

			bool FixRaceDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixRaceDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixRaceDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixRaceDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixRaceDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRaceDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for the "Race" dialog not saving "Specials" if the list box was empty and a new spell was added. 
				// Inverted comparison logic.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x74 });

				return true;
			}
		}
	}
}