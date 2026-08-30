// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.ReEnableLandShadows.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			ReEnableLandShadows::ReEnableLandShadows() : Common::Patch()
			{
				SetName("Re-enable land shadows");
			}

			bool ReEnableLandShadows::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ReEnableLandShadows::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ReEnableLandShadows::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ReEnableLandShadows::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ReEnableLandShadows::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool ReEnableLandShadows::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ReEnableLandShadows::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Re-enable land shadows. Instead of caching the upload once per frame, upload it on every draw call.
				// (BSBatchRenderer::Draw -> GEOMETRY_TYPE_DYNAMIC_TRISHAPE uiFrameCount)
				//
				// Fixes a bug where BSDynamicTriShape dynamic data would be written to 1 of 4 ring buffers in the shadowmap pass and
				// cached. At some point later in the frame sub_140D6BF00 would increment a counter and swap the currently used
				// buffer. In the main render pass DrawDynamicTriShape would use that new buffer instead of the previous one during
				// shadows. The data offset (m_VertexAllocationOffset) was always correct, but the wrong ring buffer was used.
				//
				Relocation(ID(316345), 0x1B4).WriteFill(NOP, 6);
				Relocation(ID(677842), Offset{ 0x281, 0x274 }).WriteFill(NOP, 2);

				return true;
			}
		}
	}
}