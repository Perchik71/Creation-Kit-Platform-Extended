// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixMemoryLeakActorDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixMemoryLeakActorDlg::FixMemoryLeakActorDlg() : Common::Patch()
			{
				SetName("Fix Memory Leak ActorDlg");
			}

			bool FixMemoryLeakActorDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixMemoryLeakActorDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixMemoryLeakActorDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixMemoryLeakActorDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixMemoryLeakActorDlg::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixMemoryLeakActorDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMemoryLeakActorDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for a memory leak in BSShadowLight::ClearShadowMapData after opening "Actor" dialogs (~500kb per instance). 
				// The code loops over a ShadowMapData array and checks if ShadowMapIndex is NOT -1, freeing the data if true. 
				// When opening a dialog this is always -1 and it never gets deallocated. Hacky fix: remove the check.
				//
				Relocation(ID{ 425693, 921622 }, Offset{ 0x89, 0x8C }).WriteFill(NOP, 6);

				return true;
			}
		}
	}
}