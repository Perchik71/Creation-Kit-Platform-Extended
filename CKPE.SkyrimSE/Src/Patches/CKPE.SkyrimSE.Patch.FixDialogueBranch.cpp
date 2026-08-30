// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixDialogueBranch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixDialogueBranch::FixDialogueBranch() : Common::Patch()
			{
				SetName("Fix Dialogue Branch");
			}

			bool FixDialogueBranch::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixDialogueBranch::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixDialogueBranch::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixDialogueBranch::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixDialogueBranch::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixDialogueBranch::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDialogueBranch::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for the "Dialogue Branch" dialog showing corrupted starting topic strings. 
				// The address of a variable is provided instead of a string pointer. 
				// Change LEA to MOV.
				//
				Relocation(ID(197121), 0x23C).Write({ 0x4C, 0x8B });

				return true;
			}
		}
	}
}