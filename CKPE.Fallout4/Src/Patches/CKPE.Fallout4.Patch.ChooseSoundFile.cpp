// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.ChooseSoundFile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_ChooseSoundFile_sub = 0;

			ChooseSoundFile::ChooseSoundFile() : Common::Patch()
			{
				SetName("Choose Sound File");
			}

			bool ChooseSoundFile::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ChooseSoundFile::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ChooseSoundFile::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ChooseSoundFile::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ChooseSoundFile::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool ChooseSoundFile::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// The fixed is to select an audio file, also *.xwm.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub);
				pointer_ChooseSoundFile_sub = __CKPE_OFFSET(4);

				return true;
			}

			bool ChooseSoundFile::sub(std::int64_t unknown, const char* lpPath, const char* lpFormat, const char* lpCaption,
				char* lpFileName) noexcept(true)
			{
				return fast_call<bool>(pointer_ChooseSoundFile_sub, unknown, lpPath,
					"xWMA Files\0*.xwm\0Wave PCM\0*.wav\0All Files\0*.*\0",
					lpCaption, lpFileName);
			}
		}
	}
}