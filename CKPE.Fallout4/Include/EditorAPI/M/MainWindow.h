// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Main window menu command handlers.
			namespace MainWindow
			{
				// Toggles the fog setting.
				// CK 1.11.246.0 0x140583AE0.
				inline void ToggleFog() noexcept(true)
				{
					using func_t = decltype(&ToggleFog);
					static Common::Relocation<func_t> func{ Common::ID{ 506991 } };
					return func();
				}

				// Toggles the bShowMarkers setting and runs the render window show markers handler.
				// CK 1.11.246.0 0x140583ED0.
				inline void ToggleMarkers() noexcept(true)
				{
					using func_t = decltype(&ToggleMarkers);
					static Common::Relocation<func_t> func{ Common::ID{ 359818 } };
					return func();
				}
			}
		}
	}
}
