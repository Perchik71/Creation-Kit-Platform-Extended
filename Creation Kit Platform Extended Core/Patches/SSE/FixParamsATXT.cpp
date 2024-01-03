// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixParamsATXT.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixParamsATXTPatch::FixParamsATXTPatch() : Module(GlobalEnginePtr)
			{}

			bool FixParamsATXTPatch::HasOption() const
			{
				return false;
			}

			bool FixParamsATXTPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixParamsATXTPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixParamsATXTPatch::GetName() const
			{
				return "Fix ATXT params sections";
			}

			bool FixParamsATXTPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixParamsATXTPatch::GetDependencies() const
			{
				return {};
			}

			bool FixParamsATXTPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixParamsATXTPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for TESObjectLAND when writing an ATXT section, a memory multiple of 8 bytes is allocated, 
					// where 2 bytes are the index, 2 bytes are garbage, 4 bytes are transparency 0.0 to 1.0. 
					// In SSEEdit, the second value is shown as unknown, but in fact it is not pre-zeroed memory.
					// let's write the index as 32-bit, thereby zeroing out the memory.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x89, 0x8C, 0xC4, 0x00, 0x05, 0x00, 0x00, 0x90 });

					return true;
				}

				return false;
			}

			bool FixParamsATXTPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}