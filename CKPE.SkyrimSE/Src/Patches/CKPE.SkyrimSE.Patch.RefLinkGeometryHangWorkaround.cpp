// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
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

			bool RefLinkGeometryHangWorkaround::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RefLinkGeometryHangWorkaround::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Workaround for ref links and enable state parent links (2D lines) causing the CK to hang indefinitely when too many objects
				// are selected. This hack prevents said lines from being created or rendered.
				//
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 5);
				SafeWrite::WriteNop(__CKPE_OFFSET(1), 5);

				return true;
			}
		}
	}
}