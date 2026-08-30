// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixClassDlg.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TFixClassDlgSub = void(std::int64_t, std::int64_t);

			static std::function<TFixClassDlgSub> FixClassDlgSub;

			FixClassDlg::FixClassDlg() : Common::Patch()
			{
				SetName("Fix Class Dlg");
			}

			bool FixClassDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixClassDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixClassDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixClassDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixClassDlg::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixClassDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixClassDlg::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for the "Class" edit dialog not filling in the "Training" checkbox.
				// Also hide the unused "Recharge" option.
				//

				FixClassDlgSub = (TFixClassDlgSub*)Relocation(ID(163067)).WriteVFunc(86, &sub);
				
				return true;
			}

			void FixClassDlg::sub(std::int64_t FormClass, std::int64_t WindowHandle) noexcept(true)
			{
				// Hide the unused "Recharge" checkbox
				ShowWindow(GetDlgItem((HWND)WindowHandle, 1543), SW_HIDE);
				// If (max training level > 0) update "Training"
				CheckDlgButton((HWND)WindowHandle, 1542, 
					(*(uint8_t*)(FormClass + 0x95) > 0) ? BST_CHECKED : BST_UNCHECKED);

				FixClassDlgSub(FormClass, WindowHandle);
			}
		}
	}
}