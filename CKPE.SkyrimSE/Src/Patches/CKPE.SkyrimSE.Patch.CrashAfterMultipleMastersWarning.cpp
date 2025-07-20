// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashAfterMultipleMastersWarning.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void(*TCrashAfterMultipleMastersWarningSub)(std::int64_t);

			static TCrashAfterMultipleMastersWarningSub CrashAfterMultipleMastersWarningSub;

			CrashAfterMultipleMastersWarning::CrashAfterMultipleMastersWarning() : Common::Patch()
			{
				SetName("Crash after Multiple Masters Warning");
			}

			bool CrashAfterMultipleMastersWarning::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashAfterMultipleMastersWarning::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashAfterMultipleMastersWarning::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashAfterMultipleMastersWarning::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashAfterMultipleMastersWarning::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashAfterMultipleMastersWarning::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash after the "Multiple masters selected for load" dialog is shown.
				// Missing null pointer check in Sky::UpdateAurora.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				CrashAfterMultipleMastersWarningSub = (TCrashAfterMultipleMastersWarningSub)__CKPE_OFFSET(1);

				return true;
			}

			void CrashAfterMultipleMastersWarning::sub(std::int64_t a1) noexcept(true)
			{
				if (*(std::int64_t*)(a1 + 0x58))
					CrashAfterMultipleMastersWarningSub(a1);
			}
		}
	}
}