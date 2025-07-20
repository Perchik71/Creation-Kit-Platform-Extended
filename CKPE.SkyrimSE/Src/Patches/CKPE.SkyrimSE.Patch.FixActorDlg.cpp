// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixActorDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixActorDlg::FixActorDlg() : Common::Patch()
			{
				SetName("Fix Actor Dlg");
			}

			bool FixActorDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixActorDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixActorDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixActorDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixActorDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixActorDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for the "Actor Flags" or "Actor Behavior" dialogs not showing their column headers. 
				// wParam was swapped to lParam for an unknown reason in SE only. Undo that change.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x80, 0x12, 0x00, 0x00 });

				return true;
			}
		}
	}
}