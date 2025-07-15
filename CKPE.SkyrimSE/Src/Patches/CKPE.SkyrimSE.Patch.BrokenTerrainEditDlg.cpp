// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.BrokenTerrainEditDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			BrokenTerrainEditDlg::BrokenTerrainEditDlg() : Common::Patch()
			{
				SetName("Broken Terrain Edit Dlg");
			}

			bool BrokenTerrainEditDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BrokenTerrainEditDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BrokenTerrainEditDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BrokenTerrainEditDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BrokenTerrainEditDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool BrokenTerrainEditDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for broken terrain edit dialog undo functionality (Incorrectly removing elements from a linked list,
				// still contains a memory leak)
				//

				switch (db->GetVersion())
				{
				case 1:
					SafeWrite::WriteNop(__CKPE_OFFSET(0), 4);
					SafeWrite::WriteNop(__CKPE_OFFSET(1), 4);
					SafeWrite::WriteNop(__CKPE_OFFSET(2), 4);
					break;
				case 2:
					SafeWrite::WriteNop(__CKPE_OFFSET(0), 4);
					SafeWrite::WriteNop(__CKPE_OFFSET(1), 4);
					break;
				default:
					return false;
				}

				return true;				
			}
		}
	}
}