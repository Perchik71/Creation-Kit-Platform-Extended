// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixBSShadowDirectionalLight.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixBSShadowDirectionalLight::FixBSShadowDirectionalLight() : Common::Patch()
			{
				SetName("BSShadowDirectionalLight Fix");
			}

			bool FixBSShadowDirectionalLight::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixBSShadowDirectionalLight::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixBSShadowDirectionalLight::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixBSShadowDirectionalLight::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixBSShadowDirectionalLight::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBSShadowDirectionalLight::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Memory bug fix during BSShadowDirectionalLight calculations (see game patch for more information)
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x4D, 0x89, 0xE1, 0x90, 0x90, 0x90, 0x90 });

				return true;
			}
		}
	}
}