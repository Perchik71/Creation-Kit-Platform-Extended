// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixLoadMore32KAnimation.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixLoadMore32KAnimationPatch::FixLoadMore32KAnimationPatch() : Module(GlobalEnginePtr)
			{}

			bool FixLoadMore32KAnimationPatch::HasOption() const
			{
				return false;
			}

			bool FixLoadMore32KAnimationPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixLoadMore32KAnimationPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixLoadMore32KAnimationPatch::GetName() const
			{
				return "Fix Load More 32K Animation";
			}

			bool FixLoadMore32KAnimationPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixLoadMore32KAnimationPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xB7 });

					return true;
				}

				return false;
			}

			bool FixLoadMore32KAnimationPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}