// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool FixRemoteDesktop::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixRemoteDesktop::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRemoteDesktop::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;
				
				Relocation(ID(326873), 0x9C0).Write(JMP);

				return true;
			}
		}
	}
}