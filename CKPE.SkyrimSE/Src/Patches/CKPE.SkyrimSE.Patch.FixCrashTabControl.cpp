// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <Windows.h>
#include <CommCtrl.h>
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

			bool FixCrashTabControl::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixCrashTabControl::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashTabControl::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when tab control buttons are deleted.
				// Uninitialized TCITEMA structure variables.
				//
				Relocation(ID(138684)).WriteJump(&sub);

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