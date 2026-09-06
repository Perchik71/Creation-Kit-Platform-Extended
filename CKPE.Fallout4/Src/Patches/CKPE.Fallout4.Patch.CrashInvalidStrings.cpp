// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.CrashInvalidStrings.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			using TCrashInvalidStringsLength	= std::uint32_t (void*);
			using TCrashInvalidStringsGet		= const char* (void*);

			static std::function<TCrashInvalidStringsLength> CrashInvalidStringsLength;
			static std::function<TCrashInvalidStringsGet>  CrashInvalidStringsGet;

			std::uint32_t CrashInvalidStrings::GetLocalizeStringLengthSafe(void* This)
			{
				__try
				{
					return CrashInvalidStringsLength(This);
				}
				__except (1)
				{
					Console::LogWarning(Console::FORMS, "GetLocalizeStringLength return failed");

					return 0;
				}
			}

			const char* CrashInvalidStrings::GetLocalizeStringSafe(void* This)
			{
				__try
				{
					return CrashInvalidStringsGet(This);
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool CrashInvalidStrings::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				CrashInvalidStringsLength	= reinterpret_cast<TCrashInvalidStringsLength*>(Relocation(ID{ 409166, 1382155 }).WriteJump(&GetLocalizeStringLengthSafe));
				CrashInvalidStringsGet		= reinterpret_cast<TCrashInvalidStringsGet*>(Relocation(ID{ 381562 }).WriteJump(&GetLocalizeStringSafe));

				return true;
			}
		}
	}
}