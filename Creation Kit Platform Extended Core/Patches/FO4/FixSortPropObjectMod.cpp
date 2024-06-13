// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "FixSortPropObjectMod.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			static HWND hPropObjectModWnd = nullptr;

			FixSortPropObjectModPatch::FixSortPropObjectModPatch() : Module(GlobalEnginePtr)
			{}

			bool FixSortPropObjectModPatch::HasOption() const
			{
				return false;
			}

			bool FixSortPropObjectModPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSortPropObjectModPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSortPropObjectModPatch::GetName() const
			{
				return "Fix Sort Prop Object Mod";
			}

			bool FixSortPropObjectModPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixSortPropObjectModPatch::GetDependencies() const
			{
				return {};
			}

			bool FixSortPropObjectModPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool FixSortPropObjectModPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Removing stuff that messes up the index selected item.
					uintptr_t Rva1 = _RELDATA_RAV(0);
					lpRelocator->PatchNop(Rva1, 0xF);

					// Flickering is visible as the list is being re-elected and rebuilt. Let's turn on redrawing tracking.
					uintptr_t Rva2 = _RELDATA_RAV(1);
					lpRelocator->Patch(Rva2, { 0x4C, 0x89, 0xF9 });
					lpRelocator->DetourCall(Rva2 + 3, (uintptr_t)&sub1);
					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&sub2);

					return true;
				}

				return false;
			}

			bool FixSortPropObjectModPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixSortPropObjectModPatch::sub1(HWND hWnd)
			{
				hPropObjectModWnd = hWnd;
				EditorAPI::EditorUI::SuspendComboBoxUpdates(hPropObjectModWnd, true);
			}

			void FixSortPropObjectModPatch::sub2(HWND hWnd, const RECT* lpRect, BOOL bErase)
			{
				if (hPropObjectModWnd)
				{
					EditorAPI::EditorUI::SuspendComboBoxUpdates(hPropObjectModWnd, false);
					hPropObjectModWnd = nullptr;
				}

				InvalidateRect(hWnd, lpRect, bErase);
			}
		}
	}
}