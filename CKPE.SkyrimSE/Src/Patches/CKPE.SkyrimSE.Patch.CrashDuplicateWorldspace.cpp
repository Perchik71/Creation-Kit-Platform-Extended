// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashDuplicateWorldspace.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void*(*TCrashDuplicateWorldspaceSub)(void*);

			static TCrashDuplicateWorldspaceSub CrashDuplicateWorldspaceSub;

			CrashDuplicateWorldspace::CrashDuplicateWorldspace() : Common::Patch()
			{
				SetName("Crash Duplicate Worldspace");
			}

			bool CrashDuplicateWorldspace::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashDuplicateWorldspace::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashDuplicateWorldspace::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashDuplicateWorldspace::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashDuplicateWorldspace::DoQuery() const noexcept(true)
			{
				// In 1.6.1130 this fixed.
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool CrashDuplicateWorldspace::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when duplicating worldspaces
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				CrashDuplicateWorldspaceSub = (TCrashDuplicateWorldspaceSub)__CKPE_OFFSET(1);

				return true;
			}

			void* CrashDuplicateWorldspace::sub(void* a1) noexcept(true)
			{
				if (!a1)
					return nullptr;
				
				return CrashDuplicateWorldspaceSub(a1);
			}
		}
	}
}