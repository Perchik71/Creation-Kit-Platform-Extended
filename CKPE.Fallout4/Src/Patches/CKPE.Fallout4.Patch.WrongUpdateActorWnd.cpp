// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
				using namespace Common;

				// ACTOR: Wrong InvalidateRect
				Relocation(ID{ 650645, 1337975 }, Offset{ 0x131, 0x197 }).WriteFill(NOP, 6);
				Relocation(ID{ 642042, 1457407 }, 0x15A).WriteFill(NOP, 6);
				Relocation(ID{ 666248, 1989246 }, Offset{ 0x2835, 0x2C19 }).WriteFill(NOP, 0xF);

				return true;
			}
		}
	}
}