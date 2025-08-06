// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.PatchBaseWindow.h>

namespace CKPE
{
	namespace Common
	{
		WNDPROC PatchBaseWindow::GetOldWndProc() const noexcept(true)
		{ 
			return _oldWndProc; 
		}
	}
}