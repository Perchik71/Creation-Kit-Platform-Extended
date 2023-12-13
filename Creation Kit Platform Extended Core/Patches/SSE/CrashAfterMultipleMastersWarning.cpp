// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashAfterMultipleMastersWarning.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashAfterMultipleMastersWarning_sub = 0;

			CrashAfterMultipleMastersWarningPatch::CrashAfterMultipleMastersWarningPatch() : 
				Module(GlobalEnginePtr)
			{}

			bool CrashAfterMultipleMastersWarningPatch::HasOption() const
			{
				return false;
			}

			bool CrashAfterMultipleMastersWarningPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashAfterMultipleMastersWarningPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashAfterMultipleMastersWarningPatch::GetName() const
			{
				return "Crash after Multiple Masters Warning";
			}

			bool CrashAfterMultipleMastersWarningPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashAfterMultipleMastersWarningPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash after the "Multiple masters selected for load" dialog is shown.
					// Missing null pointer check in Sky::UpdateAurora.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_CrashAfterMultipleMastersWarning_sub =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool CrashAfterMultipleMastersWarningPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CrashAfterMultipleMastersWarningPatch::sub(__int64 a1)
			{
				if (*(__int64*)(a1 + 0x58))
					((void(__fastcall*)(__int64))pointer_CrashAfterMultipleMastersWarning_sub)(a1);
			}
		}
	}
}