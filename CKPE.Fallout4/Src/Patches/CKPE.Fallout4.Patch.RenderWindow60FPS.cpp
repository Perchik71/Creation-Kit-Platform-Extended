// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSSettings.h>
#include <Patches/CKPE.Fallout4.Patch.RenderWindow60FPS.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
#if 0
			static std::uint32_t Hook_INIReadSetting(EditorAPI::INISettingCollection* collection, EditorAPI::Setting* setting);

			decltype(&Hook_INIReadSetting) ptrINIReadSetting = nullptr;

			static std::uint32_t Hook_INIReadSetting(EditorAPI::INISettingCollection* collection, EditorAPI::Setting* setting)
			{
				_MESSAGE("setting \"%s\" %p", setting->Name, setting);
				return ptrINIReadSetting(collection, setting);
			}
#endif

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool RenderWindow60FPS::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 2)
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
#if 0
				ptrINIReadSetting = (decltype(&Hook_INIReadSetting))Detours::DetourClassJump(0x14259AB50, Hook_INIReadSetting);
#endif

				return true;
			}
		}
	}
}