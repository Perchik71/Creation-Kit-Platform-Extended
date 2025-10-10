// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.AssertD3D11.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			AssertD3D11::AssertD3D11() : Common::Patch()
			{
				SetName("D3D11 FL11 features are not supported");
			}

			bool AssertD3D11::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AssertD3D11::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AssertD3D11::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AssertD3D11::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AssertD3D11::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool AssertD3D11::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			void AssertD3D11::sub() noexcept(true)
			{
				CKPE_ASSERT_MSG(false, "Creation Kit renderer initialization failed because your graphics card doesn't"
					" support D3D11 Feature Level 11 (FL11_0). Updating your drivers may fix this.");
			}
		}
	}
}