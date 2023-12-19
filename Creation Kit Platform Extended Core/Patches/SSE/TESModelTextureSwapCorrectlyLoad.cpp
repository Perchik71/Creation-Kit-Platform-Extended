// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "TESModelTextureSwapCorrectlyLoad.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			TESModelTextureSwapCorrectlyLoadPatch::TESModelTextureSwapCorrectlyLoadPatch() : 
				Module(GlobalEnginePtr)
			{}

			bool TESModelTextureSwapCorrectlyLoadPatch::HasOption() const
			{
				return false;
			}

			bool TESModelTextureSwapCorrectlyLoadPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* TESModelTextureSwapCorrectlyLoadPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* TESModelTextureSwapCorrectlyLoadPatch::GetName() const
			{
				return "TESModelTextureSwap Correctly Load";
			}

			bool TESModelTextureSwapCorrectlyLoadPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool TESModelTextureSwapCorrectlyLoadPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix TESModelTextureSwap being incorrectly loaded (Record typo: 'MODS' -> 'MO5S')
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x4D, 0x4F, 0x35, 0x53 });

					return true;
				}

				return false;
			}

			bool TESModelTextureSwapCorrectlyLoadPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}