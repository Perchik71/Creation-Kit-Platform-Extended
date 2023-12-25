// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "StableSortForPerks.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			StableSortForPerksPatch::StableSortForPerksPatch() : Module(GlobalEnginePtr)
			{}

			bool StableSortForPerksPatch::HasOption() const
			{
				return false;
			}

			bool StableSortForPerksPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* StableSortForPerksPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* StableSortForPerksPatch::GetName() const
			{
				return "Stable Sort For Perks";
			}

			bool StableSortForPerksPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> StableSortForPerksPatch::GetDependencies() const
			{
				return {};
			}

			bool StableSortForPerksPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool StableSortForPerksPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0),
						(uintptr_t)&ArrayQuickSortRecursive<class BGSEntryPointPerkEntry*, true>);	// Stable sort for perk entry window

					return true;
				}

				return false;
			}

			bool StableSortForPerksPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}