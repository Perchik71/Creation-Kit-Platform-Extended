// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CheckD3D11.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			CheckD3D11::CheckD3D11() : Common::Patch()
			{
				SetName("Check D3D11");
			}

			bool CheckD3D11::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CheckD3D11::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CheckD3D11::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CheckD3D11::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CheckD3D11::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CheckD3D11::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CheckD3D11::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Assert if D3D11 FL11 features are not supported
				//
				Relocation(ID(548492), Offset{ 0x396, 0x364 }).WriteCall(&sub);

				return true;
			}

			void CheckD3D11::sub() noexcept(true)
			{
				CKPE_ASSERT_MSG(false,
					"Creation Kit renderer initialization failed because your graphics card doesn't support D3D11 Feature Level 11 (FL11_0).\n"
					"Updating your drivers may fix this.");
			}
		}
	}
}