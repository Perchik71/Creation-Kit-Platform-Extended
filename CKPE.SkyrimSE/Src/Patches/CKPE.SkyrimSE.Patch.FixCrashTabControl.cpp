// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <Windows.h>
#include <CommCtrl.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashTabControl.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixCrashTabControl::FixCrashTabControl() : Common::Patch()
			{
				SetName("Fix Crash TabControl");
			}

			bool FixCrashTabControl::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashTabControl::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashTabControl::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashTabControl::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashTabControl::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashTabControl::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when tab control buttons are deleted.
				// Uninitialized TCITEMA structure variables.
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			void FixCrashTabControl::sub(std::int64_t TabControlHandle, std::uint32_t TabIndex) noexcept(true)
			{
				TCITEMA itemInfo = {};

				if (TabCtrl_GetItem((HWND)TabControlHandle, TabIndex, &itemInfo))
					TabCtrl_DeleteItem((HWND)TabControlHandle, TabIndex);
			}
		}
	}
}