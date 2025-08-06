// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "CKPE.Common.Patch.h"
#include "CKPE.Common.UIBaseWindow.h"

namespace CKPE
{
	namespace Common
	{
		// so create record export class functions need create .cpp (omg)

		class CKPE_COMMON_API PatchBaseWindow : public Patch, public UI::CUIBaseWindow
		{
		protected:
			WNDPROC _oldWndProc{ nullptr };
		public:
			PatchBaseWindow() noexcept(true) = default;

			[[nodiscard]] WNDPROC GetOldWndProc() const noexcept(true);
		};
	}
}