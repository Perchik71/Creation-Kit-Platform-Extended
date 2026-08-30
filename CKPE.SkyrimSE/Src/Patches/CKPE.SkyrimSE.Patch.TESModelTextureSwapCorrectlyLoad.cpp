// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.TESModelTextureSwapCorrectlyLoad.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			TESModelTextureSwapCorrectlyLoad::TESModelTextureSwapCorrectlyLoad() : Common::Patch()
			{
				SetName("TESModelTextureSwap Correctly Load");
			}

			bool TESModelTextureSwapCorrectlyLoad::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESModelTextureSwapCorrectlyLoad::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESModelTextureSwapCorrectlyLoad::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESModelTextureSwapCorrectlyLoad::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESModelTextureSwapCorrectlyLoad::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool TESModelTextureSwapCorrectlyLoad::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool TESModelTextureSwapCorrectlyLoad::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix TESModelTextureSwap being incorrectly loaded (Record typo: 'MODS' -> 'MO5S')
				//
				Relocation(ID(143362), 0x89).Write({ 0x4D, 0x4F, 0x35, 0x53 });

				return true;
			}
		}
	}
}