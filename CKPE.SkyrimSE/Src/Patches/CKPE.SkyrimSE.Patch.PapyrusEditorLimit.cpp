// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.PapyrusEditorLimit.h>

namespace CKPE
{
	namespace SkyrimSE
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool PapyrusEditorLimit::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Raise the papyrus script editor text limit to 500k characters from 64k
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			bool PapyrusEditorLimit::sub(std::int64_t RichEditControl, const char* Text) noexcept(true)
			{
				SendMessageA((HWND)RichEditControl, EM_LIMITTEXT, 500000, 0);
				return SetWindowTextA((HWND)RichEditControl, Text);
			}
		}
	}
}