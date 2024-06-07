// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "IncreaseChunkSizeForSNAMPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			constexpr const uint32_t uiMaxChunkSizeSNAM = 0x800;

			IncreaseChunkSizeForSNAMPatch::IncreaseChunkSizeForSNAMPatch() : Module(GlobalEnginePtr)
			{}

			bool IncreaseChunkSizeForSNAMPatch::HasOption() const
			{
				return false;
			}

			bool IncreaseChunkSizeForSNAMPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* IncreaseChunkSizeForSNAMPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* IncreaseChunkSizeForSNAMPatch::GetName() const
			{
				return "Increase chunk size for SNAM";
			}

			bool IncreaseChunkSizeForSNAMPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> IncreaseChunkSizeForSNAMPatch::GetDependencies() const
			{
				return {};
			}

			bool IncreaseChunkSizeForSNAMPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool IncreaseChunkSizeForSNAMPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Increasing the size for the SNAM chunk from 512 to 2048
					lpRelocator->Patch(_RELDATA_RAV(0), (uint8_t*)&uiMaxChunkSizeSNAM, 4);

					return true;
				}

				return false;
			}

			bool IncreaseChunkSizeForSNAMPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}