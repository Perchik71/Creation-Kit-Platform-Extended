// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.EnableGoInSelGame.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			EnableGoInSelGame::EnableGoInSelGame() : Common::Patch()
			{
				SetName("Go to selection in game");
			}

			bool EnableGoInSelGame::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* EnableGoInSelGame::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool EnableGoInSelGame::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> EnableGoInSelGame::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool EnableGoInSelGame::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool EnableGoInSelGame::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Enable the render window "Go to selection in game" hotkey even if version control is off
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 2);

				return true;
			}
		}
	}
}