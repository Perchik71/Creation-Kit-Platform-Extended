// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixBrightLightColor.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void(*TFixBrightLightColorSub)(std::int64_t a1, std::uint32_t);

			static TFixBrightLightColorSub FixBrightLightColorSub;

			FixBrightLightColor::FixBrightLightColor() : Common::Patch()
			{
				SetName("Fix Bright Light Color");
			}

			bool FixBrightLightColor::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixBrightLightColor::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixBrightLightColor::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixBrightLightColor::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixBrightLightColor::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBrightLightColor::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for the "Bright Light Color" option having incorrect colors in the preferences window. 
				// The blue and green channels are swapped.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&sub);
				FixBrightLightColorSub = (TFixBrightLightColorSub)__CKPE_OFFSET(1);

				return true;
			}

			void FixBrightLightColor::sub(std::int64_t a1, std::uint32_t Color) noexcept(true)
			{
				// Blue and green channels are swapped
				std::uint32_t newColor = Color & 0xFF;
				newColor |= ((Color >> 16) & 0xFF) << 8;
				newColor |= ((Color >> 8) & 0xFF) << 16;
				FixBrightLightColorSub(a1, newColor);
			}
		}
	}
}