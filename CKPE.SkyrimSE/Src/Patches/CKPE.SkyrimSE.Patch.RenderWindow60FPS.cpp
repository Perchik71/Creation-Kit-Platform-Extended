// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.RenderWindow60FPS.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			RenderWindow60FPS::RenderWindow60FPS() : Common::Patch()
			{
				SetName("Render Window 60 FPS");
			}

			bool RenderWindow60FPS::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RenderWindow60FPS::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RenderWindow60FPS::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RenderWindow60FPS::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RenderWindow60FPS::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool RenderWindow60FPS::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RenderWindow60FPS::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Force render window to draw at 60fps (SetTimer(10ms))
				// DESC: BufferDesc.RefreshRate.Numerator = 60
				//
				Relocation(ID(203773), 0x6E8).Write({ USER_TIMER_MINIMUM });

				auto target = ID{ 488551, 1016974 };
				if (!_READ_OPTION_BOOL("CreationKit", "bRenderWindowVSync", true))
					// no VSync
					Relocation(target, Offset{ 0xEE, 0x1AF }).Write({ 0x33, 0xD2, 0x90 });

				// Remove lock framerate
				Relocation(target, Offset{ 0xCF, 0x124 }).Write(JMP);
				Relocation(target, Offset{ 0xFC, 0x1BD }).WriteFill(NOP, 2);

				return true;
			}
		}
	}
}