// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashFootstepSetRemove.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_sub19BD6E0 = 0;
			uintptr_t pointer_sub19BE490 = 0;

			CrashFootstepSetRemovePatch::CrashFootstepSetRemovePatch() : Module(GlobalEnginePtr)
			{}

			bool CrashFootstepSetRemovePatch::HasOption() const
			{
				return false;
			}

			bool CrashFootstepSetRemovePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashFootstepSetRemovePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashFootstepSetRemovePatch::GetName() const
			{
				return "Crash Footstep Set Remove";
			}

			bool CrashFootstepSetRemovePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashFootstepSetRemovePatch::GetDependencies() const
			{
				return {};
			}

			void CrashFootstepSetRemovePatch::RemoveSelectedItemsFromArray(BGSFootstepSet* Sets, HWND* hWindowList)
			{
				if (!Sets || !hWindowList || !*hWindowList)
					return;

				int32_t index = ListView_GetNextItem(*hWindowList, -1, LVNI_SELECTED);
				auto Items = Sets->CurrentArrayItem;
				Array<int32_t> indexes;

				// I get a list of indexes for remove items 

				while (index != -1)
				{
					indexes.push_back(index);
					index = ListView_GetNextItem(*hWindowList, index, LVNI_SELECTED);
				}

				// The array is always removed from end.

				for (auto it = indexes.rbegin(); it != indexes.rend(); it++)
				{
					auto Footstep = *(fastCall<BGSFootstep**>(pointer_sub19BD6E0, Items, *it));	
					fastCall<void>(pointer_sub19BE490, Sets, Sets->CurrentIndex, Footstep);
				}
			}

			bool CrashFootstepSetRemovePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashFootstepSetRemovePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// With multiple deletions, CTD occurs. 
					// Reason is the deletion of array elements from beginning, 
					// while element is actually deleted from array.
					// Deleting goes beyond the boundaries or removes necessary elements.
					// Detailed: https://github.com/Perchik71/Creation-Kit-Platform-Extended/issues/114

					auto off = _RELDATA_RAV(0);
					pointer_sub19BD6E0 = _RELDATA_ADDR(1);
					pointer_sub19BE490 = _RELDATA_ADDR(2);

					{
						ScopeRelocator text;
						// Remove erroneous code
						lpRelocator->PatchNop(off, 0x95);
						// mov rcx, qword ptr ss:[rsp+0xF0]
						// mov rdx, qword ptr ss:[rsp+0x68]
						lpRelocator->Patch(off, { 0x48, 0x8B, 0x8C, 0x24, 0xF0, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x54, 0x24, 0x68 });
					}

					lpRelocator->DetourCall((size_t)off + 0xD, (uintptr_t)&RemoveSelectedItemsFromArray);

					return true;
				}

				return false;
			}

			bool CrashFootstepSetRemovePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}