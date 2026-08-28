// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/TESDialogSpell.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixSpellDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixSpellDlg::FixSpellDlg() : Common::Patch()
			{
				SetName("Fix Spell Dlg");
			}

			bool FixSpellDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSpellDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSpellDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSpellDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSpellDlg::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixSpellDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSpellDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix charset Spell/Scroll/Ench etc dialoges
				//
				EditorAPI::OldSpellDlgProc = reinterpret_cast<DLGPROC>(Relocation(ID(175922)).WriteJump(&EditorAPI::SpellDlgProc));
				
				return true;
			}
		}
	}
}