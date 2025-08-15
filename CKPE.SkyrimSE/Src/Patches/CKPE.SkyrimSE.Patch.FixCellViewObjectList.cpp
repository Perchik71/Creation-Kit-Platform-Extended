// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CellViewWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCellViewObjectList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class ScopeFilterLock
			{
			public:
				ScopeFilterLock() noexcept(true)
				{
					CellViewWindow::Singleton->LockUpdateLists();
				}
				~ScopeFilterLock() noexcept(true)
				{
					CellViewWindow::Singleton->UnlockUpdateLists();
				}
			};

			std::uintptr_t pointer_FixCellViewObjectListPatch_sub = 0;

			FixCellViewObjectList::FixCellViewObjectList() : Common::Patch()
			{
				SetName("Fix CellView Object List");
			}

			bool FixCellViewObjectList::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCellViewObjectList::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCellViewObjectList::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> FixCellViewObjectList::GetDependencies() const noexcept(true)
			{
				return { "Cell View Window" };
			}

			bool FixCellViewObjectList::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCellViewObjectList::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix the "Cell View" object list current selection not being synced with the render window
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&ListViewSelectItem);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&ListViewFindAndSelectItem);
				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&ListViewDeselectItem);

				pointer_FixCellViewObjectListPatch_sub = Detours::DetourClassJump(__CKPE_OFFSET(3), &sub1);

				return true;
			}

			void FixCellViewObjectList::ListViewSelectItem(void* ListViewHandle, std::int32_t ItemIndex,
				bool KeepOtherSelections) noexcept(true)
			{
				Common::EditorUI::ListViewSelectItem(ListViewHandle, ItemIndex, KeepOtherSelections);
			}

			void FixCellViewObjectList::ListViewFindAndSelectItem(void* ListViewHandle, void* Parameter, 
				bool KeepOtherSelections) noexcept(true)
			{
				Common::EditorUI::ListViewFindAndSelectItem(ListViewHandle, Parameter, KeepOtherSelections);
			}

			void FixCellViewObjectList::ListViewDeselectItem(void* ListViewHandle, void* Parameter) noexcept(true)
			{
				Common::EditorUI::ListViewDeselectItem(ListViewHandle, Parameter);
			}

			void FixCellViewObjectList::sub1(std::uint64_t a1, std::uint64_t a2, std::uint64_t a3) noexcept(true)
			{
				ScopeFilterLock Locker;
				fast_call<void>(pointer_FixCellViewObjectListPatch_sub, a1, a2, a3);
			}
		}
	}
}