// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
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
				auto ver = db->GetVersion();
				if ((ver < 1) && (ver > 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				// Fixed crash when by load plugin in which there is no root parent to materials

				if (ver == 1)
				{
					text.Write(__CKPE_OFFSET(0), { 0x0F, 0xC6, 0x1D, 0x1C, 0xC0, 0x30, 0x01, 0xEE,
						0x0F, 0xC6, 0x05, 0x14, 0xC0, 0x30, 0x01, 0x44, 0x31, 0xC0, 0xEB, 0x4B });
					text.Write(__CKPE_OFFSET(1), { 0xEB, 0x6F });
					text.Write(__CKPE_OFFSET(2), { 0xEB });

					return true;
				}
				else if (ver == 2)
				{
					auto rva = __CKPE_OFFSET(0);
					auto rva_data = __CKPE_OFFSET(3);
					text.Write(rva, { 0x0F, 0xC6, 0x1D });
					auto RelOff = (std::uint32_t)(rva_data - (rva + 8));
					text.Write(rva + 3, (std::uint8_t*)&RelOff, 4);
					text.Write(rva + 7, { 0xEE });
					rva += 8;
					text.Write(rva, { 0x0F, 0xC6, 0x05 });
					RelOff = (std::uint32_t)(rva_data - (rva + 8));
					text.Write(rva + 3, (std::uint8_t*)&RelOff, 4);
					text.Write(rva + 7, { 0x44, 0x31, 0xC0, 0xEB, 0x4D });
					text.Write(__CKPE_OFFSET(1), { 0xEB, 0x74 });
					text.Write(__CKPE_OFFSET(2), { 0xEB });

					return true;
				}

				return true;
			}
		}
	}
}