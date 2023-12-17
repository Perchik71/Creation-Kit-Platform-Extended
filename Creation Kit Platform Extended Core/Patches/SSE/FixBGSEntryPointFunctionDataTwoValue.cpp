// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixBGSEntryPointFunctionDataTwoValue.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FixBGSEntryPointFunctionDataTwoValue_sub = 0;

			FixBGSEntryPointFunctionDataTwoValuePatch::FixBGSEntryPointFunctionDataTwoValuePatch() : 
				Module(GlobalEnginePtr)
			{}

			bool FixBGSEntryPointFunctionDataTwoValuePatch::HasOption() const
			{
				return false;
			}

			bool FixBGSEntryPointFunctionDataTwoValuePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixBGSEntryPointFunctionDataTwoValuePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixBGSEntryPointFunctionDataTwoValuePatch::GetName() const
			{
				return "Fix BGSEntryPointFunctionDataTwoValue";
			}

			bool FixBGSEntryPointFunctionDataTwoValuePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBGSEntryPointFunctionDataTwoValuePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for "Could not select actor value X in LoadDialog for BGSEntryPointFunctionDataTwoValue." 
					// Use the editor id instead of perk name for the Perk Entry dialog selection.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_FixBGSEntryPointFunctionDataTwoValue_sub = 
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool FixBGSEntryPointFunctionDataTwoValuePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			const char* FixBGSEntryPointFunctionDataTwoValuePatch::sub(int ActorValueIndex)
			{
				__int64 actorValue = ((__int64(__fastcall*)(int))pointer_FixBGSEntryPointFunctionDataTwoValue_sub)(ActorValueIndex);

				if (!actorValue)
					return nullptr;

				return *(const char**)(actorValue + 0x90);
			}
		}
	}
}