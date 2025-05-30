// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.Process.h>
#include <stdexcept>

namespace CKPE
{
	Process::Process() noexcept(true) : 
		Module((void*)GetModuleHandleW(nullptr))
	{
		_process = (void*)GetCurrentProcess();
		_processid = (std::uint32_t)GetCurrentProcessId();
	}

	void Process::Terminate() const noexcept(true)
	{
		if (!TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS))
			terminate();
		__assume(0);
	}
}