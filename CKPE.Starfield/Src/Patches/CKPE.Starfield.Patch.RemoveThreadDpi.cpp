// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.RemoveThreadDpi.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			RemoveThreadDpi::RemoveThreadDpi() : Common::Patch()
			{
				SetName("Remove Thread Dpi");
			}

			bool RemoveThreadDpi::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RemoveThreadDpi::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RemoveThreadDpi::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RemoveThreadDpi::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RemoveThreadDpi::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool RemoveThreadDpi::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				for (std::uint32_t i = 0; i < db->GetCount(); i++)
					SafeWrite::Write(__CKPE_OFFSET(i), { 0xEB });

				return true;
			}
		}
	}
}