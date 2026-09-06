// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashNoRootMat.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixCrashNoRootMat::FixCrashNoRootMat() : Common::Patch()
			{
				SetName("Fixed no root parent to materials");
			}

			bool FixCrashNoRootMat::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashNoRootMat::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashNoRootMat::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashNoRootMat::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashNoRootMat::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixCrashNoRootMat::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					text.Write(Relocation(ID(649982), 0x1C).Address(), { 0x0F, 0xC6, 0x1D, 0x1C, 0xC0, 0x30, 0x01, 0xEE,
							0x0F, 0xC6, 0x05, 0x14, 0xC0, 0x30, 0x01, 0x44, 0x31, 0xC0, JMP, 0x4B });
					text.Write(Relocation(ID(54722), 0x39F).Address(), { JMP, 0x6F });
					text.Write(Relocation(ID(797136), 0x317).Address(), { JMP });
				}
				else
				{
					// Fixed crash when by load plugin in which there is no root parent to materials
					auto rva = Relocation(ID(1471571), 0x1E).Address();
					auto rva_data = Relocation(ID(351273)).Address();

					text.Write(rva, { 0x0F, 0xC6, 0x1D });
					auto RelOff = (std::uint32_t)(rva_data - (rva + 8));
					text.Write(rva + 3, (std::uint8_t*)&RelOff, 4);
					text.Write(rva + 7, { 0xEE });
					rva += 8;
					text.Write(rva, { 0x0F, 0xC6, 0x05 });
					RelOff = (std::uint32_t)(rva_data - (rva + 8));
					text.Write(rva + 3, (std::uint8_t*)&RelOff, 4);
					text.Write(rva + 7, { 0x44, 0x31, 0xC0, JMP, 0x4D });
					text.Write(Relocation(ID(1382630), 0x3A2).Address(), { JMP, 0x74 });
					text.Write(Relocation(ID(1619614), 0x348).Address(), { JMP });
				}

				return true;
			}
		}
	}
}