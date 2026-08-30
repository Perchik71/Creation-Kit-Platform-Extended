// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool FixMoveToTopic::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixMoveToTopic::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMoveToTopic::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when using "Move to topic" in a quest dialogue view. 
				// Any unresolved/unused Topic actions default to "Unknown action",
				// but a null pointer is used while trying to get the type.
				//
				Relocation(ID(552353), Offset{ 0x1431, 0x1439 }).WriteFill(NOP, 5);

				return true;
			}
		}
	}
}