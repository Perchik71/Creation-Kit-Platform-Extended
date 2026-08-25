// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TCrashAfterMultipleMastersWarningSub = void(std::int64_t);

			static std::function<TCrashAfterMultipleMastersWarningSub> CrashAfterMultipleMastersWarningSub;

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

			bool CrashAfterMultipleMastersWarning::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashAfterMultipleMastersWarning::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashAfterMultipleMastersWarning::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash after the "Multiple masters selected for load" dialog is shown.
				// Missing null pointer check in Sky::UpdateAurora.
				//
				Relocation(ID(555984), 0xC9).WriteCall(&sub);
				CrashAfterMultipleMastersWarningSub = Relocation<TCrashAfterMultipleMastersWarningSub>(ID(555729)).Get();

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