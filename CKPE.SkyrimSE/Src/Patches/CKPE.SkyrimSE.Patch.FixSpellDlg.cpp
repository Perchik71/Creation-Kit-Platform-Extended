// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
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

			bool FixSpellDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSpellDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix charset Spell/Scroll/Ench etc dialoges
				//
				EditorAPI::OldSpellDlgProc = (DLGPROC)Detours::DetourClassJump(__CKPE_OFFSET(0), &EditorAPI::SpellDlgProc);

				return true;
			}
		}
	}
}