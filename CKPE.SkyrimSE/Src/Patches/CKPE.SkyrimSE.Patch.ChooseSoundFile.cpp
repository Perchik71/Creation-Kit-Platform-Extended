// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.ChooseSoundFile.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef bool(*TChooseSoundFileSub)(std::int64_t, const char*, const char*,
				const char*, char*, void*, std::int32_t, bool, const char*, std::uint32_t, 
				const char*, void*);

			static TChooseSoundFileSub ChooseSoundFileSub;

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
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
				ChooseSoundFileSub = (TChooseSoundFileSub)__CKPE_OFFSET(2);

				return true;
			}

			bool ChooseSoundFile::sub(std::int64_t iHandle, const char* lpPath, const char* lpFormat,
				const char* lpCaption, char* lpFileName, void* pHandler, std::int32_t iUnknown,
				bool bSaveDlg, const char* lpUnknownStr, std::uint32_t uUnknown, const char* lpBuffer, 
				void* _CrtBuffer) noexcept(true)
			{
				return ChooseSoundFileSub(iHandle, lpPath,
					"xWMA Files\0*.xwm\0Wave PCM\0*.wav\0All Files\0*.*\0\0",
					lpCaption, lpFileName, pHandler, iUnknown, bSaveDlg, lpUnknownStr, uUnknown, 
					lpBuffer, _CrtBuffer);
			}
		}
	}
}