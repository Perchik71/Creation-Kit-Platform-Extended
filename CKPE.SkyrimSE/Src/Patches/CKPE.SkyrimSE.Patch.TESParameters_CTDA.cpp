// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.TESParameters_CTDA.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void(*TTESParameters_CTDASub)(std::int64_t, std::int64_t*);

			static TTESParameters_CTDASub TESParameters_CTDASub;

			TESParameters_CTDA::TESParameters_CTDA() : Common::Patch()
			{
				SetName("TESParameters CTDA");
			}

			bool TESParameters_CTDA::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESParameters_CTDA::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESParameters_CTDA::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESParameters_CTDA::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESParameters_CTDA::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool TESParameters_CTDA::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for incorrect pointer truncate assertion while saving certain conditions (i.e 3DNPC.esp). TESParameters/CTDA.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				TESParameters_CTDASub = (TTESParameters_CTDASub)__CKPE_OFFSET(1);

				return true;
			}

			void TESParameters_CTDA::sub(std::int64_t Buffer, std::int64_t* Value) noexcept(true)
			{
				// The Value pointer can't be modified since it's used immediately after. Garbage data is still written to the plugin.
				std::int64_t newValue = *Value & 0x00000000FFFFFFFF;
				TESParameters_CTDASub(Buffer, &newValue);
			}
		}
	}
}