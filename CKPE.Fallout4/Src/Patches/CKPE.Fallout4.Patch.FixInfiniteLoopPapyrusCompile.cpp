// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.SafeWrite.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixInfiniteLoopPapyrusCompile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static Common::EditorUI::TDialogProc OldPickScriptsToCompileDlgProc = nullptr;

			static LRESULT HKPickScriptsToCompileDlgProc(void* This, UINT Message, WPARAM wParam,
				LPARAM lParam) noexcept(true)
			{
				return Common::EditorUI::CompileScriptsDialogProc(This, Message, wParam, lParam,
					Common::EditorUI::UI_FO4_COMPILESCRIPT_DIALOG_LISTVIEW, OldPickScriptsToCompileDlgProc);
			}

			FixInfiniteLoopPapyrusCompile::FixInfiniteLoopPapyrusCompile() : Common::Patch()
			{
				SetName("Fixed infinite loop by Compile Papyrus Scripts");
			}

			bool FixInfiniteLoopPapyrusCompile::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixInfiniteLoopPapyrusCompile::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixInfiniteLoopPapyrusCompile::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixInfiniteLoopPapyrusCompile::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixInfiniteLoopPapyrusCompile::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixInfiniteLoopPapyrusCompile::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Fixed infinite loop by Compile Papyrus Scripts...
				Relocation(EditorAPI::IDs::Functions::PapyrusCompileDialog_UpdateButtons).Write(RET);
				*(std::uintptr_t*)&OldPickScriptsToCompileDlgProc =
					Relocation(EditorAPI::IDs::Functions::PapyrusCompileDialog_DlgProc).WriteJump(&HKPickScriptsToCompileDlgProc);

				return true;
			}
		}
	}
}