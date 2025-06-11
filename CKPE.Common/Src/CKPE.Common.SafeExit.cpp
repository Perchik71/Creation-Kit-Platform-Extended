// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <cstdlib>
#include <CKPE.Common.SafeExit.h>
#include <CKPE.Detours.h>

namespace CKPE
{
	namespace Common
	{
		void SafeExit::Quit() noexcept(true)
		{
			QuitWithResult();
		}

		void SafeExit::QuitWithResult(std::int32_t err) noexcept(true)
		{
			if (!TerminateProcess(GetCurrentProcess(), (DWORD)err))
				std::abort();
		}

		void SafeExit::Hook::HKPostQuitMessage(int nExitCode) noexcept(true)
		{
			QuitWithResult(nExitCode);
		}

		void SafeExit::Hook::Initialize() noexcept(true)
		{
			auto base = (std::uintptr_t)GetModuleHandleA(nullptr);
			Detours::DetourIAT(base, "USER32.DLL", "PostQuitMessage", (std::uintptr_t)HKPostQuitMessage);
		}
	}
}