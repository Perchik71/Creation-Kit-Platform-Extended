// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
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

			bool SkipTopicInfoValidation::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool SkipTopicInfoValidation::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Skip 'Topic Info' validation during load
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xC3 });

				return true;
			}
		}
	}
}