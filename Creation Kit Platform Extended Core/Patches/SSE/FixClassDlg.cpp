// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixClassDlg.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FixClassDlg_sub = 0;

			FixClassDlgPatch::FixClassDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool FixClassDlgPatch::HasOption() const
			{
				return false;
			}

			bool FixClassDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixClassDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixClassDlgPatch::GetName() const
			{
				return "Fix Class Dlg";
			}

			bool FixClassDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixClassDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Class" edit dialog not filling in the "Training" checkbox.
					// Also hide the unused "Recharge" option.
					//
					Detours::X64::DetourClassVTable(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), 
						&sub, 86);

					return true;
				}

				return false;
			}

			bool FixClassDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixClassDlgPatch::sub(TESForm* Class, HWND WindowHandle)
			{
				// Hide the unused "Recharge" checkbox
				ShowWindow(GetDlgItem(WindowHandle, 1543), SW_HIDE);
				// If (max training level > 0) update "Training"
				CheckDlgButton(WindowHandle, 1542, (*(uint8_t*)((__int64)Class + 0x95) > 0) ? BST_CHECKED : BST_UNCHECKED);

				((void(__fastcall*)(TESForm*, HWND))pointer_FixClassDlg_sub)(Class, WindowHandle);
			}
		}
	}
}