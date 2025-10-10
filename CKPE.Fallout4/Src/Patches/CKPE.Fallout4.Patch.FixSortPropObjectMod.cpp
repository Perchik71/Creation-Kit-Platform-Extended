// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixSortPropObjectMod.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static HWND hPropObjectModWnd = nullptr;

			static void FixSortPropObjectMod_sub1(HWND hWnd) noexcept(true)
			{
				hPropObjectModWnd = hWnd;
				Common::EditorUI::SuspendComboBoxUpdates(hPropObjectModWnd, true);
			}

			static void FixSortPropObjectMod_sub2(HWND hWnd, const RECT* lpRect, BOOL bErase) noexcept(true)
			{
				if (hPropObjectModWnd)
				{
					Common::EditorUI::SuspendComboBoxUpdates(hPropObjectModWnd, false);
					hPropObjectModWnd = nullptr;
				}

				InvalidateRect(hWnd, lpRect, bErase);
			}

			FixSortPropObjectMod::FixSortPropObjectMod() : Common::Patch()
			{
				SetName("Fix Sort Prop Object Mod");
			}

			bool FixSortPropObjectMod::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSortPropObjectMod::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSortPropObjectMod::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSortPropObjectMod::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSortPropObjectMod::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixSortPropObjectMod::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Removing stuff that messes up the index selected item.
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 0xF);

				// Flickering is visible as the list is being re-elected and rebuilt. Let's turn on redrawing tracking.
				auto Rva = __CKPE_OFFSET(1);
				SafeWrite::Write(Rva, { 0x4C, 0x89, 0xF9 });
				Detours::DetourCall(Rva + 3, (std::uintptr_t)&FixSortPropObjectMod_sub1);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&FixSortPropObjectMod_sub2);

				return true;
			}
		}
	}
}