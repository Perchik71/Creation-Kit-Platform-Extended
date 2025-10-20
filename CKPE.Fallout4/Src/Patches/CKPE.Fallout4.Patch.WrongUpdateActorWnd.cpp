// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.WrongUpdateActorWnd.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			WrongUpdateActorWnd::WrongUpdateActorWnd() : Common::Patch()
			{
				SetName("Wrong Update Actor Window");
			}

			bool WrongUpdateActorWnd::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* WrongUpdateActorWnd::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool WrongUpdateActorWnd::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> WrongUpdateActorWnd::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool WrongUpdateActorWnd::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool WrongUpdateActorWnd::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// ACTOR: Wrong InvalidateRect
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 6);
				SafeWrite::WriteNop(__CKPE_OFFSET(1), 6);
				SafeWrite::WriteNop(__CKPE_OFFSET(2), 0xF);
				//SafeWrite::WriteNop(__CKPE_OFFSET(3), 0x1D);

				return true;
			}
		}
	}
}