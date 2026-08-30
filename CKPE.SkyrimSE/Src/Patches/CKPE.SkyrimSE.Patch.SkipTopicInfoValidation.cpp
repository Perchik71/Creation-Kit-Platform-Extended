// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.SkipTopicInfoValidation.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			SkipTopicInfoValidation::SkipTopicInfoValidation() : Common::Patch()
			{
				SetName("Skip Topic Info Validation");
			}

			bool SkipTopicInfoValidation::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* SkipTopicInfoValidation::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bSkipTopicInfoValidation";
			}

			bool SkipTopicInfoValidation::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> SkipTopicInfoValidation::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool SkipTopicInfoValidation::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool SkipTopicInfoValidation::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool SkipTopicInfoValidation::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Skip 'Topic Info' validation during load
				//
				Relocation(ID(380307)).Write(RET);

				return true;
			}
		}
	}
}