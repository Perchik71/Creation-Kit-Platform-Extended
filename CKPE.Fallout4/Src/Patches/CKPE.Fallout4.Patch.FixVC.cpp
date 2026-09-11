// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
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
				using namespace Common;

				Relocation(ID(580023), Offset{ 0x206, 0x315 }).WriteCall(&FixVCPatch_sub);

				// By disabling version control, allow the start				
				const auto target = ID{ 643528, 1351048 };
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					Relocation(target, 0x2BF).Write({0xEB, 0x81});					// skip msgbox
				else
					Relocation(target, 0x2BD).Write({0xEB, 0x82});					// skip msgbox
				Relocation(target, Offset{ 0x2E4, 0x2E2 }).Write({ 0xEB, 0xD9 });	// skip msgbox

				return true;
			}
		}
	}
}