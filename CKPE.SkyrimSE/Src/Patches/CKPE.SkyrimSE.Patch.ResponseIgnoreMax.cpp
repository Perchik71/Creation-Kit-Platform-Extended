// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.ResponseIgnoreMax.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			ResponseIgnoreMax::ResponseIgnoreMax() : Common::Patch()
			{
				SetName("Response Ignore Max");
			}

			bool ResponseIgnoreMax::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ResponseIgnoreMax::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ResponseIgnoreMax::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ResponseIgnoreMax::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ResponseIgnoreMax::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ResponseIgnoreMax::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Skip message setting blocking text input after 149 characters.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xEB });

				return true;
			}
		}
	}
}