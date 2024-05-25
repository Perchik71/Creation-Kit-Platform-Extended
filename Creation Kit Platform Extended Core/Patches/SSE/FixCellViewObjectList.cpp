// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "..\Windows\SSE\CellViewWindow.h"
#include "FixCellViewObjectList.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::EditorAPI;

			class ScopeFilterLock
			{
			public:
				ScopeFilterLock()
				{
					GlobalCellViewWindowPtr->LockUpdateLists();
				}
				~ScopeFilterLock()
				{
					GlobalCellViewWindowPtr->UnlockUpdateLists();
				}
			};

			uintptr_t pointer_FixCellViewObjectListPatch_sub = 0;

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

			bool FixCellViewObjectListPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> FixCellViewObjectListPatch::GetDependencies() const
			{
				return { "Cell View Window" };
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
					lpRelocator->DetourJump(_RELDATA_RAV(0), (uintptr_t)&ListViewSelectItem);
					lpRelocator->DetourJump(_RELDATA_RAV(1), (uintptr_t)&ListViewFindAndSelectItem);
					lpRelocator->DetourJump(_RELDATA_RAV(2), (uintptr_t)&ListViewDeselectItem);
					
					pointer_FixCellViewObjectListPatch_sub = voltek::detours_function_class_jump(_RELDATA_ADDR(3), &sub1);

					return true;
				}

				return false;
			}

			bool FixCellViewObjectListPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixCellViewObjectListPatch::ListViewSelectItem(HWND ListViewHandle, int ItemIndex, bool KeepOtherSelections)
			{
				EditorUI::ListViewSelectItem(ListViewHandle, ItemIndex, KeepOtherSelections);
			}

			void FixCellViewObjectListPatch::ListViewFindAndSelectItem(HWND ListViewHandle, void* Parameter, bool KeepOtherSelections)
			{
				EditorUI::ListViewFindAndSelectItem(ListViewHandle, Parameter, KeepOtherSelections);
			}

			void FixCellViewObjectListPatch::ListViewDeselectItem(HWND ListViewHandle, void* Parameter)
			{
				EditorUI::ListViewDeselectItem(ListViewHandle, Parameter);
			}

			void FixCellViewObjectListPatch::sub1(uint64_t a1, uint64_t a2, uint64_t a3)
			{
				ScopeFilterLock Locker;
				fastCall<void>(pointer_FixCellViewObjectListPatch_sub, a1, a2, a3);
			}
		}
	}
}