// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.RefLinkGeometryHangWorkaround.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			RefLinkGeometryHangWorkaround::RefLinkGeometryHangWorkaround() : Common::Patch()
			{
				SetName("Ref Link Geometry Hang Workaround");
			}

			bool RefLinkGeometryHangWorkaround::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* RefLinkGeometryHangWorkaround::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bRefLinkGeometryHangWorkaround";
			}

			bool RefLinkGeometryHangWorkaround::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RefLinkGeometryHangWorkaround::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RefLinkGeometryHangWorkaround::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool RefLinkGeometryHangWorkaround::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RefLinkGeometryHangWorkaround::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Workaround for ref links and enable state parent links (2D lines) causing the CK to hang indefinitely when too many objects
				// are selected. This hack prevents said lines from being created or rendered.
				//
				auto target = ID(346183);
				Relocation(target, 0x51).WriteFill(NOP, 5);
				Relocation(target, 0x9C).WriteFill(NOP, 5);

				return true;
			}
		}
	}
}