// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BaseWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		BaseWindow::BaseWindow() : Module(GlobalEnginePtr),
			_oldWndProc(nullptr)
		{}
	}
}