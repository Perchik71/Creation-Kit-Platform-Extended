// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.PapyrusEditorLimit.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			PapyrusEditorLimit::PapyrusEditorLimit() : Common::Patch()
			{
				SetName("Papyrus Editor Limit");
			}

			bool PapyrusEditorLimit::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* PapyrusEditorLimit::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool PapyrusEditorLimit::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> PapyrusEditorLimit::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool PapyrusEditorLimit::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool PapyrusEditorLimit::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				

				if (db) {
					if (db->GetVersion() != 1)
						return false;

					//
					// Raise the papyrus script editor text limit to 500k characters from 64k
					//
					Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

					return true;
				}
				else
				{
					using namespace Common;

					Relocation(ID{ 1342298 }, Offset{ 0x12C }).WriteCall(sub);

					return true;
					
				}
			}

			bool PapyrusEditorLimit::sub(std::int64_t RichEditControl, const char* Text) noexcept(true)
			{
				SendMessageA((HWND)RichEditControl, EM_LIMITTEXT, 500000, 0);
				return SetWindowTextA((HWND)RichEditControl, Text);
			}
		}
	}
}