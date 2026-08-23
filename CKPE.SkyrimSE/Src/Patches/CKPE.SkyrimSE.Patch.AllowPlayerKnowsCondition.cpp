// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowPlayerKnowsCondition.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TAllowPlayerKnowsCondition_Add = void(std::int64_t, std::uint8_t, bool, std::int64_t, std::int64_t,
				std::int64_t, std::int64_t, std::int64_t);
			static std::function<TAllowPlayerKnowsCondition_Add> AllowPlayerKnowsCondition_Add;

			AllowPlayerKnowsCondition::AllowPlayerKnowsCondition() : Common::Patch()
			{
				SetName("Allow PlayerKnows Condition");
			}

			bool AllowPlayerKnowsCondition::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AllowPlayerKnowsCondition::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AllowPlayerKnowsCondition::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AllowPlayerKnowsCondition::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AllowPlayerKnowsCondition::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool AllowPlayerKnowsCondition::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool AllowPlayerKnowsCondition::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Allow the "PlayerKnows" conditional function to accept enchantments as a function parameter
				//
				Relocation(ID(232474)).WriteJump(&sub);
				AllowPlayerKnowsCondition_Add = Relocation<TAllowPlayerKnowsCondition_Add>(ID(242709)).Get();
					
				return true;
			}

			void AllowPlayerKnowsCondition::sub(std::int64_t ControlHandle) noexcept(true)
			{
				AllowPlayerKnowsCondition_Add(ControlHandle, 18, true, 0, 0, 0, 0, 0);		// MGEF Magic Effect
				AllowPlayerKnowsCondition_Add(ControlHandle, 21, false, 0, 0, 0, 0, 0);		// ENCH Enchantment
				AllowPlayerKnowsCondition_Add(ControlHandle, 118, false, 0, 0, 0, 0, 0);	// WOOP Word of Power
			}
		}
	}
}