// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "MemoryLeakBSString.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			MemoryLeakBSStringPatch::MemoryLeakBSStringPatch() : Module(GlobalEnginePtr)
			{}

			bool MemoryLeakBSStringPatch::HasOption() const
			{
				return false;
			}

			bool MemoryLeakBSStringPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* MemoryLeakBSStringPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* MemoryLeakBSStringPatch::GetName() const
			{
				return "Memory Leak class BSString";
			}

			bool MemoryLeakBSStringPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> MemoryLeakBSStringPatch::GetDependencies() const
			{
				return {};
			}

			bool MemoryLeakBSStringPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryLeakBSStringPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// According to the indications of trace...
					// Bethesda does not free up memory when adding to the end of a string.
					// A stupid mistake, it seems to have been done on purpose, out of inexperience.

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&Append);

					return true;
				}

				return false;
			}

			bool MemoryLeakBSStringPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			EditorAPI::BSString* MemoryLeakBSStringPatch::Append(EditorAPI::BSString* DestStr, const char* SrcStr)
			{
				if (!SrcStr || !DestStr)
					return DestStr;

				DestStr->Append(SrcStr);
				return DestStr;
			}
		}
	}
}