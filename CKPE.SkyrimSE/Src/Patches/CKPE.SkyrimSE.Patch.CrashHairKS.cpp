// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashHairKS.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			CrashHairKS::CrashHairKS() : Common::Patch()
			{
				SetName("Crash HairKS");
			}

			bool CrashHairKS::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashHairKS::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashHairKS::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashHairKS::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashHairKS::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashHairKS::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix crash When getting some kind of index. Sometimes there is an object where after dereferencing rax 
				// becomes equal to 1, which leads to a crash when dereferencing it. Let's add the check to 1.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x0E, 0x83, 0xF8, 0x01, 0x74, 0x09,
					0x8B, 0x40, 0xF8, 0x25, 0xFF, 0xFF, 0xFF, 0x00, 0xC3, 0xC3 });
				SafeWrite::Write(__CKPE_OFFSET(1), { 0x0E, 0x83, 0xF8, 0x01, 0x74, 0x09,
					0x8B, 0x40, 0xF8, 0x25, 0xFF, 0xFF, 0xFF, 0x00, 0xC3, 0xC3 });

				return true;
			}
		}
	}
}