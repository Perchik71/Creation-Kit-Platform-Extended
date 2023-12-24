// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RefLinkGeometryHangWorkaround.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			RefLinkGeometryHangWorkaroundPatch::RefLinkGeometryHangWorkaroundPatch() : Module(GlobalEnginePtr)
			{}

			bool RefLinkGeometryHangWorkaroundPatch::HasOption() const
			{
				return true;
			}

			bool RefLinkGeometryHangWorkaroundPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RefLinkGeometryHangWorkaroundPatch::GetOptionName() const
			{
				return "CreationKit:bRefLinkGeometryHangWorkaround";
			}

			const char* RefLinkGeometryHangWorkaroundPatch::GetName() const
			{
				return "Ref Link Geometry Hang Workaround";
			}

			bool RefLinkGeometryHangWorkaroundPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RefLinkGeometryHangWorkaroundPatch::GetDependencies() const
			{
				return {};
			}

			bool RefLinkGeometryHangWorkaroundPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool RefLinkGeometryHangWorkaroundPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Workaround for ref links and enable state parent links (2D lines) causing the CK to hang indefinitely when too many objects
					// are selected. This hack prevents said lines from being created or rendered.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 5);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 5);

					return true;
				}

				return false;
			}

			bool RefLinkGeometryHangWorkaroundPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}