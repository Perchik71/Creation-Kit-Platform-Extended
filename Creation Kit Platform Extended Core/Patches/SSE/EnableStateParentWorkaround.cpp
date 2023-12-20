// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Patches/ConsolePatch.h"
#include "EnableStateParentWorkaround.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_EnableStateParentWorkaround_sub1 = 0;
			uintptr_t pointer_EnableStateParentWorkaround_sub2 = 0;
			uintptr_t pointer_EnableStateParentWorkaround_sub3 = 0;
			uintptr_t pointer_EnableStateParentWorkaround_sub4 = 0;

			EnableStateParentWorkaroundPatch::EnableStateParentWorkaroundPatch() : Module(GlobalEnginePtr)
			{}

			bool EnableStateParentWorkaroundPatch::HasOption() const
			{
				return true;
			}

			bool EnableStateParentWorkaroundPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* EnableStateParentWorkaroundPatch::GetOptionName() const
			{
				return "CreationKit:bEnableStateParentWorkaround";
			}

			const char* EnableStateParentWorkaroundPatch::GetName() const
			{
				return "Enable State Parent Workaround";
			}

			bool EnableStateParentWorkaroundPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool EnableStateParentWorkaroundPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Workaround for "Select Enable State Parent" selecting objects outside of the current cell or worldspace
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					pointer_EnableStateParentWorkaround_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_EnableStateParentWorkaround_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_EnableStateParentWorkaround_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					pointer_EnableStateParentWorkaround_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));

					return true;
				}

				return false;
			}

			bool EnableStateParentWorkaroundPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void EnableStateParentWorkaroundPatch::sub(__int64 RenderWindowInstance, uint32_t* UntypedPointerHandle, bool Select)
			{
				// The caller of this function already holds a reference to the pointer
				__int64 parentRefr = ((__int64(__fastcall*)(__int64))pointer_EnableStateParentWorkaround_sub1)
					(*(__int64*)(RenderWindowInstance + 0xB8));

				__int64 childRefr;
				((void(__fastcall*)(__int64*, uint32_t*))pointer_EnableStateParentWorkaround_sub2)(&childRefr, UntypedPointerHandle);

				if (childRefr)
				{
					// Only select child forms if they are in the same parent cell
					if (*(__int64*)(childRefr + 0x70) == *(__int64*)(parentRefr + 0x70))
						((void(__fastcall*)(__int64, uint32_t*, bool))pointer_EnableStateParentWorkaround_sub3)(RenderWindowInstance, 
							UntypedPointerHandle, Select);
					else
						ConsolePatch::Log("Not selecting child refr (%08X) because parent cells don't match (%p != %p)\n", 
							*(uint32_t*)(childRefr + 0x14), *(__int64*)(childRefr + 0x70), *(__int64*)(parentRefr + 0x70));
				}

				((void(__fastcall*)(__int64*))pointer_EnableStateParentWorkaround_sub4)(&childRefr);
			}
		}
	}
}