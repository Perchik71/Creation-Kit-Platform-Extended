// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.LiteralFormat.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/T/TESForm.h>
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

			bool CrashInvalidStrings::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashInvalidStrings::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashInvalidStrings::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				pointer_CrashInvalidStrings_sub1 = Relocation(ID(192607)).WriteJump(&GetLocalizeStringSafe);

				// Editor IDs passed as the sprintf_s format; sites verified on 1.7.99 only
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_SKYRIM_SE_1_7_99_0)
					LiteralFormat::Redirect({
						Relocation(ID(173333), 0x128E).Address(), Relocation(ID(173333), 0x338D).Address(),
						Relocation(ID(201988), 0xDC).Address(), Relocation(ID(201988), 0x143).Address(),
						Relocation(ID(253023), 0x804).Address(),
						Relocation(ID(552350), 0x362B).Address(), Relocation(ID(552350), 0x3E50).Address(),
						Relocation(ID(552353), 0xE7C).Address(),
						Relocation(ID(417849), 0x249).Address(), Relocation(ID(417849), 0x37E).Address() },
						Relocation(ID(618376)).Address());

				return true;
			}
		}
	}
}