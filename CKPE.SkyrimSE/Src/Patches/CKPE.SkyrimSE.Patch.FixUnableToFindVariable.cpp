// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixUnableToFindVariable.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixUnableToFindVariable::FixUnableToFindVariable() : Common::Patch()
			{
				SetName("Fix Unable To Find Variable");
			}

			bool FixUnableToFindVariable::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixUnableToFindVariable::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixUnableToFindVariable::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixUnableToFindVariable::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixUnableToFindVariable::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixUnableToFindVariable::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixUnableToFindVariable::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash (invalid parameter termination) when the "Unable to find variable" warning would exceed 
				// the buffer size
				//
				static const auto str = ", Text \"%.240s\"";
				Relocation(ID(164745)).Write(str, strlen(str) + 1);
				
				return true;
			}
		}
	}
}