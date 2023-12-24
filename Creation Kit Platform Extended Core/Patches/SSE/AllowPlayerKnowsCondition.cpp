// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "AllowPlayerKnowsCondition.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_AllowPlayerKnowsCondition_sub = 0;

			AllowPlayerKnowsConditionPatch::AllowPlayerKnowsConditionPatch() : Module(GlobalEnginePtr)
			{}

			bool AllowPlayerKnowsConditionPatch::HasOption() const
			{
				return false;
			}

			bool AllowPlayerKnowsConditionPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AllowPlayerKnowsConditionPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* AllowPlayerKnowsConditionPatch::GetName() const
			{
				return "Allow PlayerKnows Condition";
			}

			bool AllowPlayerKnowsConditionPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> AllowPlayerKnowsConditionPatch::GetDependencies() const
			{
				return {};
			}

			bool AllowPlayerKnowsConditionPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool AllowPlayerKnowsConditionPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Allow the "PlayerKnows" conditional function to accept enchantments as a function parameter
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_AllowPlayerKnowsCondition_sub = lpRelocationDatabaseItem->At(1);

					return true;
				}

				return false;
			}

			bool AllowPlayerKnowsConditionPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void AllowPlayerKnowsConditionPatch::sub(HWND ControlHandle)
			{
				auto sub = (void(*)(HWND, uint8_t, bool, __int64, __int64, __int64, __int64, __int64))
					pointer_AllowPlayerKnowsCondition_sub;

				sub(ControlHandle, 18, true, 0, 0, 0, 0, 0);	// MGEF Magic Effect
				sub(ControlHandle, 21, false, 0, 0, 0, 0, 0);	// ENCH Enchantment
				sub(ControlHandle, 118, false, 0, 0, 0, 0, 0);	// WOOP Word of Power
			}
		}
	}
}