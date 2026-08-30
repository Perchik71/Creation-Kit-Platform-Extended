// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.ShowReloadShadersAlways.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			ShowReloadShadersAlways::ShowReloadShadersAlways() : Common::Patch()
			{
				SetName("Show Reload Shaders Always");
			}

			bool ShowReloadShadersAlways::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ShowReloadShadersAlways::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ShowReloadShadersAlways::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ShowReloadShadersAlways::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ShowReloadShadersAlways::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool ShowReloadShadersAlways::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ShowReloadShadersAlways::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Force bShowReloadShadersButton to always be enabled
				Relocation(ID(554127), Offset{ 0xAC3, 0xAC0 }).WriteFill(NOP, 2);

				return true;
			}
		}
	}
}