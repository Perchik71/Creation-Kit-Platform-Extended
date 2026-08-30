// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool IncreaseMaximumNavmesh::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool IncreaseMaximumNavmesh::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool IncreaseMaximumNavmesh::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Increase the maximum navmesh autogeneration cell limit to 100,000 and prevent spamming UI updates (0.01% -> 1.00%)
				//
				Relocation(ID(16881), 0x2A6).Write({ 0xA0, 0x86, 0x01, 0x00 });
				Relocation(ID(239902), 0x68).WriteCall(&sub);

				return true;
			}

			float IncreaseMaximumNavmesh::sub(float Delta) noexcept(true)
			{
				return std::abs(Delta) / 100.0f;
			}
		}
	}
}