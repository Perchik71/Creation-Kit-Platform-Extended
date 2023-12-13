// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixSpellDlg.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixSpellDlgPatch::FixSpellDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool FixSpellDlgPatch::HasOption() const
			{
				return false;
			}

			bool FixSpellDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSpellDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSpellDlgPatch::GetName() const
			{
				return "Fix Spell Dlg";
			}

			bool FixSpellDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSpellDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix charset Spell/Scroll/Ench etc dialoges
					//
					OldSpellDlgProc = (DLGPROC)Detours::X64::DetourFunctionClass(
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), 
						&EditorAPI::SkyrimSpectialEdition::SpellDlgProc);
	
					return true;
				}

				return false;
			}

			bool FixSpellDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}