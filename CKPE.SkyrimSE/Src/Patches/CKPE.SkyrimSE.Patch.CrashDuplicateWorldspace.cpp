// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TCrashDuplicateWorldspaceSub = void*(void*);

			static std::function<TCrashDuplicateWorldspaceSub> CrashDuplicateWorldspaceSub;

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

			bool CrashDuplicateWorldspace::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashDuplicateWorldspace::DoQuery() const noexcept(true)
			{
				// In 1.6.1130 this fixed.
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool CrashDuplicateWorldspace::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when duplicating worldspaces
				//
				CrashDuplicateWorldspaceSub = reinterpret_cast<TCrashDuplicateWorldspaceSub*>(
					Relocation(ID(443063), 0x73A).WriteCall(&sub));

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