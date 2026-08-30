// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool FixBSShadowDirectionalLight::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixBSShadowDirectionalLight::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBSShadowDirectionalLight::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Memory bug fix during BSShadowDirectionalLight calculations (see game patch for more information)
				//
				Relocation(ID(522853), Offset{ 0x20DD, 0x2080, 0x2080, 0x2096 }).Write
					({ 0x4D, 0x89, 0xE1, 0x90, 0x90, 0x90, 0x90 });

				return true;
			}
		}
	}
}