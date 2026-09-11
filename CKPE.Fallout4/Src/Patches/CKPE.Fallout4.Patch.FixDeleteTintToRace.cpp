// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixDeleteTintToRace.h>

#include <commctrl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixDeleteTintToRace::FixDeleteTintToRace() : Common::Patch()
			{
				SetName("Fixed delete group tinting to race");
			}

			bool FixDeleteTintToRace::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixDeleteTintToRace::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixDeleteTintToRace::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixDeleteTintToRace::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixDeleteTintToRace::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixDeleteTintToRace::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Fixed when you delete a group tinting to race window
				const auto target = ID{ 286613, 1992272 };
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					Relocation(target, 0x19E).Write({ 0x4D, 0x8B, 0x47, 0x8, 0x4C, 0x89, 0xE2 });
					Relocation(target, 0x1A5).WriteCall(&sub);
					Relocation(target, 0x1AA).Write({ 0xEB, 0x18 });
				}
				else
				{
					Relocation(target, 0x2C2).Write({ 0x4C, 0x8B, 0x46, 0x8, 0x89, 0xC1, 0x4C, 0x89, 0xF2 });
					Relocation(target, 0x2CB).WriteCall(&sub);
					Relocation(target, 0x2D0).Write({ 0xEB, 0x22 });
				}

				return true;
			}

			void FixDeleteTintToRace::sub(std::int64_t count, std::int64_t item_id, std::uintptr_t listview) noexcept(true)
			{
				if (!count) return;
				for (auto i = 0; i < count; i++)
					ListView_DeleteItem((HWND)listview, item_id);
			}
		}
	}
}