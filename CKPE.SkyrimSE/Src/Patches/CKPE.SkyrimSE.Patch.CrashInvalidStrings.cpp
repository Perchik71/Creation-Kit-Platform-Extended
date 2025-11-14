// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashInvalidStrings.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_CrashInvalidStrings_sub1 = 0;

			const char* CrashInvalidStrings::GetLocalizeStringSafe(void* This)
			{
				__try
				{
					return fast_call<const char*>(pointer_CrashInvalidStrings_sub1, This);
				}
				__except (1)
				{
					Console::LogWarning(Console::FORMS, "GetLocalizeString return failed");

					return nullptr;
				}
			}

			CrashInvalidStrings::CrashInvalidStrings() : Common::Patch()
			{
				SetName("Crash Invalid Strings");
			}

			bool CrashInvalidStrings::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashInvalidStrings::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashInvalidStrings::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> CrashInvalidStrings::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool CrashInvalidStrings::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashInvalidStrings::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				pointer_CrashInvalidStrings_sub1 = Detours::DetourClassJump(__CKPE_OFFSET(1), &GetLocalizeStringSafe);

				return true;
			}
		}
	}
}