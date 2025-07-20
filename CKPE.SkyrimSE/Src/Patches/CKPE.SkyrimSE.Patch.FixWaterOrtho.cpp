// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixWaterOrtho.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixWaterOrtho::FixWaterOrtho() : Common::Patch()
			{
				SetName("Fix Water In Ortho mode");
			}

			bool FixWaterOrtho::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixWaterOrtho::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixWaterOrtho::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixWaterOrtho::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixWaterOrtho::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixWaterOrtho::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for water not rendering correctly while using the orthographic (top-down) camera view. 
				// SSE camera scaling changes cause weird behavior with water shaders.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			void FixWaterOrtho::sub(std::uintptr_t a1, bool a2) noexcept(true)
			{
				memset((void*)a1, 0, 0x1C);

				// Unknown camera distance multiplier
				*(bool*)(a1 + 0x18) = a2;
				*(float*)(a1 + 0x10) = 1000.0f;
			}
		}
	}
}