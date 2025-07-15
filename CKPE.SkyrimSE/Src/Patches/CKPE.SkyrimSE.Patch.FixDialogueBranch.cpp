// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
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

			bool FixDialogueBranch::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDialogueBranch::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for the "Dialogue Branch" dialog showing corrupted starting topic strings. 
				// The address of a variable is provided instead of a string pointer. 
				// Change LEA to MOV.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x4C, 0x8B });

				return true;
			}
		}
	}
}