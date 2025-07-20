// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.NavMeshInfoMap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			NavMeshInfoMap::NavMeshInfoMap() : Common::Patch()
			{
				SetName("NavMesh Info Map");
			}

			bool NavMeshInfoMap::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* NavMeshInfoMap::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool NavMeshInfoMap::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> NavMeshInfoMap::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool NavMeshInfoMap::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool NavMeshInfoMap::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix use-after-free with a NavMeshInfoMap inserted in the altered forms list during a virtual destructor call. 
				// NavMeshInfoMap::Clear.
				//
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 6);

				return true;
			}
		}
	}
}