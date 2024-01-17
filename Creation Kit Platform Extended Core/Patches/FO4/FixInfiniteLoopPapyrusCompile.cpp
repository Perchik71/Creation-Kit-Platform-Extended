// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixInfiniteLoopPapyrusCompile.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixInfiniteLoopPapyrusCompilePatch::FixInfiniteLoopPapyrusCompilePatch() : Module(GlobalEnginePtr)
			{}

			bool FixInfiniteLoopPapyrusCompilePatch::HasOption() const
			{
				return false;
			}

			bool FixInfiniteLoopPapyrusCompilePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixInfiniteLoopPapyrusCompilePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixInfiniteLoopPapyrusCompilePatch::GetName() const
			{
				return "Fixed infinite loop by Compile Papyrus Scripts";
			}

			bool FixInfiniteLoopPapyrusCompilePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixInfiniteLoopPapyrusCompilePatch::GetDependencies() const
			{
				return {};
			}

			bool FixInfiniteLoopPapyrusCompilePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixInfiniteLoopPapyrusCompilePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fixed infinite loop by Compile Papyrus Scripts...
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xC3 });

					return true;
				}

				return false;
			}

			bool FixInfiniteLoopPapyrusCompilePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}