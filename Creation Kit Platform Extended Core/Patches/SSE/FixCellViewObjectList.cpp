// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCellViewObjectList.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixCellViewObjectListPatch::FixCellViewObjectListPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCellViewObjectListPatch::HasOption() const
			{
				return false;
			}

			bool FixCellViewObjectListPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCellViewObjectListPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCellViewObjectListPatch::GetName() const
			{
				return "Fix CellView Object List";
			}

			bool FixCellViewObjectListPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCellViewObjectListPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix the "Cell View" object list current selection not being synced with the render window
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&ListViewSelectItem);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&ListViewFindAndSelectItem);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&ListViewDeselectItem);

					return true;
				}

				return false;
			}

			bool FixCellViewObjectListPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool FixCellViewObjectListPatch::ListViewSetItemState(HWND ListViewHandle, WPARAM Index, UINT Data, UINT Mask)
			{
				// Microsoft's implementation of this define is broken (ListView_SetItemState)
				LVITEMA item
				{
					.mask = LVIF_STATE,
					.state = Data,
					.stateMask = Mask
				};

				return static_cast<BOOL>(SendMessageA(ListViewHandle, LVM_SETITEMSTATE, Index, reinterpret_cast<LPARAM>(&item)));
			}

			void FixCellViewObjectListPatch::ListViewSelectItem(HWND ListViewHandle, int ItemIndex, bool KeepOtherSelections)
			{
				if (!KeepOtherSelections)
					ListViewSetItemState(ListViewHandle, -1, 0, LVIS_SELECTED);

				if (ItemIndex != -1)
				{
					ListView_EnsureVisible(ListViewHandle, ItemIndex, FALSE);
					ListViewSetItemState(ListViewHandle, ItemIndex, LVIS_SELECTED, LVIS_SELECTED);
				}
			}

			void FixCellViewObjectListPatch::ListViewFindAndSelectItem(HWND ListViewHandle, void* Parameter, bool KeepOtherSelections)
			{
				if (!KeepOtherSelections)
					ListViewSetItemState(ListViewHandle, -1, 0, LVIS_SELECTED);

				LVFINDINFOA findInfo
				{
					.flags = LVFI_PARAM,
					.lParam = reinterpret_cast<LPARAM>(Parameter)
				};

				int index = ListView_FindItem(ListViewHandle, -1, &findInfo);

				if (index != -1)
					ListViewSelectItem(ListViewHandle, index, KeepOtherSelections);
			}

			void FixCellViewObjectListPatch::ListViewDeselectItem(HWND ListViewHandle, void* Parameter)
			{
				LVFINDINFOA findInfo
				{
					.flags = LVFI_PARAM,
					.lParam = reinterpret_cast<LPARAM>(Parameter)
				};

				int index = ListView_FindItem(ListViewHandle, -1, &findInfo);

				if (index != -1)
					ListViewSetItemState(ListViewHandle, index, 0, LVIS_SELECTED);
			}
		}
	}
}