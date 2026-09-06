// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.SkipUpdateCheck.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			SkipUpdateCheck::SkipUpdateCheck() : Common::Patch()
			{
				SetName("Skipping the program update check");
			}

			bool SkipUpdateCheck::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* SkipUpdateCheck::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool SkipUpdateCheck::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> SkipUpdateCheck::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool SkipUpdateCheck::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool SkipUpdateCheck::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				Relocation(ID{ 643506, 2007835 }, Offset{ 0x1CB, 0x1CB, 0x21F }).Write({ 0xEB, 0x10 });

				return true;
			}
		}
	}
}