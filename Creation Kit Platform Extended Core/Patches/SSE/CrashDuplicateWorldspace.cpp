// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashDuplicateWorldspace.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashDuplicateWorldspace_sub = 0;

			CrashDuplicateWorldspacePatch::CrashDuplicateWorldspacePatch() : Module(GlobalEnginePtr)
			{}

			bool CrashDuplicateWorldspacePatch::HasOption() const
			{
				return false;
			}

			bool CrashDuplicateWorldspacePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashDuplicateWorldspacePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashDuplicateWorldspacePatch::GetName() const
			{
				return "Crash Duplicate Worldspace";
			}

			bool CrashDuplicateWorldspacePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// In 1.6.1130 this fixed.
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool CrashDuplicateWorldspacePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when duplicating worldspaces
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_CrashDuplicateWorldspace_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool CrashDuplicateWorldspacePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void* CrashDuplicateWorldspacePatch::sub(void* a1)
			{
				if (!a1)
					return nullptr;

				return ((void* (__fastcall*)(void*))pointer_CrashDuplicateWorldspace_sub)(a1);
			}
		}
	}
}