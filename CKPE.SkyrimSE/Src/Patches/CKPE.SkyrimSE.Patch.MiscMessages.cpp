﻿// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.MiscMessages.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			MiscMessages::MiscMessages() : Common::Patch()
			{
				SetName("Misc Messages");
			}

			bool MiscMessages::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* MiscMessages::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool MiscMessages::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> MiscMessages::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool MiscMessages::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MiscMessages::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::Write(__CKPE_OFFSET(0), { 0x90, 0xE9 });

				return true;
			}
		}
	}
}