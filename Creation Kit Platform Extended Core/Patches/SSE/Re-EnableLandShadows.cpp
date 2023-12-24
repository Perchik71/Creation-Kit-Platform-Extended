// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Re-EnableLandShadows.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			ReEnableLandShadowsPatch::ReEnableLandShadowsPatch() : Module(GlobalEnginePtr)
			{}

			bool ReEnableLandShadowsPatch::HasOption() const
			{
				return false;
			}

			bool ReEnableLandShadowsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ReEnableLandShadowsPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ReEnableLandShadowsPatch::GetName() const
			{
				return "Re-enable land shadows";
			}

			bool ReEnableLandShadowsPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ReEnableLandShadowsPatch::GetDependencies() const
			{
				return {};
			}

			bool ReEnableLandShadowsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool ReEnableLandShadowsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Re-enable land shadows. Instead of caching the upload once per frame, upload it on every draw call.
					// (BSBatchRenderer::Draw -> GEOMETRY_TYPE_DYNAMIC_TRISHAPE uiFrameCount)
					//
					// Fixes a bug where BSDynamicTriShape dynamic data would be written to 1 of 4 ring buffers in the shadowmap pass and
					// cached. At some point later in the frame sub_140D6BF00 would increment a counter and swap the currently used
					// buffer. In the main render pass DrawDynamicTriShape would use that new buffer instead of the previous one during
					// shadows. The data offset (m_VertexAllocationOffset) was always correct, but the wrong ring buffer was used.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 2);

					return true;
				}

				return false;
			}

			bool ReEnableLandShadowsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}