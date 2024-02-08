// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashGenMaxHeightData.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashGenMaxHeightData_sub1 = 0;
			uintptr_t pointer_CrashGenMaxHeightData_sub2 = 0;

			CrashGenMaxHeightDataPatch::CrashGenMaxHeightDataPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashGenMaxHeightDataPatch::HasOption() const
			{
				return false;
			}

			bool CrashGenMaxHeightDataPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashGenMaxHeightDataPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashGenMaxHeightDataPatch::GetName() const
			{
				return "Crash Generate Max Height Data";
			}

			bool CrashGenMaxHeightDataPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashGenMaxHeightDataPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashGenMaxHeightDataPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return false; //eEditorCurrentVersion == EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool CrashGenMaxHeightDataPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// FaceGenMorph ....

					//
					// There is no check for an empty memory pointer, which leads to CTD
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_CrashGenMaxHeightData_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_CrashGenMaxHeightData_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

					return true;
				}

				return false;
			}

			bool CrashGenMaxHeightDataPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			uint32_t CrashGenMaxHeightDataPatch::sub(uint64_t a1, uint64_t a2, uint64_t a3)
			{
				auto Ret = fastCall<uint32_t>(pointer_CrashGenMaxHeightData_sub1, a1, a2, a3);			
				if (!Ret)
				{
					// Skip the entire code
					if (!fastCall<uint64_t>(pointer_CrashGenMaxHeightData_sub2, a2)) 
						return 1;
				}

				return Ret;
			}
		}
	}
}