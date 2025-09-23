// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSSettings.h>
#include <Patches/CKPE.Fallout4.Patch.CrashConditionItemGetCrime.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			EditorAPI::Setting** pointer_CrashConditionItemGetCrimePatch_data = nullptr;

			CrashConditionItemGetCrime::CrashConditionItemGetCrime() : Common::Patch()
			{
				SetName("Crash Condition Item GetCrime");
			}

			bool CrashConditionItemGetCrime::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashConditionItemGetCrime::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashConditionItemGetCrime::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashConditionItemGetCrime::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashConditionItemGetCrime::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool CrashConditionItemGetCrime::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Strangely, there are 6 elements in the array in memory, when Beth is forced to pass exactly 7
				// 7 element is always nullptr

				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				pointer_CrashConditionItemGetCrimePatch_data = (EditorAPI::Setting**)__CKPE_OFFSET(1);

				return true;
			}

			void CrashConditionItemGetCrime::sub(std::uintptr_t hCombobox) noexcept(true)
			{
				if (!hCombobox) return;

				Common::EditorUI::Hook::HKSendMessageA((void*)hCombobox, CB_RESETCONTENT, 0, 0);
				// Same as steal
				//EditorAPI::EditorUI::ComboBoxInsertItemDeferred(hCombobox, " ANY ", nullptr, 1);
				Common::EditorUI::ComboBoxInsertItemDeferred((void*)hCombobox, " NONE ", (void*)-1, 1);

				auto Array = pointer_CrashConditionItemGetCrimePatch_data;
				if (!Array) return;

				std::uint32_t index = 0;
				do {
					if (auto Setting = Array[index]; Setting)
						Common::EditorUI::ComboBoxInsertItemDeferred((void*)hCombobox, Setting->Name, (void*)index, 1);
				} while (++index < 7);
			}
		}
	}
}