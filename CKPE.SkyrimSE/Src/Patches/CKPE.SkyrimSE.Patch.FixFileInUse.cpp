// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixFileInUse.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixFileInUse::FixFileInUse() : Common::Patch()
			{
				SetName("File in use");
			}

			bool FixFileInUse::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixFileInUse::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixFileInUse::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixFileInUse::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixFileInUse::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixFileInUse::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for "File in use" UI hang after hitting cancel. 
				// It tries to use the main window handle as a parent, but it's suspended during the initial data load, 
				// resulting in a deadlock. The new BGSThreadedProgressDlg causes this.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x4D, 0x33, 0xC0, 0x90, 0x90, 0x90, 0x90 });

				return true;
			}
		}
	}
}