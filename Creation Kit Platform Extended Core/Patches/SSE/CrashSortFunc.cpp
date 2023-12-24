// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashSortFunc.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashSortFunc_sub = 0;

			CrashSortFuncPatch::CrashSortFuncPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashSortFuncPatch::HasOption() const
			{
				return false;
			}

			bool CrashSortFuncPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashSortFuncPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashSortFuncPatch::GetName() const
			{
				return "Crash Sort Function";
			}

			bool CrashSortFuncPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashSortFuncPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashSortFuncPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashSortFuncPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash (recursive sorting function stack overflow) when saving certain ESP files (i.e 3DNPC.esp)
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub<TESForm*>);

					return true;
				}

				return false;
			}

			bool CrashSortFuncPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}