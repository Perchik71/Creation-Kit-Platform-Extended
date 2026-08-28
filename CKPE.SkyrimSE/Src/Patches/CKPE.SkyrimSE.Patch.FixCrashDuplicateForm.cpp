// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashDuplicateForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixCrashDuplicateForm::FixCrashDuplicateForm() : Common::Patch()
			{
				SetName("Fix Crash Duplicate Form");
			}

			bool FixCrashDuplicateForm::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashDuplicateForm::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashDuplicateForm::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashDuplicateForm::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashDuplicateForm::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixCrashDuplicateForm::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashDuplicateForm::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when duplicating a form with an empty editor id. 
				// Integer underflow when string length is 0. TESForm::MakeUniqueEditorID.
				//
				auto addr = Relocation(ID(241315), 0x2E).Address();
				Detours::DetourCall(addr, (std::uintptr_t)&sub);
				SafeWrite::WriteNop(addr + 5, 1);

				return true;
			}

			const char* FixCrashDuplicateForm::sub(std::int64_t a1) noexcept(true)
			{
				const char* formEDID = (*(const char*(__fastcall**)(__int64))(*(__int64*)a1 + 0x1E8))(a1);

				if (!formEDID || strlen(formEDID) <= 0)
					return "UNNAMED_FORM";

				return formEDID;
			}
		}
	}
}