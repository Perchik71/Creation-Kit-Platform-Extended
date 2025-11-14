// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
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

			bool FixRecordSoundCapture::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1;
			}

			bool FixRecordSoundCapture::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// No terminate process for CKPE, if calling this quit message

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&PostQuitMessage);
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&PostQuitMessage);

				return true;
			}
		}
	}
}