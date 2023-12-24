// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashMakeXYZCircles.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixCrashMakeXYZCirclesPatch::FixCrashMakeXYZCirclesPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashMakeXYZCirclesPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashMakeXYZCirclesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashMakeXYZCirclesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashMakeXYZCirclesPatch::GetName() const
			{
				return "Fix Crash MakeXYZCircles";
			}

			bool FixCrashMakeXYZCirclesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashMakeXYZCirclesPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashMakeXYZCirclesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashMakeXYZCirclesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when MakeXYZCircles passes an invalid line count to BSShaderResourceManager::CreateLineShape. 
					// BGSPrimitiveSphere only creates these debug marker circles in the editor.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub_unknown);

					return true;
				}

				return false;
			}

			bool FixCrashMakeXYZCirclesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void* FixCrashMakeXYZCirclesPatch::sub_unknown(__int64 ResourceManager, uint32_t IndexCount, __int64 a3, __int64 a4,
				__int64 a5, __int64 a6, __int64 a7)
			{
				// Convert # of indices to # of lines (divide by two)
				return (*(void* (__fastcall**)(__int64, __int64, __int64, __int64, __int64, __int64, __int64))
					(*(__int64*)ResourceManager + 152))(ResourceManager, IndexCount / 2, a3, a4, a5, a6, a7);
			}
		}
	}
}