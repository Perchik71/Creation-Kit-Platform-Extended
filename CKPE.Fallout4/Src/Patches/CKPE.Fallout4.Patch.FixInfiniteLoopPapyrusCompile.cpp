// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixInfiniteLoopPapyrusCompile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixInfiniteLoopPapyrusCompile::FixInfiniteLoopPapyrusCompile() : Common::Patch()
			{
				SetName("Fixed infinite loop by Compile Papyrus Scripts");
			}

			bool FixInfiniteLoopPapyrusCompile::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixInfiniteLoopPapyrusCompile::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixInfiniteLoopPapyrusCompile::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixInfiniteLoopPapyrusCompile::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixInfiniteLoopPapyrusCompile::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixInfiniteLoopPapyrusCompile::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fixed infinite loop by Compile Papyrus Scripts...
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xC3 });

				return true;
			}
		}
	}
}