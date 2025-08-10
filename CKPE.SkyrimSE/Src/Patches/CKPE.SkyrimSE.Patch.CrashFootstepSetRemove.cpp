// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/BGSFootstepSet.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashFootstepSetRemove.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			uintptr_t pointer_sub19BD6E0 = 0;
			uintptr_t pointer_sub19BE490 = 0;

			CrashFootstepSetRemove::CrashFootstepSetRemove() : Common::Patch()
			{
				SetName("Crash Footstep Set Remove");
			}

			bool CrashFootstepSetRemove::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashFootstepSetRemove::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashFootstepSetRemove::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashFootstepSetRemove::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashFootstepSetRemove::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashFootstepSetRemove::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// With multiple deletions, CTD occurs. 
				// Reason is the deletion of array elements from beginning, 
				// while element is actually deleted from array.
				// Deleting goes beyond the boundaries or removes necessary elements.
				// Detailed: https://github.com/Perchik71/Creation-Kit-Platform-Extended/issues/114

				auto off = __CKPE_OFFSET(0);
				pointer_sub19BD6E0 = __CKPE_OFFSET(1);
				pointer_sub19BE490 = __CKPE_OFFSET(2);

				// Remove erroneous code
				SafeWrite::WriteNop(off, 0x95);
				// mov rcx, qword ptr ss:[rsp+0xF0]
				// mov rdx, qword ptr ss:[rsp+0x68]
				SafeWrite::Write(off, { 0x48, 0x8B, 0x8C, 0x24, 0xF0, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x54, 0x24, 0x68 });
				Detours::DetourCall((std::size_t)off + 0xD, (std::uintptr_t)&RemoveSelectedItemsFromArray);

				return true;
			}

			void CrashFootstepSetRemove::RemoveSelectedItemsFromArray(void* Sets, void** hWindowList) noexcept(true)
			{
				if (!Sets || !hWindowList || !*hWindowList)
					return;

				auto hwnd = (HWND)(*hWindowList);
				auto sets = ((EditorAPI::Forms::BGSFootstepSet*)Sets);
				
				auto Items = sets->CurrentArrayItem;
				if (!Items)
					return;

				std::int32_t index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);
				std::vector<std::int32_t> indexes;

				// I get a list of indexes for remove items 

				while (index != -1)
				{
					indexes.push_back(index);
					index = ListView_GetNextItem(hwnd, index, LVNI_SELECTED);
				}

				// The array is always removed from end.

				for (auto it = indexes.rbegin(); it != indexes.rend(); it++)
				{
					auto reference_form = fast_call<EditorAPI::Forms::BGSFootstep**>(pointer_sub19BD6E0, Items, *it);
					if (!reference_form)
					{
						auto editorID = sets->EditorID;
						Console::LogWarning(Console::FORMS,
							"BGSFootstepSet \"%s\" (0x%08X) does not contain BGSFootstep behind the %d index. Aborting operation.", 
							editorID ? editorID : "", sets->FormID, *it);
						// Stopped
						break;
					}

					fast_call<void>(pointer_sub19BE490, Sets,
						((EditorAPI::Forms::BGSFootstepSet*)Sets)->CurrentIndex, *reference_form);
				}
			}
		}
	}
}