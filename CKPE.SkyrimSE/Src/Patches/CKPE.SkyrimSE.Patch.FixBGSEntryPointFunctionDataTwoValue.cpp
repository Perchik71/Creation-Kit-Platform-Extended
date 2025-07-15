// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixBGSEntryPointFunctionDataTwoValue.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef std::int64_t (*TFixBGSEntryPointFunctionDataTwoValueSub)(std::int32_t);
			TFixBGSEntryPointFunctionDataTwoValueSub FixBGSEntryPointFunctionDataTwoValueSub;

			FixBGSEntryPointFunctionDataTwoValue::FixBGSEntryPointFunctionDataTwoValue() : Common::Patch()
			{
				SetName("Fix BGSEntryPointFunctionDataTwoValue");
			}

			bool FixBGSEntryPointFunctionDataTwoValue::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixBGSEntryPointFunctionDataTwoValue::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixBGSEntryPointFunctionDataTwoValue::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixBGSEntryPointFunctionDataTwoValue::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixBGSEntryPointFunctionDataTwoValue::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBGSEntryPointFunctionDataTwoValue::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for "Could not select actor value X in LoadDialog for BGSEntryPointFunctionDataTwoValue." 
				// Use the editor id instead of perk name for the Perk Entry dialog selection.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&sub);
				FixBGSEntryPointFunctionDataTwoValueSub = (TFixBGSEntryPointFunctionDataTwoValueSub)
					__CKPE_OFFSET(1);

				return true;
			}

			const char* FixBGSEntryPointFunctionDataTwoValue::sub(std::int32_t ActorValueIndex) noexcept(true)
			{
				auto actorValue = FixBGSEntryPointFunctionDataTwoValueSub(ActorValueIndex);

				if (!actorValue)
					return nullptr;

				return *(const char**)(actorValue + 0x90);
			}
		}
	}
}