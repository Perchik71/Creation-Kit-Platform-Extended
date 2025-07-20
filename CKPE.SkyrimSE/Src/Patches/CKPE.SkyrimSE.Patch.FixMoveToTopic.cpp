// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixMoveToTopic.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixMoveToTopic::FixMoveToTopic() : Common::Patch()
			{
				SetName("Fix Move to topic");
			}

			bool FixMoveToTopic::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixMoveToTopic::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixMoveToTopic::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixMoveToTopic::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixMoveToTopic::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMoveToTopic::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when using "Move to topic" in a quest dialogue view. 
				// Any unresolved/unused Topic actions default to "Unknown action",
				// but a null pointer is used while trying to get the type.
				//
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 5);

				return true;
			}
		}
	}
}