// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.RunAppWithoutNet.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			RunAppWithoutNet::RunAppWithoutNet() : Common::Patch()
			{
				SetName("Run app without net");
			}

			bool RunAppWithoutNet::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RunAppWithoutNet::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RunAppWithoutNet::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RunAppWithoutNet::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RunAppWithoutNet::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_78_0;
			}

			bool RunAppWithoutNet::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
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