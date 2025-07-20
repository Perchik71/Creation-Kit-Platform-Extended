// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixReverbParameters.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixReverbParameters::FixReverbParameters() : Common::Patch()
			{
				SetName("Fix Reverb Parameters");
			}

			bool FixReverbParameters::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixReverbParameters::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixReverbParameters::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixReverbParameters::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixReverbParameters::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool FixReverbParameters::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Bethesda pointed to a function that is not WndProc
				// According to RTTI readings, this object has one less function compared to CK 1.6.1130
				// and the last function is exactly what is needed.

				auto __BGSReverbParametersRTTI = Common::RTTI::GetSingleton()->Find("class BGSReverbParameters");
				if (!__BGSReverbParametersRTTI || (__BGSReverbParametersRTTI->VFunctionCount != 102)) return false;

				ScopeSafeWrite vtable(__BGSReverbParametersRTTI->VTableAddress, 
					__BGSReverbParametersRTTI->VFunctionCount * 8);

				auto Class = (std::uintptr_t*)__BGSReverbParametersRTTI->VTableAddress;
				Class[84] = Class[101];

				return true;
			}
		}
	}
}