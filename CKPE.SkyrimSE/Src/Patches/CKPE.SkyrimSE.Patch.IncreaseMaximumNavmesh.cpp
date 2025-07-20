// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.IncreaseMaximumNavmesh.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			IncreaseMaximumNavmesh::IncreaseMaximumNavmesh() : Common::Patch()
			{
				SetName("Increase the maximum navmesh");
			}

			bool IncreaseMaximumNavmesh::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* IncreaseMaximumNavmesh::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool IncreaseMaximumNavmesh::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> IncreaseMaximumNavmesh::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool IncreaseMaximumNavmesh::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool IncreaseMaximumNavmesh::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Increase the maximum navmesh autogeneration cell limit to 100,000 and prevent spamming UI updates (0.01% -> 1.00%)
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xA0, 0x86, 0x01, 0x00 });
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);

				return true;
			}

			float IncreaseMaximumNavmesh::sub(float Delta) noexcept(true)
			{
				return std::abs(Delta) / 100.0f;
			}
		}
	}
}