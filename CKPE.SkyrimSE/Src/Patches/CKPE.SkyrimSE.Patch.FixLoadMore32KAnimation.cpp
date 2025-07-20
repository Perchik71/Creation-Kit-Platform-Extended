// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixLoadMore32KAnimation.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixLoadMore32KAnimation::FixLoadMore32KAnimation() : Common::Patch()
			{
				SetName("Fix Load More 32K Animation");
			}

			bool FixLoadMore32KAnimation::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixLoadMore32KAnimation::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixLoadMore32KAnimation::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixLoadMore32KAnimation::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixLoadMore32KAnimation::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixLoadMore32KAnimation::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::Write(__CKPE_OFFSET(0), { 0xB7 });

				return true;
			}
		}
	}
}