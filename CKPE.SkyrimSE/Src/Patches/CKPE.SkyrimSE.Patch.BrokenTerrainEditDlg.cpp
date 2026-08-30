// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool BrokenTerrainEditDlg::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool BrokenTerrainEditDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool BrokenTerrainEditDlg::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for broken terrain edit dialog undo functionality (Incorrectly removing elements from a linked list,
				// still contains a memory leak)
				//

				auto target = ID(187657);

				Relocation(target, Offset{ 0x11F, 0x153 }).WriteFill(NOP, 4);
				Relocation(target, Offset{ 0x16B, 0x1C8 }).WriteFill(NOP, 4);

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_SKYRIM_SE_1_5_73)
					Relocation(target, 0x6D1).WriteFill(NOP, 4);

				return true;
			}
		}
	}
}