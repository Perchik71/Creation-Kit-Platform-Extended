// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixBadPictureInRender.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixBadPictureInRenderPatch::FixBadPictureInRenderPatch() : Module(GlobalEnginePtr)
			{}

			bool FixBadPictureInRenderPatch::HasOption() const
			{
				return false;
			}

			bool FixBadPictureInRenderPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixBadPictureInRenderPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixBadPictureInRenderPatch::GetName() const
			{
				return "Fix Bad Picture In Render";
			}

			bool FixBadPictureInRenderPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixBadPictureInRenderPatch::GetDependencies() const
			{
				return {};
			}

			bool FixBadPictureInRenderPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion != EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_943_1) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST);
			}

			bool FixBadPictureInRenderPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();
				if ((verPatch == 1) || (verPatch == 2))
				{
					// Remove stuff init FXAA or TAA
					lpRelocator->PatchNop(_RELDATA_RAV(0), verPatch == 1 ? 0x55 : 0x50);

					return true;
				}

				return false;
			}

			bool FixBadPictureInRenderPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}