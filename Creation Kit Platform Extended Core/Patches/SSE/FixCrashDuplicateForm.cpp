// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashDuplicateForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixCrashDuplicateFormPatch::FixCrashDuplicateFormPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashDuplicateFormPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashDuplicateFormPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashDuplicateFormPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashDuplicateFormPatch::GetName() const
			{
				return "Fix Crash Duplicate Form";
			}

			bool FixCrashDuplicateFormPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashDuplicateFormPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashDuplicateFormPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashDuplicateFormPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when duplicating a form with an empty editor id. 
					// Integer underflow when string length is 0. TESForm::MakeUniqueEditorID.
					//
					uintptr_t addr = lpRelocationDatabaseItem->At(0);
					lpRelocator->DetourCall(addr, (uintptr_t)&sub);
					lpRelocator->PatchNop(addr + 5, 1);

					return true;
				}

				return false;
			}

			bool FixCrashDuplicateFormPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			const char* FixCrashDuplicateFormPatch::sub(__int64 a1)
			{
				const char* formEDID = (*(const char*(__fastcall**)(__int64))(*(__int64*)a1 + 0x1E8))(a1);

				if (!formEDID || strlen(formEDID) <= 0)
					return "UNNAMED_FORM";

				return formEDID;
			}
		}
	}
}