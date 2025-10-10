// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.ResponseIgnoreMax.h>

namespace CKPE
{
	namespace Fallout4
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
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
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 6);

				return true;
			}
		}
	}
}