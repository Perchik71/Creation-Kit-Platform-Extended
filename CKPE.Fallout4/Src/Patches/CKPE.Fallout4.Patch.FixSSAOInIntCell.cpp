// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TES.h>
#include <Patches/CKPE.Fallout4.Patch.FixSSAOInIntCell.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			using TFixSSAOInIntCell_sub = void(void*, std::uint32_t);

			static std::function<TFixSSAOInIntCell_sub> FixSSAOInIntCell_sub;

			FixSSAOInIntCell::FixSSAOInIntCell() : Common::Patch()
			{
				SetName("Fix SSAO In Interior Cell");
			}

			bool FixSSAOInIntCell::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSSAOInIntCell::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSSAOInIntCell::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSSAOInIntCell::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSSAOInIntCell::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixSSAOInIntCell::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Fix crash caused by toggling SSAO while in an interior cell (no worldspace)
				FixSSAOInIntCell_sub = reinterpret_cast<TFixSSAOInIntCell_sub*>(Relocation(ID{ 86141, 1941469 }).WriteJump(&sub));

				return true;
			}

			void FixSSAOInIntCell::sub(void* Unk, std::uint32_t SkyFlag) noexcept(true)
			{
				struct UnkData
				{
					char pad00[0x18];
					EditorAPI::Sky* sky;	// same TES->GetSky()
				};

				auto TES = EditorAPI::TES::Singleton.GetSingleton();
				if (!TES) return;

				if (TES->Empty())
				{
					bool Safe = EditorAPI::Sky::Setting_SkyView->GetBool();
					EditorAPI::Sky::Setting_SkyView->SetBool(false);

					TES->GetSky()->UnsetFog();
					TES->GetSky()->UnsetWeather();

					FixSSAOInIntCell_sub(Unk, 0);
					EditorAPI::Sky::Setting_SkyView->SetBool(Safe);

					return;
				}

				FixSSAOInIntCell_sub(Unk, SkyFlag);
			}
		}
	}
}