// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.UnEquipSound.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			UnEquipSound::UnEquipSound() : Common::Patch()
			{
				SetName("UnEquip Sound");
			}

			bool UnEquipSound::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* UnEquipSound::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool UnEquipSound::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> UnEquipSound::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool UnEquipSound::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool UnEquipSound::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool UnEquipSound::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Correct the "Push-to-game not supported" error when clicking the "UnEquip Sound" button on the weapon editor
				// dialog or "Add" button on the music track dialog. 3682 is reserved exclusively for the PTG functionality, so
				// the button ids must be changed. Disable PTG code instead.
				//
				Relocation(ID(175922), Offset{ 0xA99, 0xAA1 }).WriteFill(NOP, 6);

				return true;
			}
		}
	}
}