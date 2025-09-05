// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.RemoveSpamMessage.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			RemoveSpamMessage::RemoveSpamMessage() : Common::Patch()
			{
				SetName("Spam Message");
			}

			bool RemoveSpamMessage::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RemoveSpamMessage::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RemoveSpamMessage::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RemoveSpamMessage::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RemoveSpamMessage::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_78_0;
			}

			bool RemoveSpamMessage::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::Write(__CKPE_OFFSET(0), { 0xC3, 0x90, 0x90, 0x90, 0x90 });

				return true;
			}
		}
	}
}