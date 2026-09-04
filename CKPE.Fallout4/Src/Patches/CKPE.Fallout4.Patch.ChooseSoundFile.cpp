// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.ChooseSoundFile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			using TChooseSoundFileSub = bool(std::int64_t, const char*, const char*,
				const char*, char*, void*, std::int32_t, bool, const char*, std::uint32_t,
				const char*, void*, std::int64_t, std::int64_t, std::int64_t, std::int64_t);

			static std::function<TChooseSoundFileSub> ChooseSoundFileSub;

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
				using namespace Common;

				//
				// The fixed is to select an audio file, also *.xwm.
				//

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					Relocation(ID(417165), 0xD0).WriteFill(NOP, 0x6);

					auto target = ID(138153);
					Relocation(target, 0x67).WriteCall(&sub);
					Relocation(target, 0xF9).WriteFill(NOP, 0x2);

					Relocation(ID(284852), 0x5D).WriteFill(NOP, 0x2);
					Relocation(ID(330881), 0xAE).WriteCall(&sub);
					Relocation(ID(356112), 0x4CE).WriteCall(&sub);

					ChooseSoundFileSub = reinterpret_cast<TChooseSoundFileSub*>(ID(411272).Address());
					Relocation(ID(440489), 0x32).WriteFill(NOP, 0x2);
				}
				else
				{
					Relocation(ID(1578071), 0x63).WriteCall(&sub);
					Relocation(ID(1623267), 0x87).WriteCall(&sub);
					Relocation(ID(1413124), 0x3CB).WriteCall(&sub);
					Relocation(ID(356112), 0x57B).WriteCall(&sub);
					ChooseSoundFileSub = reinterpret_cast<TChooseSoundFileSub*>(ID(1353832).Address());
					Relocation(ID(1623270), 0x194).WriteFill(NOP, 0x6);
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