// Copyright © 2023-2025 aka perchik71. All rights reserved.
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
			typedef void(*TFixClassDlgSub)(std::int64_t, std::int64_t);

			static TFixClassDlgSub FixClassDlgSub;

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

			bool FixClassDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixClassDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for the "Class" edit dialog not filling in the "Training" checkbox.
				// Also hide the unused "Recharge" option.
				//
				Detours::DetourClassVTable(__CKPE_OFFSET(0), &sub, 86);
				FixClassDlgSub = (TFixClassDlgSub)(__CKPE_OFFSET(1));

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