// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixVC.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static void FixVCPatch_sub(HWND hWnd, const char* lpText, const char* lpCaption, std::uint32_t uType)
			{
				if (lpText)
					_CONSOLE("MESSAGE: %s", lpText);
			}

			FixVC::FixVC() : Common::Patch()
			{
				SetName("Version Control fixes");
			}

			bool FixVC::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixVC::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixVC::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixVC::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixVC::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixVC::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&FixVCPatch_sub);

				// By disabling version control, allow the start

				if (verPatch == 1)
					SafeWrite::Write(__CKPE_OFFSET(1), { 0xEB, 0x81 });	// skip msgbox 
				else
					SafeWrite::Write(__CKPE_OFFSET(1), { 0xEB, 0x82 });	// skip msgbox 

				SafeWrite::Write(__CKPE_OFFSET(2), { 0xEB, 0xD9 });	// skip msgbox

				return true;
			}
		}
	}
}