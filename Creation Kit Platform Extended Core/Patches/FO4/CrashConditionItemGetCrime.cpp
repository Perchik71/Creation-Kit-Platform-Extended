// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "CrashConditionItemGetCrime.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			class GameSettingCollection
			{
			public:
				// format: <name>\x0<value>
				char* Data;
			};

			template<typename T>
			class SettingT
			{
			public:
				virtual ~SettingT() = default;

				char* Name;
				GameSettingCollection Option;
			};

			SettingT<GameSettingCollection>** pointer_CrashConditionItemGetCrimePatch_data = nullptr;

			CrashConditionItemGetCrimePatch::CrashConditionItemGetCrimePatch() : Module(GlobalEnginePtr)
			{}

			bool CrashConditionItemGetCrimePatch::HasOption() const
			{
				return false;
			}

			bool CrashConditionItemGetCrimePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashConditionItemGetCrimePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashConditionItemGetCrimePatch::GetName() const
			{
				return "Crash Condition Item GetCrime";
			}

			bool CrashConditionItemGetCrimePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashConditionItemGetCrimePatch::GetDependencies() const
			{
				return {};
			}

			bool CrashConditionItemGetCrimePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST) &&
					(eEditorCurrentVersion != EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_943_1);
			}

			bool CrashConditionItemGetCrimePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Strangely, there are 6 elements in the array in memory, when Beth is forced to pass exactly 7
					// 7 element is always nullptr

					lpRelocator->DetourJump(_RELDATA_RAV(0), (uintptr_t)&sub);
					pointer_CrashConditionItemGetCrimePatch_data = (SettingT<GameSettingCollection>**)_RELDATA_ADDR(1);

					return true;
				}

				return false;
			}

			bool CrashConditionItemGetCrimePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CrashConditionItemGetCrimePatch::sub(HWND hCombobox)
			{
				if (!hCombobox) return;

				EditorAPI::EditorUI::HKSendMessageA(hCombobox, CB_RESETCONTENT, 0, 0);
				// Same as steal
				//EditorAPI::EditorUI::ComboBoxInsertItemDeferred(hCombobox, " ANY ", nullptr, 1);
				EditorAPI::EditorUI::ComboBoxInsertItemDeferred(hCombobox, " NONE ", (void*)-1, 1);
				
				auto Array = pointer_CrashConditionItemGetCrimePatch_data;
				if (!Array) return;

				uint32_t index = 0;
				do {
					if (auto Setting = Array[index]; Setting) 
						EditorAPI::EditorUI::ComboBoxInsertItemDeferred(hCombobox, Setting->Name, (void*)index, 1);
				} while (++index < 7);
			}
		}
	}
}