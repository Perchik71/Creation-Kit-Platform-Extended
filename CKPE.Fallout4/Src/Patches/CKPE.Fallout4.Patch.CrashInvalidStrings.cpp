// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.Fallout4.Patch.CrashInvalidStrings.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_CrashInvalidStrings_sub = 0;

			bool CrashInvalidStrings::sub(void* This)
			{
				__try
				{
					return fast_call<bool>(pointer_CrashInvalidStrings_sub, This);
				}
				__except(1)
				{
					return false;
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
				return false;
			}

			std::vector<std::string> CrashInvalidStrings::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashInvalidStrings::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool CrashInvalidStrings::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				auto rtti = Common::RTTI::GetSingleton()->Find("class TESForm");
				if (rtti)
				{
				//	_CONSOLE("%llX", rtti->VTableAddress);

				/*	*(std::uintptr_t*)&EditorAPI::Forms::TESForm::GetFormEditorIDLengthImpl =
						Detours::DetourClassJump(*((std::uintptr_t*)(rtti->VTableAddress + 0x230)), 
							&EditorAPI::Forms::TESForm::GetFormEditorIDLength);
					*(std::uintptr_t*)&EditorAPI::Forms::TESForm::GetFormEditorIDImpl =
						Detours::DetourClassJump(*((std::uintptr_t*)(rtti->VTableAddress + 0x238)),
							&EditorAPI::Forms::TESForm::GetFormEditorID);*/

					return true;
				}

				//pointer_CrashInvalidStrings_sub = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}
		}
	}
}