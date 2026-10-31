// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Main window status bar / loading progress.
			namespace ProgressWindow
			{
				// Sets the text of one part of the main window status bar (SB_SETTEXTA on the status bar window).
				// CK 1.11.246.0 0x1406D6D70.
				inline void SetStatusBarText(std::int32_t Part, const char* Text) noexcept(true)
				{
					using func_t = decltype(&SetStatusBarText);
					static Common::Relocation<func_t> func{ Common::ID{ 142976 } };
					return func(Part, Text);
				}

				// The status update called from the plugin loading loop (the OptimizationLoad patch throttles that call).
				// CK 1.11.246.0 0x14056EC00.
				inline void UpdateLoadStatus(std::int64_t Arg) noexcept(true)
				{
					using func_t = decltype(&UpdateLoadStatus);
					static Common::Relocation<func_t> func{ Common::ID{ 638107, 1670900 } };
					return func(Arg);
				}
			}
		}
	}
}
