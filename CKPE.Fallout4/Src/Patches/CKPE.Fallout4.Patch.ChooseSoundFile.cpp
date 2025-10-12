// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
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
			typedef bool(*TChooseSoundFileSub)(std::int64_t, const char*, const char*,
				const char*, char*, void*, std::int32_t, bool, const char*, std::uint32_t,
				const char*, void*, std::int64_t, std::int64_t, std::int64_t, std::int64_t);

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
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool ChooseSoundFile::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((db->GetVersion() != 1) && (db->GetVersion() != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// The fixed is to select an audio file, also *.xwm.
				//
				if (verPatch == 1)
				{
					SafeWrite::WriteNop(__CKPE_OFFSET(0), 6);
					Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
					SafeWrite::WriteNop(__CKPE_OFFSET(2), 2);
					SafeWrite::WriteNop(__CKPE_OFFSET(3), 2);
					Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&sub);
					ChooseSoundFileSub = (TChooseSoundFileSub)__CKPE_OFFSET(6);
					SafeWrite::WriteNop(__CKPE_OFFSET(7), 2);
				}
				else
				{
					Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub);
					ChooseSoundFileSub = (TChooseSoundFileSub)__CKPE_OFFSET(4);
					SafeWrite::WriteNop(__CKPE_OFFSET(5), 6);
				}

				return true;
			}

			bool ChooseSoundFile::sub(std::int64_t unk01, const char* lpPath, const char* lpFormat, const char* lpCaption,
				char* lpFileName, void* pHandler, std::int32_t iUnknown, bool bSaveDlg, const char* lpUnknownStr,
				std::uint32_t uUnknown, const char* lpBuffer, void* _CrtBuffer, std::int64_t unk02, 
				std::int64_t unk03, std::int64_t unk04, std::int64_t unk05) noexcept(true)
			{
				return ChooseSoundFileSub(unk01, lpPath, "xWMA Files\0*.xwm\0Wave PCM\0*.wav\0All Files\0*.*\0\0",
					lpCaption, lpFileName, pHandler, iUnknown, bSaveDlg, lpUnknownStr, uUnknown, lpBuffer, _CrtBuffer,
					unk02, unk03, unk04, unk05);
			}
		}
	}
}