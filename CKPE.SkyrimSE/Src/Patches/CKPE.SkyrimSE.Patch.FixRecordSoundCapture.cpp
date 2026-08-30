// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixRecordSoundCapture.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixRecordSoundCapture::FixRecordSoundCapture() : Common::Patch()
			{
				SetName("Fix Record Sound Capture");
			}

			bool FixRecordSoundCapture::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixRecordSoundCapture::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixRecordSoundCapture::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixRecordSoundCapture::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixRecordSoundCapture::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixRecordSoundCapture::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRecordSoundCapture::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// No terminate process for CKPE, if calling this quit message
				auto target = ID(342981);
				Relocation(target, 0x8A).WriteCall(&PostQuitMessage);
				Relocation(target, 0x10B).WriteCall(&PostQuitMessage);

				return true;
			}
		}
	}
}