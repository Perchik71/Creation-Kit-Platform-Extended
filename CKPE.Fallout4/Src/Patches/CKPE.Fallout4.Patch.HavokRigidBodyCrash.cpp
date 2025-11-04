// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.HavokRigidBodyCrash.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			HavokRigidBodyCrash::HavokRigidBodyCrash() : Common::Patch()
			{
				SetName("Havok Rigid Body Crash");
			}

			bool HavokRigidBodyCrash::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* HavokRigidBodyCrash::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool HavokRigidBodyCrash::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> HavokRigidBodyCrash::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool HavokRigidBodyCrash::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool HavokRigidBodyCrash::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::Write(__CKPE_OFFSET(0), { 0x60 });

				return true;
			}
		}
	}
}