// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"
#include "Editor API/UI/UIBaseWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		namespace Classes = ::Core::Classes::UI;

		class BaseWindow : public Module
		{
		public:
			BaseWindow();
			~BaseWindow() = default;

			inline WNDPROC GetOldWndProc() const { return _oldWndProc; }
		protected:
			WNDPROC _oldWndProc;
		};
	}
}