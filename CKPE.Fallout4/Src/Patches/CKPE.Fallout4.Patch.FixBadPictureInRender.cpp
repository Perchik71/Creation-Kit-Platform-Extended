// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixBadPictureInRender.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixBadPictureInRender::FixBadPictureInRender() : Common::Patch()
			{
				SetName("Fix Bad Picture In Render");
			}

			bool FixBadPictureInRender::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixBadPictureInRender::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixBadPictureInRender::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixBadPictureInRender::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixBadPictureInRender::DoQuery() const noexcept(true)
			{
				auto ver = VersionLists::GetEditorVersion();
				return (ver != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1) &&
					(ver <= VersionLists::EDITOR_FALLOUT_C4_LAST);
			}

			bool FixBadPictureInRender::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Remove stuff init FXAA or TAA
				Relocation(ID{ 122998, 1575961 }, Offset{ 0x7D9, 0x9B5 }).WriteFill(NOP, 0x50);

				return true;
			}
		}
	}
}