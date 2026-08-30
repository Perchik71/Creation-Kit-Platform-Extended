// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TFixBrightLightColorSub = void(std::int64_t a1, std::uint32_t);

			static std::function<TFixBrightLightColorSub> FixBrightLightColorSub;

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

			bool FixBrightLightColor::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixBrightLightColor::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBrightLightColor::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for the "Bright Light Color" option having incorrect colors in the preferences window. 
				// The blue and green channels are swapped.
				//

				FixBrightLightColorSub = reinterpret_cast<TFixBrightLightColorSub*>
					(Relocation(ID(554127), Offset{ 0xAA8, 0xAA5 }).WriteCall(&sub));

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