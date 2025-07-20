// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixRemoteDesktop.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixRemoteDesktop::FixRemoteDesktop() : Common::Patch()
			{
				SetName("Fix Remote Desktop");
			}

			bool FixRemoteDesktop::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixRemoteDesktop::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixRemoteDesktop::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixRemoteDesktop::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixRemoteDesktop::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRemoteDesktop::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::Write(__CKPE_OFFSET(0), { 0xEB });

				return true;
			}
		}
	}
}