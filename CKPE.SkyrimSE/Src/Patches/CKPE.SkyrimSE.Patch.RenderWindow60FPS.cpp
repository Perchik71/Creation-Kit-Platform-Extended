// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.SafeWrite.h>
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

			bool RenderWindow60FPS::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RenderWindow60FPS::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				//
				// Force render window to draw at 60fps (SetTimer(10ms))
				// DESC: BufferDesc.RefreshRate.Numerator = 60
				//
				text.Write(__CKPE_OFFSET(0), { USER_TIMER_MINIMUM });

				if (!_READ_OPTION_BOOL("CreationKit", "bRenderWindowVSync", true))
					// no VSync
					text.Write(__CKPE_OFFSET(1), { 0x33, 0xD2, 0x90 });

				// Remove lock framerate
				text.Write(__CKPE_OFFSET(2), { 0xEB });
				text.WriteNop(__CKPE_OFFSET(3), 2);

				return true;
			}
		}
	}
}